#include "pch.h"
#include "Physics.h"

namespace nu
{
	bool Physics::Initialize()
	{
		b2WorldDef worldDef = b2DefaultWorldDef();
		worldDef.gravity = b2Vec2{ 0.0f, -10.0f };
		m_worldId = b2CreateWorld(&worldDef);

		return true;
	}

	void Physics::Shutdown()
	{
		b2DestroyWorld(m_worldId);
	}

	void Physics::Update(float dt)
	{
		b2World_Step(m_worldId, 1.0f / 60.0f, 4);
	}
}