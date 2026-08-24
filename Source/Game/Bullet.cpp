#include "Bullet.h"
#include "Math_Util.h"
#include "Factory.h"
#include "Json.h"

static nu::RegisterActor<Bullet> regBullet("Bullet");

bool Bullet::Read(const rapidjson::Value& value) {
    if (!nu::Actor::Read(value)) return false;

    JSON_READ(value, m_speed);
    return true;
}

void Bullet::Update(float dt)
{
    nu::Vector2 forward{ 0.0f, -1.0f };
    nu::Vector2 velocity = forward.Rotate(m_transform.rotation * nu::DegToRad) * m_speed;

    SetVelocity(velocity);

    // Despawn when out of window bounds (with safety buffer)
    if (m_transform.position.x > 2000.0f || m_transform.position.x < -100.0f ||
        m_transform.position.y > 1200.0f || m_transform.position.y < -100.0f)
    {
        SetDestroyed();
    }

    Actor::Update(dt);
}

std::unique_ptr<nu::Object> Bullet::Clone() const
{
    return std::make_unique<Bullet>(*this);
}