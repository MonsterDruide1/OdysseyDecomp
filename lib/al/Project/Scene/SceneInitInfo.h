#pragma once

#include <basis/seadTypes.h>

namespace al {
class AudioDirector;
class GameDataHolderBase;

struct SceneInitInfo {
    struct GameSystemInfo* gameSysInfo;
    GameDataHolderBase* gameDataHolder;
    char field_0x10[8];
    char* initStageName;
    u32 scenarioNo;
    char filler[0x218];
    AudioDirector* audioDirector;
};

static_assert(sizeof(SceneInitInfo) == 0x248);
}  // namespace al
