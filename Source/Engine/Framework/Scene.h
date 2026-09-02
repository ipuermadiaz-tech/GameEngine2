#pragma once
#include <vector>
#include "Actor.h"
#include "Math/Transformer.h"
#include <iostream>
#include <string>
#include <map>
#include <memory>

namespace nu {
    class Game;

    class Scene {
    public:
        Scene() = default;
        ~Scene() = default;

        bool Load(const std::string& filename);
        void Update(float dt);
        void Draw(const Renderer& renderer);

        void AddActor(std::unique_ptr<Actor> actor);
        void RemoveAllActors();

        Actor* GetActorByName(const std::string& name);
        Actor* Instantiate(const std::string& prototypeName, const Transform& transform = Transform{});

        void SetGame(Game* game) { m_game = game; }
        Game* GetGame() { return m_game; }

        template <typename T>
        T* GetActor();

    private:
        Game* m_game = nullptr;
        std::vector<std::unique_ptr<Actor>> m_actors;
        std::vector<std::unique_ptr<Actor>> m_pendingActors; // Added to prevent vector reallocation crashes during Update()
        std::map<std::string, std::unique_ptr<Actor>> m_prototypes;
    };

    template <typename T>
    inline T* Scene::GetActor()
    {
        for (auto& actor : m_actors)
        {
            T* result = dynamic_cast<T*>(actor.get());
            if (result) return result;
        }
        return nullptr;
    }
}