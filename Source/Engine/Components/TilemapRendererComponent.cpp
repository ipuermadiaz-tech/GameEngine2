#include "pch.h"
#include "TilemapRendererComponent.h"
#include "Engine.h"
#include "Framework/Actor.h"
#include "Renderer/Tilemap.h"
#include "Physics/Physics.h"
#include "Physics/PhysicsBody.h"
#include "Renderer/Renderer.h"
#include "Math/Rect.h"
#include "Serialization/Json.h"

namespace nu
{
	FACTORY_REGISTER(TilemapRendererComponent)

		TilemapRendererComponent::TilemapRendererComponent(const TilemapRendererComponent& other)
	{
		m_tilemapName = other.m_tilemapName;
		m_tilemap = other.m_tilemap; // Fix: Retain tilemap resource pointer when cloned
	}

	void TilemapRendererComponent::Initialize()
	{
		// Load the tilemap resource if not already set
		if (!m_tilemap)
		{
			m_tilemap = Resources().Get<Tilemap>(m_tilemapName, Engine::Get().GetRenderer());
		}

		if (!m_tilemap)
		{
			std::cerr << "Could not load tilemap: " << m_tilemapName << std::endl;
			return;
		}

		// Clear previous physics bodies if re-initialized
		m_physicsBodies.clear();

		// Define physics body parameters
		PhysicsBody::PhysicsBodyDef bodyDef;
		bodyDef.isDynamic = false;   // Static body
		bodyDef.actor = m_owner;     // Collision owner

		// Iterate through layers for collision
		for (const auto& layer : m_tilemap->GetLayers())
		{
			if (!layer.hasCollision) continue;

			for (size_t i = 0; i < layer.data.size(); i++)
			{
				int tileId = layer.data[i];
				if (tileId == 0) continue;

				Rect sourceRect = m_tilemap->GetTileRect(layer, tileId);

				Transform transform = m_owner->GetTransform();
				transform.position = m_owner->GetTransform().position + (m_tilemap->GetTilePosition(layer, (int)i) * transform.scale);

				Vector2 size{ (float)sourceRect.w, (float)sourceRect.h };

				auto physicsBody = std::make_unique<PhysicsBody>(transform, size, bodyDef, Engine::Get().GetPhysics());
				m_physicsBodies.push_back(std::move(physicsBody));
			}
		}
	}

	void TilemapRendererComponent::Draw(const Renderer& renderer) const
	{
		if (!m_tilemap) return;

		for (const auto& layer : m_tilemap->GetLayers())
		{
			if (!layer.texture) continue;

			for (size_t i = 0; i < layer.data.size(); i++)
			{
				int tileId = layer.data[i];
				if (tileId == 0) continue;

				Rect sourceRect = m_tilemap->GetTileRect(layer, tileId);

				Transform transform = m_owner->GetTransform();
				transform.position = transform.position + (m_tilemap->GetTilePosition(layer, (int)i) * transform.scale);

				SDL_FRect fSourceRect{ (float)sourceRect.x, (float)sourceRect.y, (float)sourceRect.w, (float)sourceRect.h };

				renderer.DrawTexture(layer.texture.get(), fSourceRect, transform);
			}
		}
	}

	bool TilemapRendererComponent::Read(const rapidjson::Value& value)
	{
		RenderComponent::Read(value);

		JSON_READ_NAME(value, "tilemap_name", m_tilemapName);
		//JSON_READ_NAME(value, "flight", m_tilemapName);

		return true;
	}
}