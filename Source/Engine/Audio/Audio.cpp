#include "pch.h"
#include "Audio.h"

namespace nu {

    bool Audio::Initialize()
    {
        FMOD_RESULT result = FMOD::System_Create(&m_fmodSystem);
        if (!CheckFMODResult(result))
            return false;

        void* extradriverdata = nullptr;
        result = m_fmodSystem->init(32, FMOD_INIT_NORMAL, extradriverdata);
        if (!CheckFMODResult(result))
            return false;

        return true;
    }

    void Audio::Shutdown()
    {
        for (auto& pair : m_sounds)
        {
            if (pair.second)
            {
                pair.second->release();
            }
        }
        m_sounds.clear();
        m_channels.clear();

        if (m_fmodSystem)
        {
            CheckFMODResult(m_fmodSystem->release());
        }
    }

    void Audio::Update()
    {
        if (m_fmodSystem)
        {
            CheckFMODResult(m_fmodSystem->update());
        }
    }

    bool Audio::AddSound(const std::string& name, const std::string& filename)
    {
        if (m_sounds.find(name) != m_sounds.end())
        {
            std::cerr << "Audio System : name already exists " << name << std::endl;
            return false;
        }

        FMOD::Sound* sound = nullptr;
        FMOD_RESULT result = m_fmodSystem->createSound(filename.c_str(), FMOD_DEFAULT, nullptr, &sound);
        if (!CheckFMODResult(result))
            return false;

        m_sounds[name] = sound;

        return true;
    }

    bool Audio::PlaySound(const std::string& name, bool loop)
    {
        auto iter = m_sounds.find(name);
        if (iter == m_sounds.end())
        {
            std::cerr << "Audio System : name doesn't exist " << name << std::endl;
            return false;
        }

        // Set looping mode on the sound
        iter->second->setMode(loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);

        FMOD::Channel* channel = nullptr;
        FMOD_RESULT result = m_fmodSystem->playSound(iter->second, nullptr, false, &channel);
        if (!CheckFMODResult(result))
            return false;

        // Save channel handle for pause/stop commands
        m_channels[name] = channel;

        return true;
    }

    bool Audio::PauseSound(const std::string& name, bool pause)
    {
        auto iter = m_channels.find(name);
        if (iter == m_channels.end() || !iter->second)
        {
            return false;
        }

        FMOD_RESULT result = iter->second->setPaused(pause);
        return CheckFMODResult(result);
    }

    bool Audio::StopSound(const std::string& name)
    {
        auto iter = m_channels.find(name);
        if (iter == m_channels.end() || !iter->second)
        {
            return false;
        }

        FMOD_RESULT result = iter->second->stop();
        m_channels.erase(iter);
        return CheckFMODResult(result);
    }

    bool Audio::CheckFMODResult(FMOD_RESULT result)
    {
        if (result != FMOD_OK)
        {
            std::cerr << FMOD_ErrorString(result) << std::endl;
            return false;
        }

        return true;
    }

} // namespace nu