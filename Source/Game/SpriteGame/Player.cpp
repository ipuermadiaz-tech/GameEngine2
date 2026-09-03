#include "Player.h"
#include "Renderer/Renderer.h"
#include "Engine.h"
#include "SpriteGame.h"
#include "Core/Factory.h"
#include "Components/RigidBodyComponent.h"
#include <cmath>

static nu::RegisterActor<Player> regPlayer("Player");

void Player::Update(float dt)
{
    // --- INVINCIBILITY & FLICKER TIMERS ---
    if (m_isInvincible)
    {
        m_invincibleTimer -= dt;
        if (m_invincibleTimer <= 0.0f)
        {
            m_isInvincible = false;
            m_visible = true; // Ensure sprite is fully visible when timer ends
        }
        else
        {
            // Rapid flicker effect (toggles visible state every 0.08s)
            m_flickerTimer += dt;
            if (m_flickerTimer >= 0.08f)
            {
                m_flickerTimer = 0.0f;
                m_visible = !m_visible;
            }
        }
    }

    // --- PARTICLE TRAIL LOGIC ---
    nu::Particle particle;
    particle.position = m_transform.position;
    particle.color = { 255.0f, 222.0f, 222.0f };
    particle.lifespan = ru::RandomFloat(0.4f, 0.5f);
    particle.velocity = { ru::RandomFloat(-200.0f, 200.0f), ru::RandomFloat(-200.0f, 200.0f) };
    particle.scale = 0.5f;

    // Swap particle texture based on invincibility state
    if (m_isInvincible)
    {
        particle.texture = nu::Resources().Get<nu::Texture>("textures/particle_2.png", nu::Engine::Get().GetRenderer());
    }
    else
    {
        particle.texture = nu::Resources().Get<nu::Texture>("textures/particle.png", nu::Engine::Get().GetRenderer());
    }

    nu::Engine::Get().GetPS().AddParticle(particle);

    // --- TURBO SPEED LOGIC ---
    float currentSpeed = m_speed;
    if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_LSHIFT))
    {
        currentSpeed *= 2.0f;
    }

    // --- 8-DIRECTIONAL MOVEMENT ---
    nu::Vector2 direction{ 0.0f, 0.0f };

    if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_W)) direction.y -= 1.0f;
    if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_S)) direction.y += 1.0f;
    if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_A)) direction.x -= 1.0f;
    if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_D)) direction.x += 1.0f;

    if (direction.LengthSqr() > 0.0f)
    {
        direction = direction.Normalized();
    }

    SetRotation(0.0f);

    auto physicsComponent = GetComponent<nu::RigidBodyComponent>();
    if (physicsComponent)
    {
        nu::Vector2 velocity = direction * currentSpeed;
        physicsComponent->AddVelocity(velocity * dt);
    }

    // --- VERTICAL EDGE CLAMPING ---
    if (m_transform.position.y < 20.0f)
    {
        m_transform.position.y = 20.0f;
        if (physicsComponent) physicsComponent->velocity.y = 0.0f;
    }
    else if (m_transform.position.y > 1020.0f)
    {
        m_transform.position.y = 1020.0f;
        if (physicsComponent) physicsComponent->velocity.y = 0.0f;
    }

    // --- WEAPON MECHANICS ---
    if (!canShoot) {
        counter++;
        if (counter >= counterTarget) {
            m_ammo = max_ammo;
            canShoot = true;
            counter = 0;
        }
    }

    if (canShoot) {
        if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_SPACE)) {
            nu::Actor* bullet = m_scene->Instantiate("BulletPrototype", m_transform);
            if (bullet) {
                nu::Vector2 bulletDir{ 0.0f, -1.0f };
                auto* rb = bullet->GetComponent<nu::RigidBodyComponent>();
                if (rb) {
                    rb->velocity = bulletDir * 1600.0f;
                }
                else {
                    bullet->SetVelocity(bulletDir * 1600.0f);
                }
            }

            m_ammo--;
            if (m_ammo <= 0) {
                m_ammo = 0;
                canShoot = false;
                counter = 0;
            }
        }
    }

    Actor::Update(dt);
}

void Player::OnCollision(Actor* other)
{
    // Ignore all damage while invincible
    if (m_isInvincible) return;

    if (other->GetTag() == "Enemy" || other->GetTag() == "Wall" || other->GetTag() == "Bat" || other->GetTag() == "EnemyBullet" || other->GetTag() == "Boss" || other->GetTag() == "Fairy")
    {
        SetDestroyed();
        other->SetDestroyed();

        if (m_scene && m_scene->GetGame()) {
            ((SpriteGame*)m_scene->GetGame())->OnPlayerDead();
        }

        for (int i = 0; i < 100; i++)
        {
            nu::Particle particle;
            particle.position = m_transform.position;
            particle.color = { 255.0f, 0.0f, 0.0f };
            particle.lifespan = ru::RandomFloat(0.5f, 2.0f);
            particle.velocity = { ru::RandomFloat(-600.0f, 600.0f), ru::RandomFloat(-600.0f, 600.0f) };

            nu::Engine::Get().GetPS().AddParticle(particle);
        }
    }
}

void Player::Draw(const nu::Renderer& renderer) const
{
    // Skip rendering mesh on off-flicker frames
    if (m_isInvincible && !m_visible) return;

    Actor::Draw(renderer);
}

bool Player::Read(const rapidjson::Value& value)
{
    if (!nu::Actor::Read(value)) return false;

    JSON_READ(value, m_speed);

    if (value.HasMember("m_ammo")) {
        JSON_READ(value, m_ammo);
    }

    return true;
}