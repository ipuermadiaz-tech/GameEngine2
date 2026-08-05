#pragma once
#include <vector>
#include "Vector2.h"

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
		bool GetKeyDown(int key) const { return m_keyStates[key]; }
		bool GetKeyPressed(int key) const { return m_keyStates[key]; }
		//bool GetKeyPressed(int key) const { return m_keyStates[key] && !m_prevKeyStates[key]; }
		bool GetKeyReleased(int key) const { return !m_keyStates[key] && m_prevKeyStates[key]; }
		bool GetButtonDown(MouseButton button) const { return m_prevButtonStates & GetButtonBit(button); }
		//uint32_t GetButtonBit(MouseButton button) const;
		


		// 2. NEW HELPER: Reads the current frame state (since your Update stores the latest state in m_buttonStates)
bool GetButtonCurrentDown(MouseButton button) const { return m_buttonStates & GetButtonBit(button); }

// 3. FIXED Pressed: It is down NOW, but was NOT down on the previous frame
bool GetButtonPressed(MouseButton button) const {
    return GetButtonCurrentDown(button) && !GetButtonDown(button);
}

// 4. FIXED Released: It is NOT down now, but WAS down on the previous frame
bool GetButtonReleased(MouseButton button) const { 
    return !GetButtonCurrentDown(button) && GetButtonDown(button); 
}


		Vector2 GetMousePosition() {
			return m_mousePosition;
		}

	private:
		// keyboard 
		std::vector<bool> m_keyStates;
		std::vector<bool> m_prevKeyStates;
		// mouse
		uint32_t m_buttonStates=0;
		uint32_t m_prevButtonStates=0;

		Vector2 m_mousePosition=0;

		uint32_t GetButtonBit(MouseButton button) const;
	};
}