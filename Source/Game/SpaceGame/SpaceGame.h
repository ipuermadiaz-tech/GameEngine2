#pragma once
#include "Framework/Game.h"
#include "Text.h"

namespace nu {
    class Scene;
}

class SpaceGame : public nu::Game
{
public:
    enum class GameState
    {
        Title,
        StartGame,
        StartLevel,
        Game,
        GameOver
    };

public:
    SpaceGame() = default;
    SpaceGame(nu::Scene* scene) : Game{ scene } {}
    virtual ~SpaceGame() = default;

    bool Initialize() override;
    void Update(float dt) override;
    void Draw(const class nu::Renderer& renderer) override;

    void OnPlayerDead();
    void AddPoints(int points) { m_score += points; }
    void TakeLife(int points) { m_lives -= points; }
    void SetFuel(int points) { m_fuel = points; }

private:
    void SpawnMap();
    void SpawnPlayer();
    void SpawnEnemy();
    void SpawnWave();

private:
    int m_score{ 0 };
    int m_lives{ 0 };
    float m_stateTimer = 0.0f;
    float m_spawnTimer = 0.0f;
    float m_spawnTime = 3.0f;

    float max_Timer = 5.0f;
    int m_fuel = 0;
    int wave_counter = 5;
    int m_previousScore = -1;
    int m_previousLives = -1;
    int m_previousFuel = -1;
    GameState m_gameState = GameState::Title;

    nu::Text* m_titleText{ nullptr };
    nu::Text* m_gameOverText{ nullptr };
    nu::Text* m_gameText{ nullptr };
    nu::Text* m_liveText{ nullptr };
    nu::Text* m_fuelText{ nullptr };
};