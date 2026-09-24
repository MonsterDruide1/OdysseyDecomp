#pragma once

#include <basis/seadTypes.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
class Scene;
}  // namespace al

class StageSceneStateSkipDemo;

class StageSceneStateScenarioCamera : public al::HostStateBase<al::Scene> {
public:
    StageSceneStateScenarioCamera(const char* name, al::Scene* scene, const char* cameraName,
                                  s32 scenarioNo, al::LiveActor* playerActor);

    void setStateSkipDemo(StageSceneStateSkipDemo* skipDemo);
    void appear() override;
    void kill() override;
    bool tryStart();
    bool isExistEnableCamera() const;
    void exeCamera();
    void exeSkip();

private:
    void* _20 = nullptr;
    void* _28 = nullptr;
    const char* mCameraName = nullptr;
    s32 mScenarioNo = 0;
    al::LiveActor* mPlayerActor = nullptr;
    StageSceneStateSkipDemo* mStateSkipDemo = nullptr;
};

static_assert(sizeof(StageSceneStateScenarioCamera) == 0x50);
