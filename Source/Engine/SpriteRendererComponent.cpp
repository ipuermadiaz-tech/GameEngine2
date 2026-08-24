#include "SpriteRendererComponent.h"

namespace nu {
    FACTORY_REGISTER(SpriteRendererComponent);
    std::unique_ptr<Object> SpriteRendererComponent::Clone() const
    {
        return std::make_unique<SpriteRendererComponent>(*this);
    }

    bool SpriteRendererComponent:: Read(const rapidjson::Value& value)
    {
        std::string textureName;
        if (JSON_READ(value, textureName))
        {
            m_texture = ResourceManager::Instance().Get<nu::Texture>(textureName, Engine::Get().GetRenderer());
        }
        return true;
    }

    void SpriteRendererComponent::Draw(const Renderer& renderer) const
    {
        if (m_texture && m_owner)
        {
            renderer.DrawTexture(m_texture.get(), m_owner->m_transform);
        }
    }
}