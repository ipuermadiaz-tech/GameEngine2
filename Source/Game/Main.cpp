#include "pch.h"
#include "Engine.h"
#include "SpriteGame/SpriteGame.h"
#include <memory>
#include <iostream>
#include <vector>

using namespace nu;

int main(int argc, char* argv[])
{
    // 1. Working directory setup
    nu::SetWorkingDirectory("Assets/SpriteGame");

    // 2. Initialize Engine
    Engine::Get().Initialize();

    // 3. Game setup (MUST be reset/destroyed BEFORE Engine::ShutDown)
    std::unique_ptr<SpriteGame> game = std::make_unique<SpriteGame>();
    game->Initialize();

    // --- JSON Load Test ---
    std::string buffer;
    if (ReadTextFile("data/data.json", buffer))
    {
        std::cout << buffer << std::endl;

        rapidjson::Document document;
        if (json::Load("data/data.json", document))
        {
            std::string name;
            int age;
            float speed;
            bool isAwake;
            nu::Vector2 position;
            nu::Vector3 color;

            JSON_READ(document, name);
            JSON_READ(document, age);
            JSON_READ(document, speed);
            JSON_READ(document, isAwake);
            JSON_READ(document, position);
            JSON_READ(document, color);

            std::cout << name << " " << age << " " << speed << " " << isAwake << std::endl;
            std::cout << position.x << " " << position.y << std::endl;
            std::cout << color.x << " " << color.y << " " << color.z << std::endl;
        }
    }

    // --- Resource & Variable Setup (OUTSIDE LOOP) ---
    auto texture = Resources().Get<Texture>("Textures/Background.png", Engine::Get().GetRenderer());

    std::vector<Vector2> points;
    for (int i = 0; i < 300; i++) {
        points.push_back(Vector2{ ru::RandomFloat(1280), ru::RandomFloat(1024) });
    }

    // Mesh declared once outside loop
    Mesh bulletMesh{
        {
            Vector2{ -1, -1 },
            Vector2{ 1, 0 },
            Vector2{ -1, 1 },
            Vector2{ -1, -1 },
        },
        Color{ 1.0f, 0.0f, 0.0f }
    };

    SDL_Event event;

    // --- MAIN LOOP ---
    bool quit = false;
    while (!quit)
    {
        // 1. INPUT / EVENTS
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            }
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE) {
                quit = true;
            }
        }

        if (Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_1))
        {
            Engine::Get().GetAudio().PlaySound("bass");
        }

        if (Engine::Get().GetInput().GetButtonPressed(Input::MouseButton::Right))
        {
            if (!points.empty()) { points.pop_back(); }
        }

        // 2. ENGINE & GAME UPDATE
        Engine::Get().Update();
        float dt = Engine::Get().GetTime().GetDeltaTime();
        game->Update(dt);

        // 3. RENDER (Clear -> Draw -> Present)
        Engine::Get().GetRenderer().Clear();

        // Draw Background
        if (texture) {
            Engine::Get().GetRenderer().DrawTexture(texture.get(), 0, 0);
        }

        // Draw Debug Points
        for (size_t i = 0; i < points.size(); i++) {
            Engine::Get().GetRenderer().SetColor(ru::RandomInt(255), ru::RandomInt(255), ru::RandomInt(255));
            Engine::Get().GetRenderer().DrawPoint(points[i].x, points[i].y);
        }

        // Draw Particle System & Game Scene
        Engine::Get().GetPS().Draw(Engine::Get().GetRenderer());
        game->Draw(Engine::Get().GetRenderer());

        Engine::Get().GetRenderer().Present();
    }

    // --- SHUTDOWN (SAFE ORDER) ---
    // Destroy Game FIRST so its Actors/Scene release Engine memory
    game.reset();

    // THEN shut down Engine managers
    Engine::Get().ShutDown();

    return 0;
}