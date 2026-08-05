#pragma once
#include "Mesh.h"

namespace nu
{
	class Model
	{
	public:
		Model() = default;
		Model(const std::vector<Mesh>& meshes) : m_meshes{meshes}{
			CalculateRadious();
		}

		void AddMesh(const Mesh& mesh) { 
			
			m_meshes.push_back(mesh);
			CalculateRadious();
		}
		void SetMeshes(const std::vector<Mesh> meshes) { 
			
			m_meshes = meshes; 
			CalculateRadious();
		};
		const std::vector<Mesh>& GetMeshes() const { return m_meshes; }
		float GetRadious() const { return m_radious; };
		void CalculateRadious();
	private:
		float m_radious = 0.0f;
		std::vector<Mesh> m_meshes;

	};

}