#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class CameraTicket;
class LiveActor;
class Scene;
}  // namespace al

class ScenarioStartCameraHolder;
class StageSceneStateSkipDemo;

class StageSceneStateScenarioCamera : public al::HostStateBase<al::Scene> {
public:
    StageSceneStateScenarioCamera(const char*, al::Scene*, const char*, s32, al::LiveActor*);
    ~StageSceneStateScenarioCamera() override;

    void setStateSkipDemo(StageSceneStateSkipDemo*);

    void appear() override;
    void kill() override;

    bool tryStart();
    bool isExistEnableCamera() const;
    void exeCamera();
    void exeSkip();

private:
    al::CameraTicket* mCameraTicket = nullptr;
    ScenarioStartCameraHolder* mScenarioStartCameraHolder = nullptr;
    const char* mStageName = nullptr;
    s32 mScenarioNo = 0;
    al::LiveActor* mDemoActor = nullptr;
    StageSceneStateSkipDemo* mStateSkipDemo = nullptr;
};

static_assert(sizeof(StageSceneStateScenarioCamera) == 0x50);
