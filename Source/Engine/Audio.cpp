#include "Audio.h"
#include <fmod_errors.h> // Required for FMOD_ErrorString
#include <iostream>

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

        // Insert sound into map
        m_sounds[name] = sound;

        return true;
    }

    bool Audio::PlaySound(const std::string& name)
    {

        auto iter = m_sounds.find(name);
        if (iter == m_sounds.end())
        {
            std::cerr << "Audio System : name doesn't exist " << name << std::endl;
            return false;
        }

  
        FMOD_RESULT result = m_fmodSystem->playSound(iter->second, nullptr, false, nullptr);
        if (!CheckFMODResult(result))
            return false;

        return true;
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