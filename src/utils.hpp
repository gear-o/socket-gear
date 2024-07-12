#pragma once
#include <string>
#include <uuid/uuid.h>

namespace socketGearUtils{
    std::string uuid(){
        uuid_t uuid;
        uuid_generate(uuid);
        char uuid_str[37];
        uuid_unparse(uuid, uuid_str);
        return uuid_str;
    }
}