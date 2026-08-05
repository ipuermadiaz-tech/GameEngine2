#include "Enemy.h"
//#include "Renderer.h"
#include "Engine.h"
#include "Player.h"
#include "SpaceGame.h"
void Enemy::Update(float dt)
{
    if (GetName() == "Enemy") {
        if (upndown) {
            if (m_counter <= 1280) {
                m_counter++;
            }
            else {
                upndown = false;
            }

        }
        else {
            if (m_counter >= -1280) {
                m_counter--;
            }
            else {
                upndown = true;
            }
        }
        Player* player = m_scene->GetActorByName<Player>("Player");
        if (player)
        {
            nu::Vector2 direction = player->GetTransform().position - m_transform.position;
            float rotation = direction.Angle();
            SetRotation(rotation * nu::RadToDeg);

            //nu::Vector2 forward(1, 0);
            //forward.Rotate(rotation);
            //if (upndown) {
            //    AddVelocity(forward * m_speed * dt);
            //}
            //else {
            //    SubstractVelocity(forward * m_speed * dt);
            //}
        }

        // SetRotation(m_transform.rotation + rotate * dt);
    }
    nu::Vector2 forward{ 1,0 };//->
    nu::Vector2 velocity = forward.Rotate(m_transform.rotation * nu::DegToRad) * m_speed;
    AddVelocity(velocity * dt);
    Actor::Update(dt);
}
void Enemy::OnCollision(Actor* other)
{
    if (other->GetTag() == "PlayerBullet")
    {  
        if (GetName() == "Enemy") {
            SetDestroyed();
            ((SpaceGame*)m_scene->GetGame())->AddPoints(100);
        }
        
        other->SetDestroyed();

        
       

        // create particle explosion
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
