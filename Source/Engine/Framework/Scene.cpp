#include "pch.h"
#include "Scene.h"
#include "Actor.h"
#include "Core/Factory.h"
#include "Serialization/Json.h"

namespace nu
{
    bool Scene::Load(const std::string& filename)
    {
        rapidjson::Document document;
        if (!json::Load(filename, document))
        {
            std::cerr << "[Scene ERROR] Failed to load JSON file: " << filename << std::endl;
            return false;
        }

        if (document.HasMember("prototypes") && document["prototypes"].IsArray())
        {
            for (const auto& protoVal : document["prototypes"].GetArray())
            {
                std::string type;
                if (!json::Read(protoVal, "type", type)) continue;

                auto obj = Factory::Instance().Create<Object>(type);
                auto actor = dynamic_cast<Actor*>(obj.get());

                if (actor)
                {
                    // Transfer ownership to scene prototype container
                    std::unique_ptr<Actor> actorPtr(static_cast<Actor*>(obj.release()));
                    actorPtr->Read(protoVal);

                    std::string name;
                    if (json::Read(protoVal, "name", name))
                    {
                        m_prototypes[name] = std::move(actorPtr);
                    }
                }
            }
        }

        return true;
    }

    Actor* Scene::Instantiate(const std::string& prototypeName, const Transform& transform)
    {
        auto it = m_prototypes.find(prototypeName);
        if (it == m_prototypes.end())
        {
            std::cerr << "[Scene ERROR] Prototype not found: " << prototypeName << std::endl;
            return nullptr;
        }

        auto object = it->second->Clone();
        std::unique_ptr<Actor> actor(dynamic_cast<Actor*>(object.release()));
        if (actor)
        {
            actor->SetPosition(transform.position);
            actor->SetRotation(transform.rotation);

            // Fallback to scale 1.0f if 0.0f is provided
            float scaleFactor = (transform.scale == 0.0f) ? 1.0f : transform.scale;
            actor->SetScale(scaleFactor);

            Actor* rawPtr = actor.get();
            AddActor(std::move(actor));
            return rawPtr;
        }

        return nullptr;
    }
    void Scene::DrawLayerRange(const nu::Renderer& renderer, int minLayer, int maxLayer)
    {
        // Ensure actors are sorted by layer
        std::stable_sort(m_actors.begin(), m_actors.end(),
            [](const std::unique_ptr<Actor>& a, const std::unique_ptr<Actor>& b) {
                if (!a || !b) return a < b;
                return a->GetLayer() < b->GetLayer();
            });

        for (const auto& actor : m_actors)
        {
            if (actor && actor->GetLayer() >= minLayer && actor->GetLayer() <= maxLayer)
            {
                actor->Draw(renderer);
            }
        }
    }

    void Scene::AddActor(std::unique_ptr<Actor> actor)
    {
        if (actor)
        {
            actor->m_scene = this;
            // Push to pending buffer to prevent vector iterator invalidation during Update loop
            m_pendingActors.push_back(std::move(actor));
        }
    }

    void Scene::RemoveAllActors()
    {
        m_actors.clear();
        m_pendingActors.clear();
    }

    void Scene::Update(float dt)
    {
        // 1. Update all active actors (index-based to be safe against size modifications)
        size_t currentActorCount = m_actors.size();
        for (size_t i = 0; i < currentActorCount; ++i)
        {
            if (m_actors[i])
            {
                m_actors[i]->Update(dt);
            }
        }

        // 2. Collision logic
        for (size_t i = 0; i < m_actors.size(); i++)
        {
            for (size_t j = i + 1; j < m_actors.size(); j++)
            {
                if (!m_actors[i] || !m_actors[j]) continue;
                if (m_actors[i]->GetDestroyed() || m_actors[j]->GetDestroyed()) continue;

                float distance = (m_actors[i]->GetTransform().position - m_actors[j]->GetTransform().position).Length();
                float radiusSum = m_actors[i]->GetRadious() + m_actors[j]->GetRadious();

                if (distance <= radiusSum)
                {
                    m_actors[i]->OnCollision(m_actors[j].get());
                    m_actors[j]->OnCollision(m_actors[i].get());
                }
            }
        }

        // 3. Cleanup destroyed actors
        auto it = m_actors.begin();
        while (it != m_actors.end())
        {
            if ((*it)->GetDestroyed())
            {
                it = m_actors.erase(it);
            }
            else
            {
                ++it;
            }
        }

        // 4. Append new actors instantiated during this frame's Update/Collision step
        if (!m_pendingActors.empty())
        {
            for (auto& pending : m_pendingActors)
            {
                pending->Initialize();
                m_actors.push_back(std::move(pending));
            }
            m_pendingActors.clear();
        }
    }

    void Scene::Draw(const nu::Renderer& renderer)
    {
        // Sort actors from lowest layer to highest layer before rendering
        std::stable_sort(m_actors.begin(), m_actors.end(),
            [](const std::unique_ptr<Actor>& a, const std::unique_ptr<Actor>& b) {
                if (!a || !b) return a < b;
                return a->GetLayer() < b->GetLayer();
            });

        for (const auto& actor : m_actors)
        {
            if (actor)
            {
                actor->Draw(renderer);
            }
        }
    }

    Actor* Scene::GetActorByName(const std::string& name)
    {
        for (auto& actor : m_actors)
        {
            if (actor && actor->GetName() == name)
            {
                return actor.get();
            }
        }

        for (auto& actor : m_pendingActors)
        {
            if (actor && actor->GetName() == name)
            {
                return actor.get();
            }
        }

        return nullptr;
    }
}