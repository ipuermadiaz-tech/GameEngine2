#include "pch.h"
#include "ParticleSystem.h"
#include "Renderer.h"
#include "Texture.h"
#include "Transformer.h"

namespace nu {

    bool ParticleSystem::Initialize(size_t size)
    {
        // reserve # particles in container
        m_particles.resize(size);
        return true;
    }

    void ParticleSystem::Shutdown()
    {
        m_particles.clear();
    }

    void ParticleSystem::Update(float dt)
    {
        // update active particles
        for (auto& particle : m_particles)
        {
            if (!particle.active) continue;

            // reduce particle.lifespan by subtracting delta time
            particle.lifespan -= dt;

            // set active to false if lifespan reaches zero
            if (particle.lifespan <= 0.0f)
            {
                particle.active = false;
                continue;
            }

            // update position with velocity (multiply by dt)
            particle.position += particle.velocity * dt;
        }

    }

    void ParticleSystem::Draw(const Renderer& renderer)
    {
        for (const auto& particle : m_particles)
        {
            if (particle.active && particle.lifespan > 0.0f)
            {
                if (particle.texture)
                {
                    Transform transform{ particle.position, particle.rotation, particle.scale };
                    renderer.DrawTexture(particle.texture.get(), transform);
                }
                else
                {
              
                    renderer.SetColor(
                        static_cast<Uint8>(particle.color.x),
                        static_cast<Uint8>(particle.color.y),
                        static_cast<Uint8>(particle.color.z)
                    );
                    renderer.DrawPoint(particle.position.x, particle.position.y);
                }
            }
        }
    }

    void ParticleSystem::AddParticle(const Particle& particle)
    {
        // get free particle
        Particle* freeParticle = GetFreeParticle();
        // check if free particle is not nullptr
        if (freeParticle)
        {
            // set free particle object with particle
            *freeParticle = particle;
            // set particle active
            freeParticle->active = true;
        }
    }

    Particle* ParticleSystem::GetFreeParticle()
    {
        // find and return inactive particle
        for (auto& particle : m_particles)
        {
            if (!particle.active)
            {
                return &particle;
            }
        }

        // no free particles, return nullptr
        return nullptr;
    }

} // namespace nu