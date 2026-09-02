#pragma once
#include "Math/Vector2.h"
#include "Math/Transformer.h"
#include <box2d/box2d.h>

namespace nu
{
	class Actor;
	class Physics;

	class PhysicsBody
	{
	public:
		struct PhysicsBodyDef
		{
			bool isDynamic{ false };
			bool isSensor{ false };
			float density{ 1.0f };
			float friction{ 0.3f };
			float restitution{ 0.0f };
			Actor* actor{ nullptr };
		};

	public:
		PhysicsBody() = default;
		PhysicsBody(const Transform& transform, const Vector2& size, const PhysicsBodyDef& def, Physics& physics);
		~PhysicsBody();

		PhysicsBody(const PhysicsBody&) = delete;
		PhysicsBody& operator=(const PhysicsBody&) = delete;

		PhysicsBody(PhysicsBody&& other) noexcept;
		PhysicsBody& operator=(PhysicsBody&& other) noexcept;

		void Create(const Transform& transform, const Vector2& size, const PhysicsBodyDef& def, Physics& physics);
		void Destroy();

		b2BodyId GetBodyId() const { return m_bodyId; }

	private:
		b2BodyId m_bodyId{ b2_nullBodyId };
	};
}