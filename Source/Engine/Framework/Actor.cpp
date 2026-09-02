#include "pch.h"
#include "Actor.h"
#include "Math/Vector2.h"
#include "Math/Transformer.h"
#include "Renderer/Renderer.h"
#include "Math/Math_Util.h"
#include "Renderer/Model.h"
#include "Core/Factory.h"
#include "Engine.h"
#include "Resources/ResourceManager.h"
#include "Serialization/Json.h"
#include "Components/CircleColliderComponent.h"


namespace nu
{
    static RegisterActor<Actor> registerActor("Actor");

    Actor::Actor(const Actor& other)
    {
        m_name = other.m_name;
        m_tag = other.m_tag;
        m_transform = other.m_transform;
        m_velocity = other.m_velocity;
        m_damping = other.m_damping;
        m_texture = other.m_texture;
        m_model = other.m_model;
        m_lifespan = other.m_lifespan;
        m_destroyed = other.m_destroyed;
        m_scene = other.m_scene;
        type = other.type;

        for (const auto& component : other.m_components)
        {
            auto clonedObj = component->Clone();
            if (clonedObj)
            {
                auto clonedComp = std::unique_ptr<Component>(dynamic_cast<Component*>(clonedObj.release()));
                if (clonedComp)
                {
                    clonedComp->m_owner = this;
                    m_components.push_back(std::move(clonedComp));
                }
            }
        }
    }

    Actor& Actor::operator=(const Actor& other)
    {
        if (this == &other) return *this;

        m_name = other.m_name;
        m_tag = other.m_tag;
        m_transform = other.m_transform;
        m_velocity = other.m_velocity;
        m_damping = other.m_damping;
        m_texture = other.m_texture;
        m_model = other.m_model;
        m_lifespan = other.m_lifespan;
        m_destroyed = other.m_destroyed;
        m_scene = other.m_scene;
        type = other.type;

        m_components.clear();
        for (const auto& component : other.m_components)
        {
            auto clonedObj = component->Clone();
            if (clonedObj)
            {
                auto clonedComp = std::unique_ptr<Component>(dynamic_cast<Component*>(clonedObj.release()));
                if (clonedComp)
                {
                    clonedComp->m_owner = this;
                    m_components.push_back(std::move(clonedComp));
                }
            }
        }

        return *this;
    }

    void Actor::Initialize()
    {
        for (const auto& component : m_components)
        {
            component->Initialize();
        }
    }

    bool Actor::Read(const rapidjson::Value& value)
    {
        if (!value.IsObject()) return false;

        json::Read(value, "name", m_name);
        json::Read(value, "tag", m_tag);
        json::Read(value, "lifespan", m_lifespan);
        json::Read(value, "velocity", m_velocity);
        json::Read(value, "damping", m_damping);
        json::Read(value, "type", type);

        if (value.HasMember("transform") && value["transform"].IsObject())
        {
            const auto& transformValue = value["transform"];
            json::Read(transformValue, "position", m_transform.position);
            json::Read(transformValue, "rotation", m_transform.rotation);
            json::Read(transformValue, "scale", m_transform.scale);
        }

        if (m_transform.scale == 0.0f)
        {
            m_transform.scale = 1.0f;
        }

        std::string textureName;
        if (json::Read(value, "texture", textureName) || json::Read(value, "textureName", textureName))
        {
            m_texture = ResourceManager::Instance().Get<nu::Texture>(textureName, Engine::Get().GetRenderer());
        }

        if (value.HasMember("components") && value["components"].IsArray())
        {
            for (const auto& compValue : value["components"].GetArray())
            {
                std::string compType;
                if (json::Read(compValue, "type", compType))
                {
                    auto component = Factory::Instance().Create<Component>(compType);
                    if (component)
                    {
                        component->Read(compValue);
                        AddComponent(std::move(component));
                    }
                    else
                    {
                        std::cerr << "[Actor ERROR] Factory failed to instantiate component: " << compType << std::endl;
                    }
                }
            }
        }

        return true;
    }

    void Actor::Update(float dt)
    {
        if (m_lifespan > 0.0f)
        {
            m_lifespan -= dt;
            if (m_lifespan <= 0.0f)
            {
                m_destroyed = true;
            }
        }

        for (auto& component : m_components)
        {
            component->Update(dt);
        }

        m_transform.position += (m_velocity * dt);
        m_velocity *= (1.0f / (1.0f + m_damping * dt));

        if (m_tag != "PlayerBullet" && m_tag != "Wall")
        {
            m_transform.position.x = nu::Wrap(0.0f, 1920.0f, m_transform.position.x);
            m_transform.position.y = nu::Wrap(0.0f, 1024.0f, m_transform.position.y);
        }
    }

    void Actor::Draw(const Renderer& renderer) const
    {
        bool renderedByComponent = false;
        for (const auto& component : m_components)
        {
            auto renderComp = dynamic_cast<RenderComponent*>(component.get());
            if (renderComp)
            {
                renderComp->Draw(renderer);
                renderedByComponent = true;
            }
        }

        if (!renderedByComponent)
        {
            if (m_texture)
            {
                renderer.DrawTexture(m_texture.get(), m_transform);
            }
            else
            {
                renderer.SetColor(255, 255, 255);
                renderer.DrawModel(m_model, m_transform);
            }
        }
    }

    void Actor::AddComponent(std::unique_ptr<Component> component)
    {
        if (component)
        {
            component->m_owner = this;
            m_components.push_back(std::move(component));
        }
    }

    float Actor::GetRadious() const
    {
        for (const auto& component : m_components)
        {
            auto collider = dynamic_cast<CircleColliderComponent*>(component.get());
            if (collider)
            {
                return collider->GetRadius();
            }
        }

        if (m_texture)
        {
            Vector2 size = m_texture->GetSize();
            float maxDimension = (size.x > size.y) ? size.x : size.y;
            return maxDimension * m_transform.scale * 0.5f;
        }

        return m_model.GetRadious() * m_transform.scale * 0.5f;
    }
}