#pragma once
#include <fmod.hpp>
#include <fmod_errors.h>
#include <string>
#include <map>
#include <iostream>

namespace nu
{
    class Audio
    {
    public:
        Audio() = default;

        bool Initialize();
        void Shutdown();
        void Update();

        bool AddSound(const std::string& name, const std::string& filename);
        bool PlaySound(const std::string& name, bool loop = false);
        bool PauseSound(const std::string& name, bool pause = true);
        bool StopSound(const std::string& name);

    private:
        bool CheckFMODResult(FMOD_RESULT result);

    private:
        FMOD::System* m_fmodSystem = nullptr;
        std::map<std::string, FMOD::Sound*> m_sounds;
        std::map<std::string, FMOD::Channel*> m_channels; // Track active channels per sound
    };
}