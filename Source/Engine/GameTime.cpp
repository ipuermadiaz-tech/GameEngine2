#include "GameTime.h"
#include <SDL3/SDL.h>
namespace nu {
	GameTime::GameTime()
	{
		Reset();
	}
	void GameTime::Reset()
	{
		m_startTicks = SDL_GetTicksNS();
		m_frameTicks = SDL_GetTicksNS();
		m_currentTicks = SDL_GetTicksNS();
	}
	void GameTime::Tick()
	{
		m_currentTicks= SDL_GetTicksNS();//get current tocls
		m_deltaTicks = m_currentTicks - m_frameTicks;//delta = curremt ticks - prev ticks
		m_frameTicks = m_currentTicks;//set prev ticks
	}

}