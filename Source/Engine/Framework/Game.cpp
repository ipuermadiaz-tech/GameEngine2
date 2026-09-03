#include "pch.h"
#include "Game.h"
#include "Scene.h"

namespace nu {

    Game::~Game() = default;

    void Game::SetScene(std::unique_ptr<Scene> scene)
    {
        m_scene = std::move(scene);
    }

    void Game::Update(float dt)
    {
        if (m_scene) m_scene->Update(dt);
    }

    void Game::Draw(const Renderer& renderer)
    {
        if (m_scene) m_scene->Draw(renderer);
    }
}