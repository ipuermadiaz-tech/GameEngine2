#pragma once
#include "RenderComponent.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "Engine.h"
#include "Actor.h"
#include "Factory.h"

namespace nu
{
    class SpriteRendererComponent : public RenderComponent
    {
    public:
        SpriteRendererComponent() = default;

        std::unique_ptr<Object> Clone() const override;

        bool Read(const rapidjson::Value& value) override;
        void Draw(const Renderer& renderer) const override;

        std::shared_ptr<nu::Texture> m_texture{ nullptr };
    };

}