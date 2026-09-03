#pragma once
#include "Framework/Actor.h"
#include "Math/Vector2.h"

struct EnemyBulletDesc : public nu::ActorDesc
{
    float speed = 300.0f;
};

class EnemyBullet : public nu::Actor
{
public:
    EnemyBullet() = default;

    EnemyBullet(const EnemyBulletDesc& bulletDesc)
        : nu::Actor{ bulletDesc }
        , m_speed{ bulletDesc.speed }
    {}

    // Copy constructor required for Clone()
    EnemyBullet(const EnemyBullet& other)
        : nu::Actor(other)
        , m_speed{ other.m_speed }
        , m_direction{ other.m_direction }
    {}

    bool Read(const rapidjson::Value& value) override;

    void Initialize(const nu::ActorDesc& desc) override;
    // REMOVED: std::unique_ptr<nu::Object> Clone() const override;

    void Update(float dt) override;
    void OnCollision(Actor* other) override;

    CLASS_PROTOTYPE(EnemyBullet) // CLASS_PROTOTYPE handles Clone() for you

private:
    float m_speed = 300.0f;
    nu::Vector2 m_direction{ 0.0f, 1.0f };
};