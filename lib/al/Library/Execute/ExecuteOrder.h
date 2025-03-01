#pragma once

#include <basis/seadTypes.h>

namespace al {
struct ExecuteOrder {
    const char* listName;
    const char* executeGroup;
    s32 listMaxSize;
    const char* groupType;
};

static_assert(sizeof(ExecuteOrder) == 0x20);

}  // namespace al
