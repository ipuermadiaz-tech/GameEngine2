#pragma once
#include "RenderComponent.h"
#include "Renderer/Texture.h"
#include "Resources/ResourceManager.h"
#include "Engine.h"
#include "Framework/Actor.h"
#include "Core/Factory.h"
#include "Math/Rect.h"
namespace nu
{
    class SpriteRendererComponent : public RenderComponent
    {
    public:
        SpriteRendererComponent() = default;

        void Initialize() override;

        std::unique_ptr<Object> Clone() const override;

        bool Read(const rapidjson::Value& value) override;
        void Draw(const Renderer& renderer) const override;

        const Vector2& GetSize() { return m_size; };
        bool GetFlight() const { return m_flight; };
        bool SetFlight(bool flight = true) { m_flight = flight; }

        std::shared_ptr<nu::Texture> m_texture{ nullptr };

    protected:
        std::string m_stringhere;
        Rect m_sourceRect;
        Vector2 m_size{ 0.0f,0.0f };
        bool m_flight{ false };

    };

}