#include "pch.h"
#include "CircleColliderComponent.h"
#include "Core/Factory.h"
#include "Serialization/Json.h"

namespace nu
{
    FACTORY_REGISTER(CircleColliderComponent)

        CircleColliderComponent::CircleColliderComponent(const CircleColliderComponent& other)
        : Component(other)
        , radius{ other.radius }
    {
    }

    bool CircleColliderComponent::Read(const rapidjson::Value& value)
    {
        JSON_READ(value, radius);
        return true;
    }

    float CircleColliderComponent::GetRadius() const
    {
        if (m_owner)
        {
            return radius * m_owner->m_transform.scale;
        }
        return radius;
    }
}