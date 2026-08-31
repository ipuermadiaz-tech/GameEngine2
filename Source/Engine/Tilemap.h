#pragma once
#include "Resource.h"
#include "Rect.h"
#include "Vector2.h"
#include <string>
#include <vector>

namespace nu
{
	class Texture;
	class Renderer;

	class Tilemap : public Resource
	{
	public:
		struct Layer
		{
			int width{ 0 };
			int height{ 0 };
			bool hasCollision{ false };
			std::vector<int> data;
			res_t<Texture> texture;
		};

	public:
		Tilemap() = default;

		bool Load(const std::string& filename, Renderer& renderer);

		const std::vector<Layer>& GetLayers() const { return m_layers; }
		Rect GetTileRect(const Layer& layer, int tileId);
		Vector2 GetTilePosition(const Layer& layer, int tileIndex);

	private:
		int m_tileWidth{ 0 };
		int m_tileHeight{ 0 };

		std::vector<Layer> m_layers;
	};
}