#pragma once
#include "Texture.h"
#include <vector>
#include <SDL3/SDL.h>

namespace nu
{
    class TextureFrames : public Texture
    {
    public:
        TextureFrames() = default;
        ~TextureFrames() = default;

        bool Load(const std::string& filename, Renderer& renderer);

        // Pre-calculates source rectangles based on columns, rows, and frame count
        bool CreateFrames(int columns, int rows, int count);

        const SDL_FRect& GetSourceRect(int frameIndex) const;
        int GetFrameCount() const { return static_cast<int>(m_frames.size()); }

    private:
        std::vector<SDL_FRect> m_frames;
    };
}