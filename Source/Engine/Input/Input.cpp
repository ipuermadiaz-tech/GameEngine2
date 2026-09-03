#include "pch.h"
#include "Input.h"
#include <SDL3/SDL.h>

namespace nu
{
	bool Input::Initialize()
	{
		int numkeys = 0;
		// Change uint8_t* -> bool* for SDL3
		const bool* keystate = SDL_GetKeyboardState(&numkeys);

		m_keyStates.resize(numkeys, 0);
		m_prevKeyStates.resize(numkeys, 0);

		if (keystate)
		{
			std::copy(keystate, keystate + numkeys, m_keyStates.begin());
			m_prevKeyStates = m_keyStates;
		}

		return true;
	}

	void Input::Shutdown()
	{
	}

	void Input::Update()
	{
		m_prevKeyStates = m_keyStates;

		int numkeys = 0;
		// Change uint8_t* -> bool* for SDL3
		const bool* keystate = SDL_GetKeyboardState(&numkeys);

		if (keystate && numkeys > 0)
		{
			if (m_keyStates.size() != static_cast<size_t>(numkeys))
			{
				m_keyStates.resize(numkeys, 0);
				m_prevKeyStates.resize(numkeys, 0);
			}

			std::copy(keystate, keystate + numkeys, m_keyStates.begin());
		}

		m_prevButtonStates = m_buttonStates;

		float x = 0.0f;
		float y = 0.0f;
		m_buttonStates = SDL_GetMouseState(&x, &y);
		m_mousePosition = Vector2{ x, y };
	}

	uint32_t Input::GetButtonBit(MouseButton button) const
	{
		return SDL_BUTTON_MASK(static_cast<uint32_t>(button));
	}
}