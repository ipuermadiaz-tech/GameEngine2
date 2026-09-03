#include "Enemy.h"
#include "Engine.h"
#include "Player.h"
#include "SpriteGame.h"
#include "Core/Factory.h"
#include "Components/RigidBodyComponent.h"
#include "Math/Rect.h"
#include "Core/Random.h"
#include <iostream>
#include <cmath>

static nu::RegisterActor<Enemy> regEnemy("Enemy");

void Enemy::Update(float dt)
{
    auto rigidBody = GetComponent<nu::RigidBodyComponent>();

    if (!rigidBody)
    {
        std::cerr << "Enemy has no RigidBodyComponent!\n";
        Actor::Update(dt);
        return;
    }

    if (m_tag == "Fairy" || m_tag == "Boss")
    {
        // Capture spawn height on first frame
        if (m_baseY < 0.0f) {
            m_baseY = m_transform.position.y;
        }

        m_zigzagTimer += dt;
        if (m_zigzagTimer >= 3.3f)
        {
            m_moveDirection *= -1;
            m_zigzagTimer = 0.0f;
        }

        float moveSpeedMultiplier = (m_tag == "Boss") ? 0.5f : 1.1f;

        // Apply horizontal velocity only
        float vx = m_moveDirection * (650.0f * moveSpeedMultiplier);
        rigidBody->velocity = nu::Vector2{ vx, 0.0f };

        // Lock Y directly around m_baseY using an un-resetting continuous timer
        static float continuousTimer = 0.0f; // or track m_totalTime in Enemy class
        m_transform.position.y = m_baseY + std::sin(m_zigzagTimer * 4.0f) * (30.0f * moveSpeedMultiplier);

        // --- Targeted Shooting Cadence ---
        m_fireRate = (m_tag == "Boss") ? 0.5f : 1.0f;
        m_fireTimer += dt;
        if (m_fireTimer >= m_fireRate)
        {
            m_fireTimer = 0.0f;

            nu::Actor* player = m_scene->GetActorByName("PlayerPrototype");
            if (player)
            {
                std::string bulletProto = (m_tag == "Boss") ? "EnemyBulletPrototype" : "FairyBulletPrototype";
                m_scene->Instantiate(bulletProto, m_transform);
            }
        }
    }
    else if (m_tag == "Enemy")
    {
        Actor* player = m_scene->GetActorByName("PlayerPrototype");

        if (player)
        {
            nu::Vector2 direction =
                (player->GetTransform().position - m_transform.position).Normalized();

            float angleInRadians = direction.Angle();
            m_transform.rotation = angleInRadians * nu::RadToDeg;

            rigidBody->velocity = direction * m_speed;
        }
        else
        {
            std::cerr << "Player not found!\n";
        }

        m_fireTimer += dt;
        if (m_fireTimer >= m_fireRate)
        {
            m_fireTimer = 0.0f;
            SpawnBullet();
        }
    }
    else if (m_tag == "Back" || m_tag == "Wall" || m_tag == "Bat")
    {
        nu::Vector2 forward{ 0.0f, 1.0f };
        rigidBody->velocity = forward * m_speed;
    }

    Actor::Update(dt);
}

void Enemy::SpawnBullet()
{
    if (!m_scene) return;

    m_scene->Instantiate("EnemyBulletPrototype", m_transform);
}

void Enemy::OnCollision(Actor* other)
{
    if (other->GetTag() == "PlayerBullet")
    {
        other->SetDestroyed();

        if (GetTag() == "Enemy" || GetTag() == "Bat" || GetTag() == "Fairy" || GetTag() == "Boss")
        {
            m_health--;

            if (m_health <= 0)
            {
                SetDestroyed();

                int particleCount = (GetTag() == "Boss") ? 300 : 100;
                for (int i = 0; i < particleCount; i++)
                {
                    nu::Particle particle;
                    particle.position = m_transform.position;
                    particle.color = { 1.0f, 1.0f, 1.0f };
                    particle.lifespan = ru::RandomFloat(0.5f, 2.5f);
                    particle.velocity = { ru::RandomFloat(-800.0f, 800.0f), ru::RandomFloat(-800.0f, 800.0f) };

                    nu::Engine::Get().GetPS().AddParticle(particle);
                }
            }
        }
    }
}

void Enemy::Draw(const nu::Renderer& renderer) const
{
    if ( m_tag == "Bat")
    {
        float originalRotation = m_transform.rotation;
        const_cast<Enemy*>(this)->m_transform.rotation += 90.0f;
        Actor::Draw(renderer);
        const_cast<Enemy*>(this)->m_transform.rotation = originalRotation;
    }
    else
    {
        Actor::Draw(renderer);
    }

    // --- Draw Health Bar for Fairy & Boss ---
    if ((m_tag == "Fairy" || m_tag == "Boss") && m_maxHealth > 1)
    {
        float barWidth = (m_tag == "Boss") ? 300.0f : 80.0f;
        float barHeight = (m_tag == "Boss") ? 16.0f : 8.0f;
        float yOffset = (m_tag == "Boss") ? 170.0f : 90.0f;

        float healthRatio = static_cast<float>(m_health) / static_cast<float>(m_maxHealth);
        nu::Vector2 barPos = m_transform.position - nu::Vector2{ barWidth * 0.5f, yOffset };

        // Background (Red)
        renderer.SetColor(255, 0, 0, 255);
        renderer.DrawFillRect(barPos.x, barPos.y, barWidth, barHeight);

        // Health Fill (Green)
        renderer.SetColor(0, 255, 0, 255);
        renderer.DrawFillRect(barPos.x, barPos.y, barWidth * healthRatio, barHeight);
    }
}

bool Enemy::Read(const rapidjson::Value& value)
{
    if (!nu::Actor::Read(value)) return false;

    if (value.HasMember("m_speed") && value["m_speed"].IsNumber()) {
        m_speed = value["m_speed"].GetFloat();
    }
    else if (value.HasMember("speed") && value["speed"].IsNumber()) {
        m_speed = value["speed"].GetFloat();
    }

    if (value.HasMember("m_health") && value["m_health"].IsInt()) {
        m_health = value["m_health"].GetInt();
        m_maxHealth = m_health;
    }

    return true;
}

std::unique_ptr<nu::Object> Enemy::Clone() const
{
    return std::make_unique<Enemy>(*this);
}