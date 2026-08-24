#pragma once
#include "Component.h"

namespace nu
{
    class Renderer;

    class RenderComponent : public Component
    {
    public:
        virtual void Draw(const Renderer& renderer) const = 0;
    };
}
