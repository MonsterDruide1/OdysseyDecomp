#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>

class ScenarioStartCamera;

namespace al {
struct ActorInitInfo;
class Scene;
}  // namespace al

class ScenarioStartCameraHolder {
public:
    ScenarioStartCameraHolder();

    void init(const al::Scene* scene, const al::ActorInitInfo& actorInitInfo);
    bool isExistEnableNextScenarioStartCamera() const;
    bool tryStartNextScenarioStartCamera();
    void startNextScenarioStartCamera();
    s32 getCurrentScenarioStartCameraMaxPlayStep() const;

private:
    sead::FixedPtrArray<ScenarioStartCamera, 8> mScenarioStartCameras;
    ScenarioStartCamera* mCurrentScenarioStartCamera = nullptr;
};

static_assert(sizeof(ScenarioStartCameraHolder) == 0x58);
