#pragma once
#include "Object.h"
#include <iostream>
#include <map>
#include <memory>
#include <string>

#define FACTORY_REGISTER(classname) \
namespace { \
    nu::RegisterActor<classname> registerInstance_##classname(#classname); \
}

namespace nu {

    // Base interface for all object creators
    class Creator {
    public:
        virtual ~Creator() = default;
        virtual Object* Create() = 0;
    };

    // Singleton Factory class
    class Factory {
    private:
        std::map<std::string, std::unique_ptr<Creator>> m_registry;

        Factory() = default;

    public:
        static Factory& Instance() {
            static Factory instance;
            return instance;
        }

        Factory(const Factory&) = delete;
        Factory& operator=(const Factory&) = delete;

        // Register a creator with a string identifier
        void Register(const std::string& name, std::unique_ptr<Creator> creator) {
            m_registry[name] = std::move(creator);
        }

        // Locate the creator by string name and construct the object
        Object* Create(const std::string& name) {
            auto it = m_registry.find(name);
            if (it != m_registry.end()) {
                std::cout << "Factory: Successfully created object of type '" << name << "'.\n";
                return it->second->Create();
            }

            std::cout << "Factory Error: Creator for '" << name << "' not found.\n";
            return nullptr;
        }

        // Templated helper for creating typed unique_ptr objects (e.g. Components)
        template <typename T>
        std::unique_ptr<T> Create(const std::string& name) {
            Object* obj = Create(name);
            if (!obj) return nullptr;

            T* derived = dynamic_cast<T*>(obj);
            if (!derived) {
                delete obj; // Clean up memory if cast fails
                return nullptr;
            }

            return std::unique_ptr<T>(derived);
        }
    };

    // Templated creator for concrete types
    template <typename T>
    class DerivedCreator : public Creator {
    public:
        Object* Create() override {
            return new T();
        }
    };

    // Helper class for static auto-registration
    template <typename T>
    class RegisterActor {
    public:
        RegisterActor(const std::string& name) {
            Factory::Instance().Register(name, std::make_unique<DerivedCreator<T>>());
        }
    };

} 