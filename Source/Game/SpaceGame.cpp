#include "SpaceGame.h"
#include "Engine.h"
#include "Player.h"
#include "Enemy.h"
#include "Assets.h"
using namespace nu;
bool SpaceGame::Initialize()
{
    Game::Initialize();
    m_scene = new Scene();
    m_scene->SetGame(this);
    Engine::Get().GetAudio().AddSound("bass", "bass.wav");
    m_titleFont= new Font();
    m_titleFont->Load("fonts/Handmade_Calligraphy.ttf", 128);
    m_titleText= new Text(m_titleFont);
    m_titleText->Create(Engine::Get().GetRenderer(), "Super Cool Space Game", Color{ 1.0f, 1.0f, 1.0f });

    m_gameFont = new Font();
    m_gameFont->Load("fonts/Handmade_Calligraphy.ttf", 64);

    m_gameText = new Text(m_gameFont);
    m_liveText = new Text(m_gameFont);
    m_fuelText = new Text(m_gameFont);
    

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
        SpawnPlayer();
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
        m_titleText->Draw(renderer, 400, 480);
        break;
    case GameState::StartGame:
        break;
    case GameState::StartLevel:
        break;
    case GameState::Game:
        // Update textures ONLY when the values actually change
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
        // Drawing existing textures is lightning fast!
        m_gameText->Draw(renderer, 30, 30);
        m_liveText->Draw(renderer, 800, 30);
        m_fuelText->Draw(renderer, 1600, 30);
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

    Mesh mesh{ {Vector2{-3.0f, 3.0f}, Vector2{3.0f, 3.0f}, Vector2{0.0f, 0.0f},Vector2{-3.0f, 3.0f}}, Color{255.0f, 255.0f, 255.0f} };
    Mesh mesh3{ {Vector2{2.0f, 7.0f}, Vector2{6.0f, 6.0f}, Vector2{0.0f, 0.0f},Vector2{2.0f, 7.0f}}, Color{255.0f, 10.0f, 2.0f} };
    Mesh mesh4{ {Vector2{8.0f, 7.0f}, Vector2{6.0f, 6.0f}, Vector2{2.0f, 2.0f},Vector2{8.0f, 7.0f}}, Color{255.0f, 250.0f, 2.0f} };
    //Mesh mesh4{ {Vector2{-2.0f, 7.0f}, Vector2{6.0f, 6.0f},Vector2{-2.0f, 7.0f}}, Color{705.0f, 102.0f, 2.0f} };
    Model model = std::vector<Mesh>{ mesh,mesh3,mesh4 };
    PlayerDesc playerDesc;
    playerDesc.name = "Player";
    playerDesc.tag = "Player";
    playerDesc.speed = 1200.0f;
    playerDesc.damping = 2.3f;
    playerDesc.model = model;
    playerDesc.transform = Transform{ Vector2{640.0f,512.0f},0.0f,15.0f };


    std::unique_ptr<Player> player = std::make_unique<Player>(playerDesc);
    m_scene->AddActor(std::move(player));
    m_fuel = 2000;
}

void SpaceGame::SpawnEnemy()
{
    Mesh mesh{ {Vector2{-3.0f, 3.0f}, Vector2{3.0f, 3.0f}, Vector2{0.0f, 0.0f},Vector2{-3.0f, 3.0f}}, Color{255.0f, 255.0f, 255.0f} };
    Model model2 = std::vector<Mesh>{ mesh };
    EnemyDesc enemyDesc;
    enemyDesc.name = "Enemy";
    enemyDesc.tag = "Enemy";
    enemyDesc.speed = 500.0f;
    enemyDesc.damping = 3.0f;
    enemyDesc.model = model2;
    enemyDesc.transform = Transform{ Vector2{ru::RandomFloat(1000.0f,1900.0f),ru::RandomFloat(800.0f,1200.0f)},90.0f,10.0f };


    std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>(enemyDesc);
    m_scene->AddActor(std::move(enemy));
    if (max_Timer > 2.5f) {
        max_Timer -= 0.2f;
    }
    if (max_Timer > 2.0f) {
        max_Timer -= 0.1f;
    }
  
}
void SpaceGame::SpawnWave()
{
    float thePosition=0.0f;
    
    Mesh mesh{ {Vector2{-3.0f, 3.0f}, Vector2{3.0f, 3.0f}, Vector2{2.0f, 2.0f},Vector2{-6.0f, 6.0f}}, Color{255.0f, 255.0f, 255.0f} };
    Model model2 = std::vector<Mesh>{ mesh };
    EnemyDesc enemyDesc;
    enemyDesc.name = "Enemy2";
    enemyDesc.tag = "Enemy";
    enemyDesc.speed = 300.0f;
    enemyDesc.damping = 3.0f;
    enemyDesc.model = model2;
    for (int i = 0;i < 60;i++) {
        thePosition += 30;
        enemyDesc.transform = Transform{ Vector2{thePosition,0},90.0f,10.0f };
        std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>(enemyDesc);
        m_scene->AddActor(std::move(enemy));
    }

}