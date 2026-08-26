#pragma once
#include "Actor.h"
#include "Renderer.h"
#include <memory>

struct EnemyDesc : public nu::ActorDesc
{
    float speed = 200.0f;
    int ammo = 0;
};

class Enemy : public nu::Actor
{
public:
    Enemy() = default;

    Enemy(const EnemyDesc& enemyDesc)
        : nu::Actor{ enemyDesc }
        , m_speed{ enemyDesc.speed }
        , m_ammo{ enemyDesc.ammo }
    {}

    Enemy(const nu::Transform& transform) : Actor{ transform } {}
    Enemy(const nu::Transform& transform, const nu::Model& model) : Actor{ transform, model } {}

    // Copy Constructor
    Enemy(const Enemy& other)
        : nu::Actor(other)
        , m_counter{ other.m_counter }
        , upndown{ other.upndown }
        , m_ammo{ other.m_ammo }
        , m_speed{ other.m_speed }
    {}

    bool Read(const rapidjson::Value& value) override;

    void Initialize(const nu::ActorDesc& enemyDesc) override
    {
        Actor::Initialize(enemyDesc);
    }

    void Update(float dt) override;
    void OnCollision(Actor* other) override;
    void Draw(const nu::Renderer& renderer) const override;

    CLASS_PROTOTYPE(Enemy)

private:
    int m_counter = 0;
    bool upndown = true;
    int m_ammo = 0;
    float m_speed = 200.0f;
};