//#include "pch.h"
#include "Input.h"
#include <SDL3/SDL.h>

namespace nu
{
	bool Input::Initialize()
	{
		int numkeys;
		const bool* keystate = SDL_GetKeyboardState(&numkeys);

		m_keyStates.resize(numkeys);

		std::copy(keystate, keystate + numkeys, m_keyStates.begin());
		m_prevKeyStates = m_keyStates;


		return true;
	}
	void Input::Shutdown()
	{
		//
	}
	void Input::Update()
	{
		m_prevKeyStates = m_keyStates;//store last frame key states
		const bool* keystate = SDL_GetKeyboardState(nullptr);
		std::copy(keystate, keystate + m_keyStates.size(), m_keyStates.begin());

		m_prevButtonStates = m_buttonStates;//store last frame button states
		m_buttonStates = SDL_GetMouseState(&m_mousePosition.x, &m_mousePosition.y);
	}
	uint32_t Input::GetButtonBit(MouseButton button) const
	{
		//1 - 0001
		//2-
		return SDL_BUTTON_MASK((uint32_t)button);
	}
}