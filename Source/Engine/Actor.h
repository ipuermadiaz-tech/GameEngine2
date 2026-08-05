#pragma once
#include "Transformer.h"
#include "Renderer.h"
#include "Math_Util.h"
#include "Model.h"
#include <iostream>


namespace nu
{
    class Scene;

    struct  ActorDesc
    {
        std::string name;
        std::string tag;
        Transform transform;
        Vector2 velocity{ 0.0f, 0.0f };
        float damping = 0.0f;
        Model model;
    };

    class Actor

    {

    public:
        Actor(const ActorDesc& actorDesc) :
            m_name{ actorDesc.name },
            m_tag{ actorDesc.tag },
            m_transform{ actorDesc.transform },
            m_velocity{ actorDesc.velocity },
            m_damping{ actorDesc.damping },
            m_model{ actorDesc.model },
            m_destroyed{ false } // Explicitly set
        {
        }

        Actor() : m_destroyed{ false } {}

        Actor(const Transform& transform) :
            m_transform{ transform },
            m_destroyed{ false }
        {
        }

        Actor(const Transform& transform, const Model& model) :
            m_transform{ transform },
            m_model{ model },
            m_destroyed{ false }
        {
        }


    //Actor() = default;
    virtual void Update(float dt);

    virtual void Draw(const Renderer& renderer) const;
    virtual void OnCollision(Actor* other) {}
    const Transform& GetTransform() { return m_transform; }

    void SetPosition(const Vector2& position) { m_transform.position = position; }
    void SetRotation(float rotation) { m_transform.rotation = rotation; }
    void SetScale(float scale) { m_transform.scale = scale; }
    void SetVelocity(Vector2 velocity) { m_velocity = velocity; }
    void AddVelocity(Vector2 velocity) { m_velocity += velocity; }
    void SubstractVelocity(Vector2 velocity) { m_velocity -= velocity; }

    Vector2 GetVelocity() { return m_velocity; };
    const Vector2& GetVelocity() const { return m_velocity; }   

   const std::string GetName() const { return m_name; }
   const std::string& GetTag() const { return m_tag; }

   Scene* GetScene() { return m_scene; }
   friend Scene;
   float GetRadious() const;
   void SetDestroyed(bool destroy = true) { m_destroyed = destroy; }
   bool GetDestroyed() const { return m_destroyed;}



protected:
    std::string m_name="";
    std::string m_tag;
    Transform m_transform{};
    Vector2 m_velocity{ 0,0 };
    float m_damping{ 0.0f };
    Model m_model;
    Scene* m_scene{ nullptr };
    bool m_destroyed = false;
    int nowrap = 0;

};


}
