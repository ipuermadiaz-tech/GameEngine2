#pragma once

#include "Random.h"
#include "Vector2.h"
#include "Vector3.h"
#include <iostream>
#include <vector>
#include "Input.h"
#include "Texture.h"
#include "Renderer.h"
#include "GameTime.h"
#include "Math_Util.h"
#include "Transformer.h"
#include "Actor.h"
#include "Scene.h"
#include "File.h"
#include "Text.h"
#include "Game.h"
#include "Audio.h"
#include "ParticleSystem.h"
#include "ResourceManager.h"
#include "Json.h"
#include "Factory.h"
#include "Object.h"


#include "Physics.h"


namespace nu
{
	class Engine {
	public:
		static Engine& Get() { static Engine g_engine; return g_engine; }
		bool Initialize();
		void ShutDown();
		void Update();
		void Init();

		Input& GetInput() { return m_input; }
		Renderer& GetRenderer() { return m_renderer; }
		GameTime& GetTime() { return m_time; }
		Audio& GetAudio() { return m_audio; }
		ParticleSystem& GetPS() { return m_particleSystem; }

		Engine& operator=(const Engine&) = delete;
		Physics& GetPhysics() { return m_physics; }

	private:
		Engine() = default;
		Physics m_physics;
		Input m_input;
		Renderer m_renderer;
		Audio m_audio;
		GameTime m_time;
		ParticleSystem m_particleSystem;
	};
}