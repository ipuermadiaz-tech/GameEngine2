#include "Assets.h"
//#include "Renderer/Mesh.h"
#include "Math/Vector3.h"
#include "Renderer/Model.h"

using namespace nu;

namespace assets
{
	Mesh bulletMesh{

	{

		Vector2{ -1, -1 },

		Vector2{ 1, 0 },

		Vector2{ -1, 1 },

		Vector2{ -1, -1 },

	},

	Color{ 1.0f, 1.0f, 1.0f }

	};

	Model bulletModel{ std::vector<Mesh>{ bulletMesh } };

}