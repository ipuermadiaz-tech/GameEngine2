//miau
#include "Engine.h"
//#include "framework.h"
#include <iostream>

namespace nu
{
	//Engine g_engine;

	bool Engine::Initialize()
	{
		m_renderer.Initialize("Game Engine", 1920, 1024);
	    m_input.Initialize();
		m_audio.Initialize();
		m_particleSystem.Initialize();
		return true;


	}
	void Engine::ShutDown(){
		m_input.Shutdown();
		m_renderer.Shutdown();
		m_audio.Shutdown();
		m_particleSystem.Shutdown();
	}
	void Engine::Update()
	{
		m_input.Update();
		m_audio.Update();
		m_time.Tick();
		m_particleSystem.Update(m_time.GetDeltaTime());
	}

}