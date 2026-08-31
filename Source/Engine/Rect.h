#pragma once

namespace nu
{
	struct Rect
	{
		float x{ 0.0f };
		float y{ 0.0f };
		float w{ 0.0f };
		float h{ 0.0f };

		Rect() = default;
		Rect(float _x, float _y, float _w, float _h)
			: x(_x), y(_y), w(_w), h(_h) {
		}
	};
}