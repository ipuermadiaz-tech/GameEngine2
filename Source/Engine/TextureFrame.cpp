#include "pch.h"
#include "TextureFrames.h"

namespace nu
{
    bool TextureFrames::Load(const std::string& filename, Renderer& renderer)
    {
        return Texture::Load(filename, renderer);
    }

    bool TextureFrames::CreateFrames(int columns, int rows, int count)
    {
        Vector2 size = GetSize();
        if (size.x == 0 || size.y == 0 || columns <= 0 || rows <= 0) return false;

        float frameWidth = size.x / static_cast<float>(columns);
        float frameHeight = size.y / static_cast<float>(rows);

        m_frames.clear();
        for (int i = 0; i < count; i++)
        {
            int col = i % columns;
            int row = i / columns;

            SDL_FRect rect;
            rect.x = col * frameWidth;
            rect.y = row * frameHeight;
            rect.w = frameWidth;
            rect.h = frameHeight;

            m_frames.push_back(rect);
        }

        return true;
    }

    const SDL_FRect& TextureFrames::GetSourceRect(int frameIndex) const
    {
        static SDL_FRect emptyRect{ 0.0f, 0.0f, 0.0f, 0.0f };
        if (frameIndex >= 0 && frameIndex < static_cast<int>(m_frames.size()))
        {
            return m_frames[frameIndex];
        }
        return emptyRect;
    }
}