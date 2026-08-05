#include "Player.h"
#include "Renderer.h"
#include "Engine.h"
#include "SpaceGame.h"
#include "Assets.h"
#include "Bullet.h"
void Player::Update(float dt)
{
   // Player(const PlayerDesc& player)
   // nu::Vector2 force = (0.0f, 0.f);
    nu::Particle particle;
    particle.position = m_transform.position;
    particle.color = { 255.0f, 222.0f, 222.0f };
    particle.lifespan = ru::RandomFloat(1.0f, 1.1f);
    particle.velocity = { ru::RandomFloat(-200.0f, 200.0f), ru::RandomFloat(-200.0f, 200.0f) };

    nu::Engine::Get().GetPS().AddParticle(particle);


    float thrust = 0.0f;
    
        if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_W)) thrust -= m_speed;
    
    if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_S)) thrust += m_speed;




    float rotate = 0.0f;


    if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_A)) rotate -= -180.0f;
    if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_D)) rotate += -180.0f;

    SetRotation(m_transform.rotation + rotate * dt);

    nu::Vector2 forward{ 0,1 };//->
    nu::Vector2 velocity=forward.Rotate(m_transform.rotation*nu::DegToRad)*thrust;
    
        AddVelocity(velocity * dt);
        if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_Q)&& nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_W) &&fuel>0) {
            AddVelocity(velocity * dt);
            fuel--;
            ((SpaceGame*)m_scene->GetGame())->SetFuel(fuel);

        }
        // 1. Only increment reload counter IF we are out of ammo / reloading!
        if (!canShoot) {
            counter++;
            if (counter >= counterTarget) {
                m_ammo = max_ammo;
                canShoot = true;
                counter = 0; // Reset counter for the next reload cycle
            }
        }

        // 2. Shooting logic
        if (canShoot) {
            if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_SPACE)) {
                BulletDesc desc;
                desc.name = "Bullet";
                desc.tag = "PlayerBullet";
                desc.model = assets::bulletModel;
                desc.transform = m_transform;
                desc.speed = 1000.0f;

                std::unique_ptr<Bullet> bullet = std::make_unique<Bullet>(desc);
                 m_scene->AddActor(std::move(bullet));

                m_ammo--;

                // Use <= 0 to safely catch accidental underflows
                if (m_ammo <= 0) {
                    m_ammo = 0;
                    canShoot = false;
                    counter = 0; // Start reload timer
                    nu::Engine::Get().GetAudio().PlaySound("bass");
                }
            }
        }

    
    Actor::Update(dt);
}

void Player::OnCollision(Actor* other)
{
    if (other->GetTag() == "Enemy")
    {
        SetDestroyed();
        other->SetDestroyed();


        ((SpaceGame*)m_scene->GetGame())->OnPlayerDead();
        // create particle explosion
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
	Actor::Draw(renderer);
}
