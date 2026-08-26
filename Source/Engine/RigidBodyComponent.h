#pragma once
#include "Component.h"
#include "Vector2.h"

namespace nu
{
    class RigidBodyComponent : public Component
    {
    public:
        RigidBodyComponent() = default;
        RigidBodyComponent(const RigidBodyComponent& other);

        bool Read(const rapidjson::Value& value) override;
        void Update(float dt) override;

        void SetVelocity(const Vector2& velocity) { m_velocity = velocity; }
        Vector2 GetVelocity() const { return m_velocity; }

        CLASS_PROTOTYPE(RigidBodyComponent)

            Vector2 velocity{ 0.0f, 0.0f };
        float damping{ 0.0f };

    private:
        Vector2 m_velocity{ 0.0f, 0.0f };
    };
}