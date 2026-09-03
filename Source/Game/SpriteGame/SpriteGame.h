#pragma once

#include "Framework/Game.h"
#include "Text.h"
#include <memory>

namespace nu {
    class Scene;
}

class SpriteGame : public nu::Game
{
public:
    enum class GameState
    {
        Title,
        StartGame,
        StartLevel,
        Game,
        BossTransition,
        PlayerDead,
        Congrats,
        GameOver
    };

public:
    SpriteGame() = default;
    SpriteGame(std::unique_ptr<nu::Scene> scene) : Game{ std::move(scene) } {}
    virtual ~SpriteGame() = default;

    bool Initialize() override;
    void Update(float dt) override;
    void Draw(const class nu::Renderer& renderer) override;

    void OnPlayerDead();
    void AddPoints(int points) { m_score += points; }
    void TakeLife(int points) { m_lives -= points; }
    void SetFuel(int points) { m_fuel = points; }

private:
    void SpawnPlayer(bool isRespawn);
    void SpawnEnemy();
    void SpawnWave();
    void SpawnWalls();
    void SpawnBoss();
    void SpawnAura();

private:
    // To not immediately go into resetting when being hit
    float m_deathTimer = 0.0f;
    const float DEATH_DELAY = 1.0f;

    // To give score periodically
    float m_scoreTimer{ 0.0f };

    int m_score{ 0 };
    int m_lives{ 0 };
    float m_stateTimer = 0.0f;
    float m_spawnTimer = 0.0f;
    float m_spawnTime = 2.0f;

    // Boss tracking flag
    bool m_bossSpawned = false;

    // Bat waves respawning logic
    int m_wavesRemaining = 0;
    float m_waveTimer = 0.0f;
    const float WAVE_DELAY = 0.5f;
    int m_consecutiveWaves = 0;

    // Fairy spawn logic
    float m_fairyTimer = 0.0f;
    bool m_fairyForceFail = false;

    float max_Timer = 3.0f;
    int m_fuel = 0;
    int wave_counter = 5;
    int m_previousScore = -1;
    int m_previousLives = -1;
    int m_previousFuel = -1;
    GameState m_gameState = GameState::Title;

    nu::Text* m_titleText{ nullptr };
    nu::Text* m_CongratsText{ nullptr };
    nu::Text* m_gameOverText{ nullptr };
    nu::Text* m_gameText{ nullptr };
    nu::Text* m_liveText{ nullptr };
    nu::Text* m_fuelText{ nullptr };

    // Sliding background
    float m_auraTimer{ 0.0f };
    const float AURA_INTERVAL{ 1.2f };
};