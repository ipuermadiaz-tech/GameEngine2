#include "Player.h"
#include "Renderer.h"
#include "Engine.h"
#include "SpaceGame.h"
#include "Factory.h"
#include "RigidBodyComponent.h"
#include <cmath>

static nu::RegisterActor<Player> regPlayer("Player");

void Player::Update(float dt)
{
    nu::Particle particle;
    particle.position = m_transform.position;
    particle.color = { 255.0f, 222.0f, 222.0f };
    particle.lifespan = ru::RandomFloat(1.0f, 1.1f);
    particle.velocity = { ru::RandomFloat(-200.0f, 200.0f), ru::RandomFloat(-200.0f, 200.0f) };
    particle.texture = nu::Resources().Get<nu::Texture>("textures/particle.png", nu::Engine::Get().GetRenderer());
    particle.scale = 0.5f;

    nu::Engine::Get().GetPS().AddParticle(particle);

    float thrust = 0.0f;
    if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_W)) thrust -= m_speed;
    if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_S)) thrust += m_speed;

    float rotate = 0.0f;
    if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_A)) rotate -= 180.0f;
    if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_D)) rotate += 180.0f;

    SetRotation(m_transform.rotation + rotate * dt);

    nu::Vector2 forward{ 0, -1 };
    nu::Vector2 velocity = forward.Rotate(m_transform.rotation * nu::DegToRad) * thrust;

    AddVelocity(velocity * dt);
    if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_Q) && nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_W) && fuel > 0) {
        AddVelocity(velocity * dt);
        fuel--;
        if (m_scene && m_scene->GetGame()) {
            ((SpaceGame*)m_scene->GetGame())->SetFuel(fuel);
        }
    }

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
                nu::Vector2 bulletDir = nu::Vector2{ 0, -1 }.Rotate(m_transform.rotation * nu::DegToRad);
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
                nu::Engine::Get().GetAudio().PlaySound("bass");
            }
        }
    }

    // Call base Update to perform position integration and nu::Wrap
    Actor::Update(dt);
}

void Player::OnCollision(Actor* other)
{
    if (other->GetTag() == "Enemy" || other->GetTag() == "Wall")
    {
        SetDestroyed();
        other->SetDestroyed();

        if (m_scene && m_scene->GetGame()) {
            ((SpaceGame*)m_scene->GetGame())->OnPlayerDead();
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

std::unique_ptr<nu::Object> Player::Clone() const
{
    return std::make_unique<Player>(*this);
}

void Player::Draw(const nu::Renderer& renderer) const
{
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