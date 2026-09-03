#pragma once

#include "Object.h"
#include "Core/Factory.h"
#include "Math/Transformer.h"
#include "Renderer/Renderer.h"
#include "Math/Math_Util.h"
#include "Renderer/Model.h"
#include "Renderer/Texture.h"
#include "Serialization/Json.h"
#include "Components/Component.h"
#include "Components/RenderComponent.h"

namespace nu
{
    class Scene;

    struct ActorDesc
    {
        std::string name;
        std::string tag;
        Transform transform;
        Vector2 velocity{ 0.0f, 0.0f };
        float damping = 0.0f;
        std::shared_ptr<nu::Texture> texture{ nullptr };
        Model model;
    };

    class Actor : public Object
    {
    public:
        // --- Constructors ---
        Actor(const ActorDesc& actorDesc) { Initialize(actorDesc); }
        Actor() : m_destroyed{ false } {}
        Actor(const Transform& transform) : m_transform{ transform }, m_destroyed{ false } {}
        Actor(const Transform& transform, const Model& model) : m_transform{ transform }, m_model{ model }, m_destroyed{ false } {}
        Actor(const Transform& transform, std::shared_ptr<nu::Texture> texture) : m_transform{ transform }, m_texture{ texture }, m_destroyed{ false } {}
        virtual ~Actor() = default;

        // --- Deep Copy Mechanics ---
        Actor(const Actor& other);
        Actor& operator=(const Actor& other);

        virtual void Initialize(const ActorDesc& actorDesc)
        {
            m_name = actorDesc.name;
            m_tag = actorDesc.tag;
            m_transform = actorDesc.transform;
            m_velocity = actorDesc.velocity;
            m_damping = actorDesc.damping;
            m_texture = actorDesc.texture;
            m_model = actorDesc.model;
            m_destroyed = false;
        }

        void Initialize();

        bool Read(const rapidjson::Value& value) override;

        virtual void Update(float dt);
        virtual void Draw(const Renderer& renderer) const;
        virtual void OnCollision(Actor* other) {}

        // --- Component Management ---
        template<typename T, typename... Args>
        T* AddComponent(Args&&... args);

        template<typename T>
        T* GetComponent();

        void AddComponent(std::unique_ptr<Component> component);

        // --- Accessors & Setters ---
        void SetTexture(std::shared_ptr<nu::Texture> texture) { m_texture = texture; }
        std::shared_ptr<nu::Texture> GetTexture() const { return m_texture; }

        void SetModel(const Model& model) { m_model = model; }
        const Model& GetModel() const { return m_model; }

        const Transform& GetTransform() const { return m_transform; }

        void SetPosition(const Vector2& position) { m_transform.position = position; }
        void SetRotation(float rotation) { m_transform.rotation = rotation; }
        void SetScale(float scale) { m_transform.scale = scale; }
        void SetVelocity(Vector2 velocity) { m_velocity = velocity; }
        void AddVelocity(Vector2 velocity) { m_velocity += velocity; }
        void SubstractVelocity(Vector2 velocity) { m_velocity -= velocity; }

        Vector2 GetVelocity() { return m_velocity; }
        const Vector2& GetVelocity() const { return m_velocity; }

        const std::string& GetTag() const { return m_tag; }
        void SetTag(const std::string& tag) { m_tag = tag; }

        Scene* GetScene() { return m_scene; }
        friend Scene;

        float GetRadious() const;
        void SetDestroyed(bool destroy = true) { m_destroyed = destroy; }
        bool GetDestroyed() const { return m_destroyed; }
        CLASS_PROTOTYPE(Actor)


        // Public fields
        Transform m_transform{};
        Scene* m_scene{ nullptr };
        bool m_destroyed = false;
        float m_lifespan = 0.0f;
        std::string type;

        //This is to render an actor below another,used notably in the sliding background
        int GetLayer() const { return m_layer; }
        void SetLayer(int layer) { m_layer = layer; }
    protected:
        int m_layer{ 0 };
        std::string m_tag;
        Vector2 m_velocity{ 0, 0 };
        float m_damping{ 0.0f };

        std::shared_ptr<nu::Texture> m_texture{ nullptr };
        Model m_model;

        int nowrap = 0;

        std::vector<std::unique_ptr<Component>> m_components;
    };

    template<typename T, typename... Args>
    T* Actor::AddComponent(Args&&... args)
    {
        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        component->m_owner = this;
        T* result = component.get();
        m_components.push_back(std::move(component));
        return result;
    }

    template<typename T>
    T* Actor::GetComponent()
    {
        for (auto& component : m_components)
        {
            T* result = dynamic_cast<T*>(component.get());
            if (result) return result;
        }
        return nullptr;
    }
}