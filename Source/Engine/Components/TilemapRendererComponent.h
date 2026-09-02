#pragma once
#include "RenderComponent.h"
#include "Resources/Resource.h"
namespace nu

{

	class TilemapRendererComponent : public RenderComponent

	{

	public:

		TilemapRendererComponent() = default;

		TilemapRendererComponent(const TilemapRendererComponent& other);



		CLASS_PROTOTYPE(TilemapRendererComponent)



			void Initialize() override;

		void Draw(const Renderer& renderer) const override;



		bool Read(const rapidjson::Value& value) override;



	private:

		std::string m_tilemapName;

		res_t<class Tilemap> m_tilemap;



		std::vector<std::unique_ptr<class PhysicsBody>> m_physicsBodies;

	};
}