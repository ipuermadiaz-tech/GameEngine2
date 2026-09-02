#include "pch.h"
#include "Renderer.h"
#include "Math/Transformer.h"
#include "Mesh.h"
#include "Model.h"
#include "Math/Math_Util.h"
#include "Texture.h"

bool nu::Renderer::Initialize(const char* name, int width, int height) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    m_height = height;
    m_width = width;

    if (!TTF_Init()) {
        std::cerr << "TTF_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    m_window = SDL_CreateWindow(name, width, height, 0);
    if (m_window == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    m_renderer = SDL_CreateRenderer(m_window, NULL);
    if (m_renderer == nullptr) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }

    SDL_SetRenderVSync(m_renderer, 1);

    // Enable Alpha Blending mode across the renderer pipeline
    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

    return true;
}

void nu::Renderer::Shutdown()
{
    TTF_Quit();
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void nu::Renderer::SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const {
    SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
}

void nu::Renderer::Clear() const
{
    SDL_RenderClear(m_renderer);
}

void nu::Renderer::DrawPoint(float x, float y) const
{
    SDL_RenderPoint(m_renderer, x, y);
}

void nu::Renderer::DrawLine(float x1, float y1, float x2, float y2) const
{
    SDL_RenderLine(m_renderer, x1, y1, x2, y2);
}

void nu::Renderer::Present()
{
    SDL_RenderPresent(m_renderer);
}

void nu::Renderer::DrawFillRect(float x, float y, float w, float h) const
{
    SDL_FRect rect{ x, y, w, h };
    SDL_RenderFillRect(m_renderer, &rect);
}

void nu::Renderer::DrawRect(float x, float y, float w, float h) const
{
    SDL_FRect rect{ x, y, w, h };
    SDL_RenderRect(m_renderer, &rect);
}

void nu::Renderer::DrawModel(const Model& model, const Transform& transform) const
{
    for (const auto& mesh : model.GetMeshes()) {
        SetColor(255, 255, 255);

        auto& points = mesh.GetPoints();
        for (size_t i = 0; i + 1 < points.size(); i++)
        {
            Vector2 v1 = points[i];
            Vector2 v2 = points[i + 1];

            v1 *= transform.scale;
            v2 *= transform.scale;

            v1 = v1.Rotate(transform.rotation * DegToRad);
            v2 = v2.Rotate(transform.rotation * DegToRad);

            v1 += transform.position;
            v2 += transform.position;

            DrawLine(v1.x, v1.y, v2.x, v2.y);
        }
    }
}

void nu::Renderer::DrawTexture(Texture* texture, float x, float y) const
{
    if (!texture || !texture->m_texture) return;

    // Enable texture transparency
    SDL_SetTextureBlendMode(texture->m_texture, SDL_BLENDMODE_BLEND);

    Vector2 size = texture->GetSize();
    SDL_FRect destRect{ x, y, size.x, size.y };

    SDL_RenderTexture(m_renderer, texture->m_texture, NULL, &destRect);
}

void nu::Renderer::DrawTexture(Texture* texture, const Transform& transform) const
{
    if (!texture || !texture->m_texture) return;

    // Enable texture transparency
    SDL_SetTextureBlendMode(texture->m_texture, SDL_BLENDMODE_BLEND);

    Vector2 size = texture->GetSize();

    float width = size.x * transform.scale;
    float height = size.y * transform.scale;

    SDL_FRect destRect;
    destRect.w = width;
    destRect.h = height;
    destRect.x = transform.position.x - (width * 0.5f);
    destRect.y = transform.position.y - (height * 0.5f);

    SDL_FPoint center{ width * 0.5f, height * 0.5f };
    double angleInDegrees = transform.rotation;

    SDL_RenderTextureRotated(
        m_renderer,
        texture->m_texture,
        NULL,
        &destRect,
        angleInDegrees,
        &center,
        SDL_FLIP_NONE
    );
}
void nu::Renderer::DrawTexture(Texture* texture, const SDL_FRect& sourceRect, const Transform& transform) const
{
    if (!texture || !texture->m_texture) return;

    SDL_SetTextureBlendMode(texture->m_texture, SDL_BLENDMODE_BLEND);

    // Scale destination size by frame dimensions instead of full texture size
    float width = sourceRect.w * transform.scale;
    float height = sourceRect.h * transform.scale;

    SDL_FRect destRect;
    destRect.w = width;
    destRect.h = height;
    destRect.x = transform.position.x - (width * 0.5f);
    destRect.y = transform.position.y - (height * 0.5f);

    SDL_FPoint center{ width * 0.5f, height * 0.5f };
    double angleInDegrees = transform.rotation;

    SDL_RenderTextureRotated(
        m_renderer,
        texture->m_texture,
        &sourceRect, // Pass source sub-rectangle here instead of NULL
        &destRect,
        angleInDegrees,
        &center,
        SDL_FLIP_NONE
    );
}