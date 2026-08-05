#include "Text.h"
//#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include "Vector3.h"
#include "Renderer.h"
namespace nu {

	Text::~Text() {
		if (m_texture != nullptr) {
			SDL_DestroyTexture(m_texture);
		}
	}

    bool Text::Create(const Renderer& renderer, const std::string& text, const Color& color) {
        // 1. FREE PREVIOUS TEXTURE TO PREVENT MEMORY LEAKS
        if (m_texture != nullptr) {
            SDL_DestroyTexture(m_texture);
            m_texture = nullptr;
        }

        SDL_Color c{ (uint8_t)(color.x * 255), (uint8_t)(color.y * 255), (uint8_t)(color.z * 255), 255 };

        // 2. SDL3 TTF_RenderText_Solid (pass 0 for length to read full null-terminated string)
        SDL_Surface* surface = TTF_RenderText_Solid(m_font->m_ttfFont, text.c_str(), 0, c);
        if (surface == nullptr) {
            std::cerr << "Could not create surface.\n";
            return false;
        }

        m_texture = SDL_CreateTextureFromSurface(renderer.m_renderer, surface);
        if (m_texture == nullptr) {
            SDL_DestroySurface(surface);
            std::cerr << "Could not create texture: " << SDL_GetError() << std::endl;
            return false;
        }

        SDL_DestroySurface(surface);
        return true;
    }

	void Text::Draw(const Renderer& renderer, float x, float y) {
		// get the texture width and height
		float width, height;
		bool success = SDL_GetTextureSize(m_texture, &width, &height);
		assert(success);

		// set the texture into the renderer at rect 
		SDL_FRect rect{ x, y, width, height };
		success = SDL_RenderTexture(renderer.m_renderer, m_texture, NULL, &rect);
		assert(success);
	}

}