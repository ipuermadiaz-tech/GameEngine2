#pragma once
#include "Component.h"
#include "Actor.h"
#include "Factory.h"

namespace nu
{
    class CircleColliderComponent : public Component
    {
    public:
        CircleColliderComponent() = default;

        std::unique_ptr<Object> Clone() const override
        {
            return std::make_unique<CircleColliderComponent>(*this);
        }

        bool Read(const rapidjson::Value& value) override
        {
            JSON_READ(value, radius);
            return true;
        }

        float GetRadius() const
        {
            if (m_owner)
            {
                return radius * m_owner->m_transform.scale;
            }
            return radius;
        }

        float radius{ 0.0f };
    };

    static RegisterActor<CircleColliderComponent> register_circle_collider("CircleColliderComponent");
}