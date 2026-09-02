#pragma once
#include <box2d/box2d.h>
#include <memory>

namespace nu
{
	class Physics
	{
	public:
		Physics() = default;

		bool Initialize();
		void Shutdown();

		void Update(float dt);
		b2WorldId GetWorldId() const { return m_worldId; }
	private:
		b2WorldId m_worldId;
	};
}