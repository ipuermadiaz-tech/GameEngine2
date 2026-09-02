#pragma once
#include "Component.h"
#include "Framework/Actor.h"

namespace nu
{
    class CircleColliderComponent : public Component
    {
    public:
        CircleColliderComponent() = default;
        CircleColliderComponent(const CircleColliderComponent& other);

        bool Read(const rapidjson::Value& value) override;
        float GetRadius() const;

        CLASS_PROTOTYPE(CircleColliderComponent)

            float radius{ 0.0f };
    };
}