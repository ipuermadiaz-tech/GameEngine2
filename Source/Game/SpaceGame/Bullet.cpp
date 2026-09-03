#include "Bullet.h"
#include "Math/Math_Util.h"
#include "Core/Factory.h"
#include "Serialization/Json.h"
#include "Components/RigidBodyComponent.h"

static nu::RegisterActor<Bullet> regBullet("Bullet");

bool Bullet::Read(const rapidjson::Value& value)
{
    if (!nu::Actor::Read(value)) return false;

    JSON_READ(value, m_speed);
    return true;
}

void Bullet::Update(float dt)
{
    // Get the Rigidbody
    auto rigidBody = GetComponent<nu::RigidBodyComponent>();

    if (rigidBody)
    {
        nu::Vector2 forward{ 0.0f, -1.0f };

        nu::Vector2 velocity =
            forward.Rotate(m_transform.rotation * nu::DegToRad) * m_speed;

        // Set the Rigidbody's velocity instead of Actor::m_velocity
        rigidBody->velocity = velocity;
    }

    // Despawn when out of window bounds
    if (m_transform.position.x > 2000.0f ||
        m_transform.position.x < -100.0f ||
        m_transform.position.y > 1200.0f ||
        m_transform.position.y < -100.0f)
    {
        SetDestroyed();
    }

    Actor::Update(dt);
}