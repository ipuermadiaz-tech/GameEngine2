#pragma once
#include "Actor.h"
#include "Renderer.h"
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

    bool Read(const rapidjson::Value& value) override;

    Player(const nu::Transform& transform) : nu::Actor{ transform } {}
    Player(const nu::Transform& transform, const nu::Model& model) : nu::Actor{ transform, model } {}

    std::unique_ptr<nu::Object> Clone() const override;

    void Update(float dt) override;
    void Draw(const nu::Renderer& renderer) const override;
    void OnCollision(Actor* other) override;
    int GetFuel() const { return fuel; }

private:
    int counter = 0;
    int counterTarget = 120;
    bool canShoot = true;
    int max_ammo = 40;
    int m_ammo = max_ammo;
    int fuel = 2000;
    float m_speed = 800.0f;
};