#include "RigidBodyComponent.h"
#include "Actor.h"
#include "Factory.h"
#include "Json.h"

namespace nu
{
    FACTORY_REGISTER(RigidBodyComponent)

        RigidBodyComponent::RigidBodyComponent(const RigidBodyComponent& other)
        : Component(other)
        , velocity{ other.velocity }
        , damping{ other.damping }
        , m_velocity{ other.m_velocity }
    {
    }

    bool RigidBodyComponent::Read(const rapidjson::Value& value)
    {
        JSON_READ(value, velocity);
        JSON_READ(value, damping);
        return true;
    }

    void RigidBodyComponent::Update(float dt)
    {
        if (!m_owner) return;

        m_owner->m_transform.position += (velocity * dt);
        velocity *= (1.0f / (1.0f + damping * dt));
    }
}