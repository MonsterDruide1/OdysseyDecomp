#pragma once

namespace al {
class GameDataHolderBase;

struct SceneInitInfo {
    struct GameSystemInfo* gameSysInfo;
    GameDataHolderBase* gameDataHolder;
    char field_0x10[8];
    char* initStageName;
    u32 scenarioNo;
};
}  // namespace al
