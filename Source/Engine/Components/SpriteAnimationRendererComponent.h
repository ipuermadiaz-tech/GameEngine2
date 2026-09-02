#pragma once
#include "RenderComponent.h"
#include "Renderer/TextureFrames.h"
#include "Resources/ResourceManager.h"
#include "Engine.h"
#include "Framework/Actor.h"
#include "Core/Factory.h"

namespace nu
{
    class SpriteAnimationRendererComponent : public RenderComponent
    {
    public:
        SpriteAnimationRendererComponent() = default;

        std::unique_ptr<Object> Clone() const override;

        bool Read(const rapidjson::Value& value) override;
        void Update(float dt) override;
        void Draw(const Renderer& renderer) const override;

    public:
        std::shared_ptr<nu::TextureFrames> m_textureFrames{ nullptr };

        int m_frame = 0;
        float m_fps = 12.0f;
        float m_frameTimer = 0.0f;
    };
}