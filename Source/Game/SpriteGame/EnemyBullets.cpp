#include "EnemyBullet.h"
#include "Engine.h"
#include "Core/Factory.h"
#include "Components/RigidBodyComponent.h"
#include <iostream>

static nu::RegisterActor<EnemyBullet> regEnemyBullet("EnemyBullet");

bool EnemyBullet::Read(const rapidjson::Value& value)
{
    if (!nu::Actor::Read(value)) return false;

    if (value.HasMember("m_speed") && value["m_speed"].IsNumber()) {
        m_speed = value["m_speed"].GetFloat();
    }
    else if (value.HasMember("speed") && value["speed"].IsNumber()) {
        m_speed = value["speed"].GetFloat();
    }

    return true;
}

void EnemyBullet::Initialize(const nu::ActorDesc& desc)
{
    nu::Actor::Initialize(desc);
}

void EnemyBullet::Update(float dt)
{
    // Initialize direction towards player on the first frame if not set yet
    if (m_direction.x == 0.0f && m_direction.y == 1.0f && m_scene)
    {
        // Search by name FIRST, then fall back to tag search
        nu::Actor* player = m_scene->GetActorByName("PlayerPrototype");
        if (!player) player = m_scene->GetActorByName("Player");
     

        if (player)
        {
            m_direction = (player->GetTransform().position - m_transform.position).Normalized();

            float angleInRadians = m_direction.Angle();
            m_transform.rotation = angleInRadians * nu::RadToDeg;

            auto rigidBody = GetComponent<nu::RigidBodyComponent>();
            if (rigidBody)
            {
                rigidBody->velocity = m_direction * m_speed;
            }
        }
    }

    // Move bullet along m_direction
    auto rigidBody = GetComponent<nu::RigidBodyComponent>();
    if (rigidBody)
    {
        rigidBody->velocity = m_direction * m_speed;
    }
    else
    {
        m_transform.position += m_direction * m_speed * dt;
    }

    Actor::Update(dt);
}

void EnemyBullet::OnCollision(Actor* other)
{
    if (other->GetTag() == "Player")
    {
        SetDestroyed();
    }
}