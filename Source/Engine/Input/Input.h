#pragma once
#include <vector>
#include <cstdint>
#include "Math/Vector2.h"

namespace nu
{
	class Input
	{
	public:
		enum MouseButton
		{
			Left = 1,
			Middle = 2,
			Right = 3
		};

		bool Initialize();
		void Shutdown();

		void Update();

		// Keyboard Input
		bool GetKeyDown(int key) const { return m_keyStates[key]; }
		bool GetKeyPressed(int key) const { return m_keyStates[key] && !m_prevKeyStates[key]; }
		bool GetKeyReleased(int key) const { return !m_keyStates[key] && m_prevKeyStates[key]; }

		// Mouse Input
		bool GetButtonDown(MouseButton button) const { return m_prevButtonStates & GetButtonBit(button); }
		bool GetButtonCurrentDown(MouseButton button) const { return m_buttonStates & GetButtonBit(button); }
		bool GetButtonPressed(MouseButton button) const { return GetButtonCurrentDown(button) && !GetButtonDown(button); }
		bool GetButtonReleased(MouseButton button) const { return !GetButtonCurrentDown(button) && GetButtonDown(button); }

		Vector2 GetMousePosition() const { return m_mousePosition; }

	private:
		// Keyboard
		std::vector<uint8_t> m_keyStates;
		std::vector<uint8_t> m_prevKeyStates;

		// Mouse
		uint32_t m_buttonStates = 0;
		uint32_t m_prevButtonStates = 0;

		Vector2 m_mousePosition{ 0.0f, 0.0f };

		uint32_t GetButtonBit(MouseButton button) const;
	};
}