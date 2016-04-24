#pragma once

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"

#include <vector>

namespace JSONTools {
    void ReadArray(const char* key, const rapidjson::Value& v, std::vector<int>& dest);
}