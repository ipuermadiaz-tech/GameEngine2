#include "Mesh.h"

namespace nu
{
	float Mesh::GetRadious() const
	{
		float radius = 0.0f;
		for (const auto& point : m_points) {
			float lenght = point.Length();
			if (lenght > radius) {
				radius = lenght;
			}
		
		}
		return radius;
	}

}