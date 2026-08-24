#pragma once
#include "Component.h"
#include "Actor.h"
#include "Vector2.h"
#include "Factory.h"

namespace nu
{
    class RigidBodyComponent : public Component
    {
    public:
        RigidBodyComponent() = default;

        std::unique_ptr<Object> Clone() const override {
            return std::make_unique<RigidBodyComponent>(*this);
        }

        bool Read(const rapidjson::Value& value) override
        {
            JSON_READ(value, velocity);
            JSON_READ(value, damping);
            return true;
        }

        void SetVelocity(const Vector2& velocity) { m_velocity = velocity; }

        void Update(float dt) override
        {
            if (!m_owner) return;

            m_owner->m_transform.position += (velocity * dt);
            velocity *= (1.0f / (1.0f + damping * dt));
        }

        Vector2 velocity{ 0.0f, 0.0f };
        float damping{ 0.0f };

    private:
        Vector2 m_velocity{ 0.0f, 0.0f };
    };

    static RegisterActor<RigidBodyComponent> register_rigidbody("RigidBodyComponent");
}