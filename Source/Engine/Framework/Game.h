#pragma once
#include <memory>

namespace nu
{
    class Scene;

    class Game
    {
    public:
        Game() = default;
        Game(std::unique_ptr<Scene> scene) : m_scene{ std::move(scene) } {}
        virtual ~Game();

        virtual bool Initialize() { return true; };
        virtual void Shutdown() {};
        virtual void Update(float dt);
        virtual void Draw(const class Renderer& renderer);
        void SetScene(std::unique_ptr<Scene> scene);

    protected:
        std::unique_ptr<Scene> m_scene = nullptr;
    };
}