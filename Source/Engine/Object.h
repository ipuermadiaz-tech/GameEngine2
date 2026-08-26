#pragma once
#include "Json.h"
#include <rapidjson/document.h>
#include <iostream>
#include <memory>
#include <string>
#define CLASS_PROTOTYPE(classname) virtual std::unique_ptr<Object> Clone() const {return std::make_unique<classname>(*this);}

namespace nu {
    class Object {
    public:
     


        Object() = default;
        Object(const std::string& name) : m_name{ name } {}
        virtual ~Object() = default;
      
        CLASS_PROTOTYPE(Object);

        virtual bool Read(const rapidjson::Value& value) {
            JSON_READ(value, m_name);
            return true;
        }

        const std::string& GetName() const { return m_name; }
        void SetName(const std::string& name) { m_name = name; }

    protected:
        std::string m_name = "";
    };
}