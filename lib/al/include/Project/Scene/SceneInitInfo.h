#pragma once

#include <basis/seadTypes.h>

#include "System/GameDataHolderAccessor.h"

namespace al {
struct SceneInitInfo {
    struct GameSystemInfo* gameSysInfo;
    GameDataHolderAccessor* gameDataHolder;
    char field_0x10[8];
    char* initStageName;
    u32 scenarioNo;
};
}  // namespace al
