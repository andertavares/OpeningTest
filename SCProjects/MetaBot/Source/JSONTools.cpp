#include "JSONTools.h"

namespace JSONTools {
    void ReadArray(const char* key, const rapidjson::Value& v, std::vector<int>& dest) {
        if (v.HasMember(key)) {
            const rapidjson::Value& rjson_array = v[key];
            assert(rjson_array.IsArray());
            dest.resize(rjson_array.Size());
            for (rapidjson::SizeType i = 0; i < rjson_array.Size(); i++)
                dest[i] = (rjson_array[i].GetInt());
        }
    }
}