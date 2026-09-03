#pragma once
#include "Framework/Actor.h"
#include "Renderer/Renderer.h"
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

    // Explicit Copy Constructor to keep clone state clean
    Enemy(const Enemy& other)
        : nu::Actor(other)
        , m_counter{ other.m_counter }
        , upndown{ other.upndown }
        , m_ammo{ other.m_ammo }
        , m_speed{ other.m_speed }
        , m_fireTimer{ 0.0f }          // Reset timer on clone
        , m_fireRate{ other.m_fireRate }
        , m_health{ other.m_health }
        , m_maxHealth{ other.m_maxHealth }
    {}

    bool Read(const rapidjson::Value& value) override;

    void Initialize(const nu::ActorDesc& enemyDesc) override
    {
        Actor::Initialize(enemyDesc);
    }

    std::unique_ptr<nu::Object> Clone() const override;

    void Update(float dt) override;
    void OnCollision(Actor* other) override;
    void Draw(const nu::Renderer& renderer) const override;

    void SetHealth(int health) { m_health = health; m_maxHealth = health; }
    int GetHealth() const { return m_health; }

private:
    void SpawnBullet();

private:
    //for the boss and fairy to not drift downwards
    float m_baseY = -1.0f;
    int m_counter = 0;
    bool upndown = true;
    int m_ammo = 0;
    float m_speed = 200.0f;

    // Firing control
    float m_fireTimer = 0.0f;
    float m_fireRate = 2.5f;

    // HP & Fairy movement variables
    int m_health = 1;
    int m_maxHealth = 1;
    float m_zigzagTimer = 0.0f;
    int m_moveDirection = 1;
};