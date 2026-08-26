#pragma once
#include "Object.h"
#include <memory>

namespace nu {
    class Actor;
    class Renderer;

    class Component : public Object {
    public:
        Component() = default;
        Component(const Component& other) = default;
        virtual ~Component() = default;

        virtual void Initialize() {}
        virtual void Update(float dt) {}
        virtual void Draw(Renderer& renderer) {}

        Actor* m_owner{ nullptr };
    };
}