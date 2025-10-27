#pragma once

#include <basis/seadTypes.h>
#include <prim/seadSafeString.h>

namespace al {
struct GameSystemInfo;
class GameDataHolderBase;
class ScreenCaptureExecutor;
class AudioDirector;

struct SceneInitInfo {
    SceneInitInfo(const GameSystemInfo* gameSystemInfo, GameDataHolderBase* gameDataHolder,
                  ScreenCaptureExecutor* screenCaptureExecutor, const char* initStageName,
                  s32 scenarioNo, const char* sceneName, AudioDirector* audioDirector);
    const GameSystemInfo* gameSystemInfo;
    GameDataHolderBase* gameDataHolder;
    ScreenCaptureExecutor* screenCaptureExecutor;
    const char* initStageName;
    s32 scenarioNo;
    sead::FixedSafeString<512> sceneName;
    AudioDirector* audioDirector;
};

static_assert(sizeof(SceneInitInfo) == 0x248);
}  // namespace al
