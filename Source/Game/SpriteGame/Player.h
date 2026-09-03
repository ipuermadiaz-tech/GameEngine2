#pragma once
#include "Framework/Actor.h"
#include "Renderer/Renderer.h"
#include <string>
#include <memory>

struct PlayerDesc : public nu::ActorDesc
{
    float speed = 800.0f;
    int ammo = 0;
};

class Player : public nu::Actor
{
public:
    Player(const PlayerDesc& playerDesc)
        : nu::Actor{ playerDesc }
        , m_speed{ playerDesc.speed }
        , m_ammo{ playerDesc.ammo }
    {}

    Player() = default;

    Player(const nu::Transform& transform) : nu::Actor{ transform } {}
    Player(const nu::Transform& transform, const nu::Model& model) : nu::Actor{ transform, model } {}

    Player(const Player& other)
        : nu::Actor(other)
        , counter{ other.counter }
        , counterTarget{ other.counterTarget }
        , canShoot{ other.canShoot }
        , max_ammo{ other.max_ammo }
        , m_ammo{ other.m_ammo }
        , fuel{ other.fuel }
        , m_speed{ other.m_speed }
    {}

    bool Read(const rapidjson::Value& value) override;

    void Update(float dt) override;
    void Draw(const nu::Renderer& renderer) const override;
    void OnCollision(Actor* other) override;

    void SetInvincible(float duration = 10.0f) {
        m_isInvincible = true;
        m_invincibleTimer = duration;
        m_flickerTimer = 0.0f;
        m_visible = true;
    }
    bool IsInvincible() const { return m_isInvincible; }

    int GetFuel() const { return fuel; }

    CLASS_PROTOTYPE(Player)

private:
    int counter = 0;
    int counterTarget = 70;
    bool canShoot = true;
    int max_ammo = 10;
    int m_ammo = max_ammo;
    int fuel = 99999999;
    float m_speed = 800.0f;

    bool m_isInvincible = false;
    float m_invincibleTimer = 0.0f;
    float m_flickerTimer = 0.0f;
    bool m_visible = true;
};