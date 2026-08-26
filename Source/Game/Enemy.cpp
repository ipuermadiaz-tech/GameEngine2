#include "Enemy.h"
#include "Engine.h"
#include "Player.h"
#include "SpaceGame.h"
#include "Factory.h"
#include "RigidBodyComponent.h"
#include <iostream>

static nu::RegisterActor<Enemy> regEnemy("Enemy");

bool Enemy::Read(const rapidjson::Value& value)
{
    if (!nu::Actor::Read(value)) return false;

    JSON_READ(value, m_speed);
    return true;
}

void Enemy::Update(float dt)
{
    // --- Regular Enemy Homing ---
    if (m_tag == "Enemy")
    {
        Actor* player = m_scene->GetActorByName("Player");

        if (player)
        {
            nu::Vector2 direction = (player->GetTransform().position - m_transform.position).Normalized();
            float angleInRadians = direction.Angle();
            m_transform.rotation = angleInRadians * nu::RadToDeg;

            m_velocity = direction * m_speed;
        }
    }
    // --- Moving Wall Movement ---
    else if (m_tag == "Wall")
    {
        nu::Vector2 forward{ 0.0f, 1.0f }; // Downward direction
        m_velocity = forward * m_speed;
    }

    Actor::Update(dt);
}

void Enemy::OnCollision(Actor* other)
{
    if (other->GetTag() == "PlayerBullet")
    {
        if (GetTag() == "Enemy")
        {
            SetDestroyed();
            if (m_scene && m_scene->GetGame())
            {
                static_cast<SpaceGame*>(m_scene->GetGame())->AddPoints(100);
            }
        }

        other->SetDestroyed();

        // Create particle explosion
        for (int i = 0; i < 100; i++)
        {
            nu::Particle particle;
            particle.position = m_transform.position;
            particle.color = { 1.0f, 1.0f, 1.0f };
            particle.lifespan = ru::RandomFloat(0.5f, 2.0f);
            particle.velocity = { ru::RandomFloat(-600.0f, 600.0f), ru::RandomFloat(-600.0f, 600.0f) };

            nu::Engine::Get().GetPS().AddParticle(particle);
        }
    }
}

void Enemy::Draw(const nu::Renderer& renderer) const
{
    Actor::Draw(renderer);
}