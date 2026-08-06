#include <SDL3/SDL.h>
#include "Enemy.h"
//#include <iostream>
//#include <vector>
#include "Engine.h"
#include "Player.h"
#include "Main.h"
#include <fmod.hpp>
#include <map>
#include "SpaceGame.h"
#include <memory>

using namespace nu;


//class Object
//{
//public:
//    Object() { std::cout << "constructur\n"; }
//    Object() { std::cout << "destructor\n"; }
//
//    Object(const Object& object) { std::cout << "copy\n"; }
//    Object& operator = (const Object& object) { std::cout << "Assignment\n";return *this; }
//};

int main(int argc, char* argv[]) {



  
    //std::cout << "===========object============\n";
    //{
    //    Object objectA;
    //    Object objectB(objectA);
    //    Object objectC;
    //    objectC = objectA;
    //}
    //std::cout << "****************************raw pointers*******************\n"; {
    //    Object* objectA = new Object();
    //    std::cout << objectA << std::endl;
    //    Object* objectB = new Object(*objectA);
    //    std::cout << objectB << std::endl;
    //    Object* objectC = nullptr;
    //    objectC = objectA;
    //    std::cout << objectC << std::endl;

    //    delete objectA;
    //    delete objectB;
    //
    //
    //
    //}
    //std::cout << "****************************raw pointers*******************\n"; {
    //    std::unique_ptr<Object> objectA = std::make_unique<Object>();
    //    std::cout << objectA.get() << std::endl;
    //    std::unique_ptr<Object> objectB;
    //    objectB = std::move(objectA);
    //    std::cout << objectB.get() << std::endl;

    //    objectB.reset();
    //}

    nu::SetWorkingDirectory("Assets");
  // Engine engine;
    Engine::Get().Initialize();

    SpaceGame sgame;
    sgame.Initialize();



   
    

    
    


    union
    {
        struct { float x, y, z; };
        struct { float r, g, b; };
    };

    nu::Renderer renderer;
    //renderer.Initialize("Game Engine", 1920, 1024);
    //g_engine.Initialize();


    Vector2 velocity(0.0f, 0.0f);
    Vector2 position{ 640,512 };
    float speed = 400.0f;
    
    Scene scene;

    // mesh/Model
  
   
    //Player

    
   





    //Actor player{ Transform{Vector2{640.0f,512.0f},0.0f,50.0f} };

	nu::Input input;


   //Vector2 v[300];
    std::vector<Vector2> points;

    float xs[300];
    float ys[300];

    for (int i = 0;i < 300;i++) {
        xs[i] = ru::RandomFloat(1280);
        ys[i] = ru::RandomFloat(1024);
    }
	//engine 
    SDL_Event event;

    uint64_t ticks = SDL_GetTicksNS();
    uint64_t prevticks=ticks;
    // create texture, using shared_ptr so texture can be shared
// create texture, using shared_ptr so texture can be shared
    //std::shared_ptr<nu::Texture> texture = std::make_shared<nu::Texture>();

    //texture->Load("Textures/Thorn of Love.png", nu::Engine::Get().GetRenderer());


    auto texture = Resources().Get<Texture>("Textures/Thorn of Love.png", Engine::Get().GetRenderer());
    

    //Main Loop
    bool quit = false;
    	//input.Initialize();
    while (!quit) {
        

        //UPDATE
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            }
            if (event.type == SDL_EVENT_KEY_DOWN&&event.key.scancode == SDL_SCANCODE_ESCAPE){
                quit=true;
            }
       }

        if (Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_1))
        {
            Engine::Get().GetAudio().PlaySound("bass");
        }
        //emgome
        Engine::Get().Update();
        sgame.Update(Engine::Get().GetTime().GetDeltaTime());
        //if (Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_1))
        //{
        //    audio->playSound(sounds[0], nullptr, false, nullptr);
        //}

        //if (Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_2))
        //{
        //    audio->playSound(sounds[1], nullptr, false, nullptr);
        //}
        //else if (Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_3))
        //{
        //    audio->playSound(sounds[2], nullptr, false, nullptr);
        //}
        //else if (Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_4))
        //{
        //    audio->playSound(sounds[3], nullptr, false, nullptr);
        //}
        //else if (Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_5))
        //{
        //    audio->playSound(sounds[4], nullptr, false, nullptr);
        //}


        Mesh bulletMesh{
  {
    Vector2{ -1, -1 },
    Vector2{ 1, 0 },
    Vector2{ -1, 1 },
    Vector2{ -1, -1 },
  },
  Color{ 1.0f, 0.0f, 0.0f }
        };

     




		//input.Update();


        prevticks = ticks;
        ticks = SDL_GetTicksNS();

        

        float seconds = (float)ticks / 1'000'000'000;
        float dt = (float)(ticks - prevticks) / 1'000'000'000;

		Vector2 mousePosition;
		SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

      
        Vector2 force = (0.0f,0.f);
   

        velocity += (force * dt);
        position += (velocity * dt);


        position.x = Wrap(0.0f, 1920.0f, position.x);

        position.y = Wrap(0.0f, 1024.0f, position.y);

        //if (position.x > 10) position.x = 0;
        //if (position.x < 0) position.x = 1279;

        //RENDER
        Engine::Get().GetRenderer().SetColor(0, 0, 0);
       
        //g_engine.GetRenderer().DrawFillRect(position.x, position.y, 50, 50);

        //for (int i = 0; i < 300; i++) {
        //    Vector2 vec{ ru::RandomFloat() ,ru::RandomFloat() };
        //    points.push_back(vec);

        //}
        //for (int i = 0; i < 100; i++) {
        //    renderer.SetColor(ru::RandomInt(255), ru::RandomInt(255), ru::RandomInt(255));

        //    //vect[i] = vect[i]+vel;]
        //   
        //    //renderer.DrawPoint(vect[i].x, vect[i].y);
        //    renderer.DrawPoint(ru::RandomInt(1920), ru::RandomInt(1024));
        //}

        for (int i = 0; i < points.size(); i++) {
            Engine::Get().GetRenderer().SetColor(ru::RandomInt(255), ru::RandomInt(255), ru::RandomInt(255));

            //points[i] = points[i];

            Engine::Get().GetRenderer().DrawPoint(points[i].x, points[i].y);
          
        }
        

        //if(input.GetButtonPressed(Input::MouseButton::Left))
        //{
        //    points.push_back(input.GetMousePosition());
        //}
        //player.Update(dt);
        //enemy.Update(dt);
        scene.Update(dt);
        //if (g_engine.GetInput().GetButtonDown(Input::MouseButton::Left))
        //{
        //    if (points.empty())
        //    {
        //        points.push_back(g_engine.GetInput().GetMousePosition());
        //    }

        //           Vector2 v= points.back()- g_engine.GetInput().GetMousePosition();
        //  if (v.Length() > 10.0f) {
        //      points.push_back(g_engine.GetInput().GetMousePosition());
        //  }
        //}

        //if (!points.empty()) {
        //    for (size_t i = 0;i < points.size() - 1;i++) {
        //        g_engine.GetRenderer().SetColor(ru::RandomInt(255), ru::RandomInt(255), ru::RandomInt(255));
        //        g_engine.GetRenderer().DrawLine(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);

        //    }
        //}
        if (Engine::Get().GetInput().GetButtonPressed(Input::MouseButton::Right))
        {
            if (!points.empty()) { points.pop_back(); }

        }


        

        //character
        
            

        //for (int i = 0; i < 4; i++) {
        //    renderer.SetColor(rand() % 256, rand() % 256, rand() % 256);
        //    renderer.DrawFillRect(ru::RandomFloat(1920), ru::RandomFloat(1024), ru::RandomFloat(100), ru::RandomFloat(200));
        //}
        //for (int i = 0; i < 11; i++) {
        //    renderer.SetColor(rand() % 256, rand() % 256, rand() % 256);
        //    renderer.DrawLine(ru::RandomFloat(1920), ru::RandomFloat(1024), ru::RandomFloat(100), ru::RandomFloat(200));
        //}

        Engine::Get().GetRenderer().Clear();
        Engine::Get().GetPS().Draw(Engine::Get().GetRenderer());


        Engine::Get().GetRenderer().SetColor(255, 255,0);
        Engine::Get().GetRenderer().DrawTexture(texture.get(), 600, 600);
        //player.Draw(g_engine.GetRenderer());
        //enemy.Draw(g_engine.GetRenderer());

        scene.Draw(Engine::Get().GetRenderer());
        sgame.Draw(Engine::Get().GetRenderer());


        Engine::Get().GetRenderer().Present();
    }
    //SHUTDOWN
    Engine::Get().ShutDown();

    return 0;
}