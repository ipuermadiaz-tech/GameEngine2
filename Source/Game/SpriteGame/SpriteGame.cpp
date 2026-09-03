#include "SpriteGame.h"
#include "Engine.h"
#include "Player.h"
#include "Enemy.h"
#include "Core/Factory.h"
#include "Resources/ResourceManager.h"
#include <iostream>
#include <algorithm>

using namespace nu;

bool SpriteGame::Initialize()
{
    Game::Initialize();

    m_scene = std::make_unique<nu::Scene>();
    m_scene->SetGame(this);

    if (!m_scene->Load("data/scene.json")) {
        std::cerr << "Failed to load scene.json!" << std::endl;
    }

    Engine::Get().GetAudio().AddSound("bass", "bass.wav");
    Engine::Get().GetAudio().AddSound("hit", "open-hat.wav");
    Engine::Get().GetAudio().AddSound("ost", "m-art_Chocolat.mp3");

    m_titleText = new Text(ResourceManager::Instance().GetWithID<Font>("title_font", "fonts/Handmade_Calligraphy.ttf", 128.0f));
    m_titleText->Create(Engine::Get().GetRenderer(), "Decently Cool Shoot Them Up", Color{ 1.0f, 1.0f, 1.0f });

    m_CongratsText = new Text(ResourceManager::Instance().GetWithID<Font>("title_font", "fonts/Handmade_Calligraphy.ttf", 128.0f));
    m_CongratsText->Create(Engine::Get().GetRenderer(), "You did a good job", Color{ 1.0f, 1.0f, 1.0f });

    auto gameFont = ResourceManager::Instance().GetWithID<Font>("game_font", "fonts/Handmade_Calligraphy.ttf", 64.0f);
    m_gameText = new Text(gameFont);
    m_liveText = new Text(gameFont);
    m_fuelText = new Text(gameFont);

    // Pre-cache background texture for transitions & screens
    ResourceManager::Instance().GetWithID<Texture>("Textures/Blackground.png", "Textures/Blackground.png", Engine::Get().GetRenderer());

    return true;
}

void SpriteGame::Update(float dt)
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
    {
        Engine::Get().GetAudio().PlaySound("ost", true);

        m_score = 0;
        m_lives = 3;

        max_Timer = 2.5f;
        m_spawnTimer = max_Timer;
        wave_counter = 5;
        m_consecutiveWaves = 0;

        m_wavesRemaining = 0;
        m_waveTimer = 0.0f;

        m_auraTimer = 0.0f;
        m_scoreTimer = 0.0f;
        m_deathTimer = 0.0f;
        m_fairyTimer = 0.0f;
        m_fairyForceFail = false;
        m_bossSpawned = false;

        m_previousScore = -1;
        m_previousLives = -1;
        m_previousFuel = -1;

        m_scene->RemoveAllActors();
        SpawnAura();
        Transform auraTransform{ Vector2{ 600.0f, 0.0f }, 0.0f, 1.0f };
        m_scene->Instantiate("AuraPrototype", auraTransform);

        Transform auraTransform3{ Vector2{ 600.0f, 400.0f }, 0.0f, 1.0f };
        m_scene->Instantiate("AuraPrototype", auraTransform3);

        SpawnPlayer(false);
        SpawnEnemy();
        m_gameState = GameState::Game;
        break;
    }

    case GameState::StartLevel: {
        Engine::Get().GetAudio().PauseSound("ost", false);

        SpawnAura();
        Transform auraTransform{ Vector2{ 600.0f, 0.0f }, 0.0f, 1.0f };

        if (!m_scene->Instantiate("AuraPrototype", auraTransform)) {
            std::cerr << "Failed to instantiate AuraPrototype!" << std::endl;
        }

        Transform auraTransform3{ Vector2{ 600.0f, 400.0f }, 0.0f, 1.0f };

        if (!m_scene->Instantiate("AuraPrototype", auraTransform3)) {
            std::cerr << "Failed to instantiate AuraPrototype!" << std::endl;
        }

        SpawnPlayer(true);
        if (!m_bossSpawned) {
            SpawnEnemy();
        }
        m_gameState = GameState::Game;

        break;
    }

    case GameState::BossTransition:
    {
        m_stateTimer -= dt;
        if (m_stateTimer <= 0.0f)
        {
            SpawnPlayer(false);
            SpawnBoss();
            m_gameState = GameState::Game;
        }

        if (m_scene)
        {
            m_scene->Update(dt);
        }
        break;
    }

    case GameState::Game:
    {
        bool isBossAlive = (m_scene && m_scene->GetActorByName("BossPrototype") != nullptr);

        // --- BOSS DEFEAT DETECTED ---
        if (m_bossSpawned && !isBossAlive)
        {
            m_stateTimer = 1.0f; // 1-second delay state
            m_gameState = GameState::Congrats;
            break;
        }

     
        if (m_score >= 8000 && !m_bossSpawned)
        {
            m_scene->RemoveAllActors();   // Clear all background/enemies
            m_bossSpawned = true;
            m_stateTimer = 1.0f;          // 1-second delay
            m_gameState = GameState::BossTransition;
            break;
        }

        // --- AURA SPAWN TIMER ---
        m_auraTimer += dt;
        if (m_auraTimer >= AURA_INTERVAL)
        {
            SpawnAura();
            m_auraTimer -= AURA_INTERVAL;
        }

        // --- REGULAR SPAWNS (Pausable when Boss is present) ---
        if (!isBossAlive)
        {
            // Fairy Spawn Logic
            m_fairyTimer += dt;
            if (m_fairyTimer >= 12.0f)
            {
                m_fairyTimer = 0.0f;

                if (m_score > 2000)
                {
                    if (m_fairyForceFail)
                    {
                        m_fairyForceFail = false;
                    }
                    else
                    {
                        if (ru::RandomFloat(0.0f, 1.0f) <= 0.50f)
                        {
                            Transform fairyTransform{ Vector2{ 200.0f, 100.0f }, 0.0f, 1.0f };
                            if (!m_scene->Instantiate("FairyPrototype", fairyTransform)) {
                                std::cerr << "Failed to instantiate FairyPrototype!" << std::endl;
                            }
                            m_fairyForceFail = true;
                        }
                    }
                }
            }

            // Bat Waves
            if (m_wavesRemaining > 0) {
                m_waveTimer -= dt;
                if (m_waveTimer <= 0.0f) {
                    float thePosition = 0.0f;
                    for (int i = 0; i < 18; i++) {
                        thePosition += 60.0f;
                        Transform batTransform{ Vector2{ thePosition, 0.0f }, 90.0f, 1.0f };

                        if (!m_scene->Instantiate("BatPrototype", batTransform)) {
                            std::cerr << "Failed to instantiate BatPrototype!" << std::endl;
                        }
                    }

                    m_wavesRemaining--;
                    m_waveTimer = WAVE_DELAY;
                }
            }

            // Regular Enemies & Wall Spawns
            m_spawnTimer -= dt;
            if (m_spawnTimer <= 0.0f)
            {
                m_spawnTimer = max_Timer;

                SpawnEnemy();
                wave_counter--;
                if (wave_counter <= 0) {
                    if (m_consecutiveWaves >= 2) {
                        SpawnWalls();
                        m_consecutiveWaves = 0;
                    }
                    else {
                        if (std::rand() % 2 == 0) {
                            SpawnWave();
                            m_consecutiveWaves++;
                        }
                        else {
                            SpawnWalls();
                            m_consecutiveWaves = 0;
                        }
                    }

                    wave_counter = 10;
                }
            }
        }

        m_scoreTimer += dt;
        if (m_scoreTimer >= 1.0f)
        {
            AddPoints(100);
            m_scoreTimer -= 1.0f;
        }

        if (m_scene)
        {
            m_scene->Update(dt);
        }
        break;
    }

    case GameState::Congrats:
        if (m_stateTimer > 0.0f)
        {
            m_stateTimer -= dt;
            if (m_stateTimer <= 0.0f)
            {
                //Engine::Get().GetAudio().StopSound("ost");
                m_scene->RemoveAllActors();
            }

            if (m_scene)
            {
                m_scene->Update(dt);
            }
        }
        else
        {
            if (Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_SPACE))
            {
                Engine::Get().GetAudio().StopSound("ost");
                m_scene->RemoveAllActors();
                m_gameState = GameState::Title;
            }
        }
        break;

    case GameState::PlayerDead:
        m_deathTimer -= dt;
        Engine::Get().GetAudio().PlaySound("hit", false);
        if (m_deathTimer <= 0.0f)
        {
            if (m_lives == 0)
            {
                Engine::Get().GetAudio().StopSound("ost");
                m_gameState = GameState::GameOver;
            }
            else
            {
                m_gameState = GameState::StartLevel;
            }
        }
        break;

    case GameState::GameOver:
        if (Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_SPACE))
        {
            m_scene->RemoveAllActors();
            m_gameState = GameState::Title;
        }
        break;
    }

    Game::Update(dt);
}

void SpriteGame::Draw(const nu::Renderer& renderer)
{
    Game::Draw(renderer);

    switch (m_gameState) {
    case GameState::Title:
        if (m_titleText)
        {
            m_titleText->Draw(renderer, 100, 480);
        }
        break;

    case GameState::StartGame:
    case GameState::StartLevel:
        break;

    case GameState::Game:
    case GameState::PlayerDead:
        if (m_scene)
        {
            m_scene->DrawLayerRange(renderer, -9999, -1);
            Engine::Get().GetPS().Draw(renderer);
            m_scene->DrawLayerRange(renderer, 0, 9999);
        }

        if (m_score != m_previousScore) {
            m_gameText->Create(renderer, "Score : " + std::to_string(m_score), Color{ 1.0f, 1.0f, 1.0f });
            m_previousScore = m_score;
        }

        if (m_lives != m_previousLives) {
            m_liveText->Create(renderer, "Lives : " + std::to_string(m_lives), Color{ 1.0f, 1.0f, 1.0f });
            m_previousLives = m_lives;
        }

        if (m_gameText) m_gameText->Draw(renderer, 30, 30);
        if (m_liveText) m_liveText->Draw(renderer, 1000, 30);

        break;

    case GameState::BossTransition:
    {
        auto bgTexture = ResourceManager::Instance().GetWithID<Texture>("Textures/Blackground.png", "Textures/Blackground.png", Engine::Get().GetRenderer());
        if (bgTexture)
        {
            renderer.DrawTexture(bgTexture.get(), 0, 0);
        }
        break;
    }

    case GameState::Congrats:
        if (m_stateTimer > 0.0f)
        {
            if (m_scene)
            {
                m_scene->DrawLayerRange(renderer, -9999, -1);
                Engine::Get().GetPS().Draw(renderer);
                m_scene->DrawLayerRange(renderer, 0, 9999);
            }
        }
        else
        {
            // Draw black background behind the victory text
            auto bgTexture = ResourceManager::Instance().GetWithID<Texture>("Textures/Blackground.png", "Textures/Blackground.png", Engine::Get().GetRenderer());
            if (bgTexture)
            {
                renderer.DrawTexture(bgTexture.get(), 0, 0);
            }

            if (m_CongratsText)
            {
                m_CongratsText->Draw(renderer, 100, 480);
            }
        }
        break;

    case GameState::GameOver:
        if (m_titleText)
        {
            m_titleText->Draw(renderer, 100, 480);
        }
        break;
    }
}

void SpriteGame::OnPlayerDead()
{
    m_lives--;
    wave_counter = 10;

    Engine::Get().GetAudio().PauseSound("ost", true);

    m_deathTimer = DEATH_DELAY;
    m_gameState = GameState::PlayerDead;
}

void SpriteGame::SpawnAura()
{
    Transform auraTransform{ Vector2{ 600.0f, -400.0f }, 0.0f, 1.0f };

    if (!m_scene->Instantiate("AuraPrototype", auraTransform)) {
        std::cerr << "Failed to instantiate AuraPrototype!" << std::endl;
    }
}

void SpriteGame::SpawnPlayer(bool isRespawn)
{
    Actor* actor = m_scene->Instantiate("PlayerPrototype", Transform{ Vector2{600.0f, 600.0f}, 0.0f, 1.0f });

    if (!actor) {
        std::cerr << "[SpaceGame ERROR] Could not instantiate 'PlayerPrototype'! Verify scene.json key." << std::endl;
        return;
    }

    Player* player = dynamic_cast<Player*>(actor);
    if (player && isRespawn)
    {
        player->SetInvincible(8.0f);
    }

    m_fuel = 2000;
}

void SpriteGame::SpawnEnemy()
{
    Transform enemyTransform{ Vector2{ ru::RandomFloat(100.0f, 1800.0f), 0.0f }, 0.0f, 1.0f };

    if (!m_scene->Instantiate("EnemyPrototype", enemyTransform)) {
        std::cerr << "Failed to instantiate EnemyPrototype!" << std::endl;
    }

    if (ru::RandomFloat(0.0f, 1.0f) <= 0.50f && max_Timer < 2.0f)
    {
        nu::Actor* player = m_scene->GetActorByName("PlayerPrototype");
        if (player)
        {
            wave_counter--;
            Transform lockerTransform{ Vector2{ player->m_transform.position.x, 0.0f }, 0.0f, 1.0f };

            if (!m_scene->Instantiate("LockerPrototype", lockerTransform)) {
                std::cerr << "Failed to instantiate LockerPrototype!" << std::endl;
            }
        }
    }

    if (max_Timer > 1.8f) {
        max_Timer -= 0.1f;
    }
}

void SpriteGame::SpawnWave()
{
    float thePosition = 0.0f;
    for (int i = 0; i < 18; i++) {
        thePosition += 60.0f;
        Transform batTransform{ Vector2{ thePosition, 0.0f }, 90.0f, 1.0f };

        if (!m_scene->Instantiate("BatPrototype", batTransform)) {
            std::cerr << "Failed to instantiate BatPrototype!" << std::endl;
        }
    }

    m_wavesRemaining = 3;
    m_waveTimer = WAVE_DELAY;
}

void SpriteGame::SpawnWalls()
{
    bool leftToRight = (std::rand() % 2 == 0);

    float startPosition = leftToRight ? 0.0f : 1200.0f;
    float step = leftToRight ? 30.0f : -30.0f;

    for (int i = 0; i < 35; i++) {
        startPosition += step;
        Transform wallTransform{ Vector2{ startPosition, 0.0f }, 90.0f, 1.0f };

        if (!m_scene->Instantiate("WallPrototype", wallTransform)) {
            std::cerr << "Failed to instantiate WallPrototype!" << std::endl;
        }
    }
}

void SpriteGame::SpawnBoss()
{
    Transform bossTransform{ Vector2{ 600.0f, 200.0f }, 0.0f, 1.0f };

    if (!m_scene->Instantiate("BossPrototype", bossTransform)) {
        std::cerr << "[SpaceGame ERROR] Could not instantiate 'BossPrototype'! Check scene.json." << std::endl;
    }
}