#include "SpaceGame.h"
#include "Engine.h"
#include "Player.h"
#include "Enemy.h"
#include "Factory.h"
#include "ResourceManager.h"
#include <iostream>
#include <algorithm>

using namespace nu;

bool SpaceGame::Initialize()
{
    Game::Initialize();

    m_scene = new Scene();
    m_scene->SetGame(this);

    if (!m_scene->Load("data/scene.json")) {
        std::cerr << "Failed to load scene.json!" << std::endl;
    }

    Engine::Get().GetAudio().AddSound("bass", "bass.wav");

    m_titleText = new Text(ResourceManager::Instance().GetWithID<Font>("title_font", "fonts/Handmade_Calligraphy.ttf", 128.0f));
    m_titleText->Create(Engine::Get().GetRenderer(), "Super Cool Space Game", Color{ 1.0f, 1.0f, 1.0f });

    auto gameFont = ResourceManager::Instance().GetWithID<Font>("game_font", "fonts/Handmade_Calligraphy.ttf", 64.0f);
    m_gameText = new Text(gameFont);
    m_liveText = new Text(gameFont);
    m_fuelText = new Text(gameFont);

    return true;
}

void SpaceGame::Update(float dt)
{
    dt = std::min(dt, 0.05f);

    switch (m_gameState) {
    case GameState::Title:
        if (Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_SPACE))
        {
            m_gameState = GameState::StartGame;
        }
        break;

    case GameState::StartGame:
        m_score = 0;
        m_lives = 3;
        m_gameState = GameState::StartLevel;
        break;

    case GameState::StartLevel:
        m_scene->RemoveAllActors();
        SpawnPlayer();
        m_gameState = GameState::Game;
        break;

    case GameState::Game:
        m_spawnTimer -= dt;
        if (m_spawnTimer <= 0.0f)
        {
            m_spawnTimer = max_Timer;

            SpawnEnemy();
            wave_counter--;
            if (wave_counter == 0) {
                SpawnWave();
                wave_counter = 14;
            }
        }

        // --- FIXED: Drive scene updates and physics collisions ---
        if (m_scene)
        {
            m_scene->Update(dt);
        }
        break;

    case GameState::GameOver:
        m_gameState = GameState::Title;
        m_score = 0;
        wave_counter = 10;
        m_scene->RemoveAllActors();
        break;
    }

    Game::Update(dt);
}

void SpaceGame::Draw(const nu::Renderer& renderer)
{
    switch (m_gameState) {
    case GameState::Title:
        if (m_titleText)
        {
            m_titleText->Draw(renderer, 400, 480);
        }
        break;

    case GameState::StartGame:
        break;

    case GameState::StartLevel:
        break;

    case GameState::Game:
        // --- FIXED: Render all active scene actors and components ---
        if (m_scene)
        {
            m_scene->Draw(renderer);
        }

        if (m_score != m_previousScore) {
            m_gameText->Create(renderer, "Score : " + std::to_string(m_score), Color{ 1.0f, 1.0f, 1.0f });
            m_previousScore = m_score;
        }

        if (m_lives != m_previousLives) {
            m_liveText->Create(renderer, "Lives : " + std::to_string(m_lives), Color{ 1.0f, 1.0f, 1.0f });
            m_previousLives = m_lives;
        }

        if (m_fuel != m_previousFuel) {
            m_fuelText->Create(renderer, "Fuel : " + std::to_string(m_fuel), Color{ 1.0f, 1.0f, 1.0f });
            m_previousFuel = m_fuel;
        }

        if (m_gameText) m_gameText->Draw(renderer, 30, 30);
        if (m_liveText) m_liveText->Draw(renderer, 800, 30);
        if (m_fuelText) m_fuelText->Draw(renderer, 1600, 30);
        break;

    case GameState::GameOver:
        max_Timer = 5.0f;
        break;
    }

    Game::Draw(renderer);
}

void SpaceGame::OnPlayerDead()
{
    m_lives--;
    wave_counter = 10;
    if (m_lives == 0) m_gameState = GameState::GameOver;
    else m_gameState = GameState::StartLevel;
}

void SpaceGame::SpawnPlayer()
{
    Actor* player = m_scene->Instantiate("PlayerPrototype", Transform{ Vector2{600.0f, 600.0f}, 0.0f, 1.0f });

    if (!player) {
        std::cerr << "[SpaceGame ERROR] Could not instantiate 'PlayerPrototype'! Verify scene.json key." << std::endl;
    }

    m_fuel = 2000;
}

void SpaceGame::SpawnEnemy()
{
    Transform enemyTransform{ Vector2{ ru::RandomFloat(100.0f, 1800.0f), ru::RandomFloat(100.0f, 600.0f) }, 0.0f, 1.0f };

    if (!m_scene->Instantiate("EnemyPrototype", enemyTransform)) {
        std::cerr << "Failed to instantiate EnemyPrototype!" << std::endl;
    }

    if (max_Timer > 2.5f) {
        max_Timer -= 0.2f;
    }
    if (max_Timer > 2.0f) {
        max_Timer -= 0.1f;
    }
}

void SpaceGame::SpawnWave()
{
    float thePosition = 0.0f;

    for (int i = 0; i < 60; i++) {
        thePosition += 30.0f;
        Transform wallTransform{ Vector2{thePosition, 0.0f}, 90.0f, 1.0f };

        if (!m_scene->Instantiate("WallPrototype", wallTransform)) {
            std::cerr << "Failed to instantiate WallPrototype!" << std::endl;
        }
    }
}