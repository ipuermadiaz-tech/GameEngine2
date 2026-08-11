#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include <vector>
#include <memory>

namespace nu {
    class Texture;
    class Renderer;

    struct Particle
    {
        nu::Vector2 position;
        nu::Vector2 velocity;
        float lifespan{ 0.0f };
        nu::Color color;
        bool active{ false };

        std::shared_ptr<nu::Texture> texture{ nullptr };
        float scale{ 1.0f };
        float rotation{ 0.0f };
    };

    class ParticleSystem
    {
    public:
        ParticleSystem() = default;

        bool Initialize(size_t poolSize = 1000);
        void Shutdown();

        void Update(float dt);
        void Draw(const nu::Renderer& renderer);

        void AddParticle(const Particle& particle);

    private:
        Particle* GetFreeParticle();

    private:
        std::vector<Particle> m_particles;
    };
}