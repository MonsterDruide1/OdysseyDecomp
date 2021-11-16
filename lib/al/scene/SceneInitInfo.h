#pragma once

#include "game/GameData/GameDataHolder.h"

namespace al {
    struct SceneInitInfo {
        struct GameSystemInfo * gameSysInfo;
        GameDataHolderAccessor *gameDataHolder;
        undefined field_0x10[8];
        char *initStageName;
        u32 scenarioNo;
    };
}
