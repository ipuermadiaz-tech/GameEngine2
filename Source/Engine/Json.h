#pragma once
#include <rapidjson/document.h>
#include <string>
#include <vector>
#include "Vector2.h"
#include "Vector3.h"

// Core Macros expected by Tilemap and Engine Components
#define JSON_HAS_NAME(v, name) (v.HasMember(name))
#define JSON_GET_NAME(v, name) (v[name])

#define JSON_READ(value, data) nu::json::Read(value, #data, data)
#define JSON_READ_NAME(value, name, data) nu::json::Read(value, name, data)
#define JSON_READ_NAME_REQ(value, name, data) nu::json::Read(value, name, data, true)

namespace nu::json
{
    using document_t = rapidjson::Document;

    bool Load(const std::string& filename, rapidjson::Document& document);

    // read json data 
    bool Read(const rapidjson::Value& value, const std::string& name, int& data, bool isRequired = false);
    bool Read(const rapidjson::Value& value, const std::string& name, float& data, bool isRequired = false);
    bool Read(const rapidjson::Value& value, const std::string& name, bool& data, bool isRequired = false);
    bool Read(const rapidjson::Value& value, const std::string& name, std::string& data, bool isRequired = false);
    bool Read(const rapidjson::Value& value, const std::string& name, Vector2& data, bool isRequired = false);
    bool Read(const rapidjson::Value& value, const std::string& name, Vector3& data, bool isRequired = false);
    bool Read(const rapidjson::Value& value, const std::string& name, std::vector<int>& data, bool isRequired = false);
}