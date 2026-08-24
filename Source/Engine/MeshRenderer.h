#pragma once
#include "RenderComponent.h"
#include "Model.h"
#include "Actor.h"
#include "Factory.h"
#include "ResourceManager.h"

namespace nu
{
    class MeshRendererComponent : public RenderComponent
    {
    public:
        MeshRendererComponent() = default;

        std::unique_ptr<Component> Clone() const override
        {
            return std::make_unique<MeshRendererComponent>(*this);
        }

        bool Read(const rapidjson::Value& value) override
        {
            std::string modelName;
            if (JSON_READ(value, modelName))
            {
                m_model = Resources().Get<nu::Model>(modelName);
            }
            return true;
        }

        void Draw(const Renderer& renderer) const override
        {
            if (m_model && m_owner)
            {
                m_model->Draw(renderer, m_owner->m_transform);
            }
        }

        res_t<nu::Model> m_model{ nullptr };
    };

    static RegisterActor<MeshRendererComponent> register_mesh_renderer("MeshRendererComponent");
}