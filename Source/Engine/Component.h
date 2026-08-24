#pragma once
#include "Object.h"
#include <memory>

namespace nu {
    class Actor;
    class Renderer;

    class Component : public Object {
    public:
        Component() = default;
        virtual ~Component() = default;

        virtual std::unique_ptr<Object> Clone() const override = 0;

        virtual void Initialize() {}
        virtual void Update(float dt) {}
        virtual void Draw(Renderer& renderer) {}

        Actor* m_owner{ nullptr };
    };
}