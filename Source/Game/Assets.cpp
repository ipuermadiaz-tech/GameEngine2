#include "Assets.h"
#include "Mesh.h"
#include "Vector3.h"
#include "Model.h"

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