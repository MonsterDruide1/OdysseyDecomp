#include "Project/Scene/SceneInitInfo.h"

namespace al {
SceneInitInfo::SceneInitInfo(const GameSystemInfo* gameSysInfo, GameDataHolderBase* gameDataHolder,
                             ScreenCaptureExecutor* screenCaptureExecutor,
                             const char* initStageName, s32 scenarioNo, const char* initSceneName,
                             AudioDirector* audioDirector)
    : gameSysInfo(gameSysInfo), gameDataHolder(gameDataHolder),
      screenCaptureExecutor(screenCaptureExecutor), initStageName(initStageName),
      scenarioNo(scenarioNo), audioDirector(audioDirector) {
    sceneName = initSceneName;
}
}  // namespace al
