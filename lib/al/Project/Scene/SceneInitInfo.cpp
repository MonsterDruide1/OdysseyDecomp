#include "Project/Scene/SceneInitInfo.h"

namespace al {
SceneInitInfo::SceneInitInfo(const GameSystemInfo* gameSystemInfo,
                             GameDataHolderBase* gameDataHolder,
                             ScreenCaptureExecutor* screenCaptureExecutor,
                             const char* initStageName, s32 scenarioNo, const char* initSceneName,
                             AudioDirector* audioDirector)
    : gameSystemInfo(gameSystemInfo), gameDataHolder(gameDataHolder),
      screenCaptureExecutor(screenCaptureExecutor), initStageName(initStageName),
      scenarioNo(scenarioNo), audioDirector(audioDirector) {
    sceneName = initSceneName;
}
}  // namespace al
