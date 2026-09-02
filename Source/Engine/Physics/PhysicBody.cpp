#include "pch.h"
#include "PhysicsBody.h"
#include "Physics.h"

namespace nu
{
	PhysicsBody::PhysicsBody(const Transform& transform, const Vector2& size, const PhysicsBodyDef& def, Physics& physics)
	{
		Create(transform, size, def, physics);
	}

	PhysicsBody::~PhysicsBody()
	{
		Destroy();
	}

	PhysicsBody::PhysicsBody(PhysicsBody&& other) noexcept
	{
		m_bodyId = other.m_bodyId;
		other.m_bodyId = b2_nullBodyId;
	}

	PhysicsBody& PhysicsBody::operator=(PhysicsBody&& other) noexcept
	{
		if (this != &other)
		{
			Destroy();
			m_bodyId = other.m_bodyId;
			other.m_bodyId = b2_nullBodyId;
		}
		return *this;
	}

	void PhysicsBody::Create(const Transform& transform, const Vector2& size, const PhysicsBodyDef& def, Physics& physics)
	{
		Destroy();

		b2BodyDef bodyDef = b2DefaultBodyDef();
		bodyDef.type = def.isDynamic ? b2_dynamicBody : b2_staticBody;
		bodyDef.position = { transform.position.x, transform.position.y };
		bodyDef.rotation = b2MakeRot(transform.rotation * (3.14159265f / 180.0f));
		bodyDef.userData = def.actor;

		m_bodyId = b2CreateBody(physics.GetWorldId(), &bodyDef);

		b2Polygon box = b2MakeBox((size.x * 0.5f) * transform.scale, (size.y * 0.5f) * transform.scale);

		b2ShapeDef shapeDef = b2DefaultShapeDef();
		shapeDef.density = def.density;
		shapeDef.material.friction = def.friction;      
		shapeDef.material.restitution = def.restitution;
		shapeDef.isSensor = def.isSensor;

		b2CreatePolygonShape(m_bodyId, &shapeDef, &box);
	}

	void PhysicsBody::Destroy()
	{
		if (b2Body_IsValid(m_bodyId))
		{
			b2DestroyBody(m_bodyId);
			m_bodyId = b2_nullBodyId;
		}
	}
}