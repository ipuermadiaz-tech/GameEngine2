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

    bool Read(const rapidjson::Value& value) override;

    void Initialize(const nu::ActorDesc& enemyDesc) override
    {
        Actor::Initialize(enemyDesc);
    }

    std::unique_ptr<nu::Object> Clone() const override;

    void Update(float dt) override;
    void OnCollision(Actor* other) override;
    void Draw(const nu::Renderer& renderer) const override;

private:
    int m_counter = 0;
    bool upndown = true;
    int m_ammo = 0;
    float m_speed = 200.0f;
};