#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class CameraTicket;
class LiveActor;
class Scene;
}  // namespace al

class StageSceneStateGetShineMainLast : public al::NerveStateBase {
public:
    StageSceneStateGetShineMainLast(const char*, al::Scene*, al::LiveActor*, al::CameraTicket*);

    void appear() override;
    void kill() override;

    void exeWait();

private:
    al::Scene* mScene = nullptr;
    al::LiveActor* mDemoShine = nullptr;
    al::CameraTicket* mCameraTicket = nullptr;
};

static_assert(sizeof(StageSceneStateGetShineMainLast) == 0x30);
