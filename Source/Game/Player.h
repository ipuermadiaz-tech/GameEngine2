#pragma once
#include "Actor.h"
#include "Renderer.h"
#include <string>


struct PlayerDesc : public nu::ActorDesc
{
    float speed = 800.0f;
    int ammo = 0;
};
class Player:public nu::Actor
{
public:
    Player(const PlayerDesc& playerDesc)
        : nu::Actor{ playerDesc }   
        , m_speed{ playerDesc.speed }      
        , m_ammo{ playerDesc.ammo }         
    {
    }


    Player() = default;
    Player(const nu::Transform& transform) : Actor{ transform } {}

    Player(const nu::Transform& transform, const nu::Model& model) :
        Actor{transform,model}
    {
    }
    void Update(float dt) override;
    void Draw(const class nu::Renderer& renderer) const override;
    void OnCollision(Actor* other) override;
    int GetFuel() { return fuel; };

private:
    int counter = 0;
    int counterTarget = 120;
    bool canShoot = true;
    int max_ammo = 40;
	int m_ammo = max_ammo;
    //int max_fuel = 2000;
    int fuel = 2000;
    float m_speed = 800.0f;
};