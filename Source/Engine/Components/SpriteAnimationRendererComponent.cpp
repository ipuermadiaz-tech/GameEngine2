#include "pch.h"
#include "SpriteAnimationRendererComponent.h"

namespace nu
{
    FACTORY_REGISTER(SpriteAnimationRendererComponent)

        std::unique_ptr<Object> SpriteAnimationRendererComponent::Clone() const
    {
        return std::make_unique<SpriteAnimationRendererComponent>(*this);
    }

    bool SpriteAnimationRendererComponent::Read(const rapidjson::Value& value)
    {
        std::string textureName;
        if (JSON_READ(value, textureName))
        {
            m_textureFrames = ResourceManager::Instance().Get<nu::TextureFrames>(textureName, Engine::Get().GetRenderer());
        }

        // Default m_fps to a reasonable value if JSON read fails
        m_fps = 1.0f;
        JSON_READ(value, m_fps);

        int columns = 1;
        int rows = 1;
        int count = 0;

        JSON_READ(value, columns);
        JSON_READ(value, rows);
        JSON_READ(value, count);

        if (m_textureFrames)
        {
            m_textureFrames->CreateFrames(columns, rows, count);
        }

        // Reset frame state on read
        m_frame = 0;
        m_frameTimer = 0.0f;

        return true;
    }

    void SpriteAnimationRendererComponent::Update(float dt)
    {
        if (!m_textureFrames || m_textureFrames->GetFrameCount() == 0) return;
        if (m_fps <= 0.0f) return;

        m_frameTimer += dt;
        float frameDuration = 1.0f / m_fps;

        if (m_frameTimer >= frameDuration)
        {
            m_frameTimer = 0.0f; // Reset timer directly
            m_frame = (m_frame + 1) % m_textureFrames->GetFrameCount();
        }
    }

    void SpriteAnimationRendererComponent::Draw(const Renderer& renderer) const
    {
        if (m_textureFrames && m_owner)
        {
            SDL_FRect srcRect = m_textureFrames->GetSourceRect(m_frame);
            renderer.DrawTexture(m_textureFrames.get(), srcRect, m_owner->m_transform);
        }
    }
}