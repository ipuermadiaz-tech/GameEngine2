#pragma once
#include "Actor.h"

struct BulletDesc : public nu::ActorDesc
{
	float speed = 800.0f;
};

class Bullet : public nu::Actor
{
public:
	Bullet() = default;
	Bullet(const BulletDesc& BulletDesc) :
		Actor(BulletDesc),
		m_speed{ BulletDesc.speed }
	{
	}
	Bullet(float speed, const nu::Transform& transform) :
		Actor{ transform },
		m_speed{ speed }
	{
	}
	Bullet(float speed, const nu::Transform& transform, const nu::Model& model) :
		Actor{ transform, model },
		m_speed{ speed }
	{
	}

	void Update(float dt) override;

private:
	float m_speed = 800.0f;
	int nowrap = 1;
};
