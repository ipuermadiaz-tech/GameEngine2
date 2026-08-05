#include "ParticleSystem.h"
#include "Renderer.h" // Ensure Renderer header is included for draw/color functions

bool nu::ParticleSystem::Initialize(size_t size)
{
	// reserve # particles in container
	m_particles.resize(size);

	return true;
}

void nu::ParticleSystem::Shutdown()
{
	m_particles.clear();
}

void nu::ParticleSystem::Update(float dt)
{
	// update active particles
	for (auto& particle : m_particles)
	{
		if (!particle.active) continue;

		// reduce particle.lifespan by subtracting delta time
		particle.lifespan -= dt;

		// set active if (particle.lifespan > 0);
		particle.active = (particle.lifespan > 0);

		// update position with velocity (multiply by dt)
		particle.position += particle.velocity * dt;
	}
}

void nu::ParticleSystem::Draw(const Renderer& renderer)
{
	// draw all active particles
	for (const auto& particle : m_particles)
	{
		if (particle.active)
		{
			// set particle color and draw point at current position
			renderer.SetColor(particle.color.x, particle.color.y, particle.color.z);
			renderer.DrawPoint(particle.position.x, particle.position.y);
		}
	}
}

void nu::ParticleSystem::AddParticle(const Particle& particle)
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

nu::Particle* nu::ParticleSystem::GetFreeParticle()
{
	// find and return inactive particle 
	for (auto& particle : m_particles)
	{
		// return pointer to inactive particle
		if (!particle.active)
		{
			return &particle;
		}
	}

	// no free particles, return nullptr
	return nullptr;
}