#pragma once
#include "RenderComponent.h"
#include "Model.h"
#include "Actor.h"
#include "Factory.h"
#include "ResourceManager.h"
#include "Engine.h"

namespace nu
{
    class MeshRendererComponent : public RenderComponent
    {
    public:
        MeshRendererComponent() = default;

        std::unique_ptr<Object> Clone() const override
        {
            return std::make_unique<MeshRendererComponent>(*this);
        }

        bool Read(const rapidjson::Value& value) override
        {
            std::string modelName;
            if (JSON_READ(value, modelName))
            {
                m_model = ResourceManager::Instance().Get<Model>(modelName);
            }
            return true;
        }

        void Draw(const Renderer& renderer) const override
        {
            if (m_owner)
            {
                renderer.SetColor(255, 255, 255);
                renderer.DrawModel(m_model, m_owner->m_transform);
            }
        }

        Model m_model;
    };

    static RegisterActor<MeshRendererComponent> register_mesh_renderer("MeshRendererComponent");
}