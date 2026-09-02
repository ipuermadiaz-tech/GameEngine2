#include "pch.h"
#include "Tilemap.h"
#include "Texture.h"
#include "Renderer.h"
#include "Resources/ResourceManager.h"
#include "Serialization/Json.h"
#include "Core/StringUtils.h"
#include <iostream>
#include <algorithm>

namespace nu
{
	bool Tilemap::Load(const std::string& filename, Renderer& renderer)
	{
		// load json 
		json::document_t document;
		if (!json::Load(filename, document))
		{
			std::cerr << "Could not load Tilemap " << filename << std::endl;
			return false;
		}

		// read tilemap data 
		JSON_READ_NAME_REQ(document, "tilewidth", m_tileWidth);
		JSON_READ_NAME_REQ(document, "tileheight", m_tileHeight);

		// read tilemap layers 
		if (JSON_HAS_NAME(document, "layers"))
		{
			for (auto& layerValue : JSON_GET_NAME(document, "layers").GetArray())
			{
				Layer layer;

				JSON_READ_NAME_REQ(layerValue, "height", layer.height);
				JSON_READ_NAME_REQ(layerValue, "width", layer.width);
				JSON_READ_NAME_REQ(layerValue, "data", layer.data);

				// load texture from properties 
				if (JSON_HAS_NAME(layerValue, "properties"))
				{
					for (auto& propertyValue : JSON_GET_NAME(layerValue, "properties").GetArray())
					{
						std::string name;
						JSON_READ_NAME(propertyValue, "name", name);

						if (EqualsIgnoreCase(name, "texture_name"))
						{
							std::string value;
							JSON_READ_NAME(propertyValue, "value", value);

							// Convert Windows backslashes to standard forward slashes
							std::replace(value.begin(), value.end(), '\\', '/');

							layer.texture = Resources().Get<Texture>(value, renderer);
							if (!layer.texture)
							{
								std::cerr << "Could not read tilemap layer texture " << value << std::endl;
							}
						}
						else if (EqualsIgnoreCase(name, "has_collision") || EqualsIgnoreCase(name, "hascollision"))
						{
							JSON_READ_NAME(propertyValue, "value", layer.hasCollision);
						}
					}
				}

				m_layers.push_back(layer);
			}
		}

		return true;
	}

	// get texture source rectangle from the tile id 
	Rect Tilemap::GetTileRect(const Layer& layer, int tileId)
	{
		if (tileId == 0 || !layer.texture) return Rect();

		Vector2 textureSize = layer.texture->GetSize();
		int tilesPerRow = (int)(textureSize.x / m_tileWidth);
		if (tilesPerRow <= 0) return Rect();

		// Convert 1-indexed GID to 0-indexed texture position
		int localTileId = tileId - 1;

		int column = localTileId % tilesPerRow;
		int row = localTileId / tilesPerRow;

		return Rect
		{
			(float)(column * m_tileWidth),
			(float)(row * m_tileHeight),
			(float)(m_tileWidth),
			(float)(m_tileHeight)
		};
	}

	// get the tile position from the tile index (index into tile data) 
	Vector2 Tilemap::GetTilePosition(const Layer& layer, int tileIndex)
	{
		if (layer.width <= 0) return Vector2{ 0, 0 };

		int column = tileIndex % layer.width;
		int row = tileIndex / layer.width;

		return Vector2{ (float)(column * m_tileWidth), (float)(row * m_tileHeight) };
	}
}