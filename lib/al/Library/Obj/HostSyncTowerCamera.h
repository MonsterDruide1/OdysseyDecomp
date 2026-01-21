#pragma once

#include "Library/Nerve/NerveExecutor.h"

namespace al {
struct ActorInitInfo;
class AreaObjGroup;
class CameraTicket;
class LiveActor;

class HostSyncTowerCamera : public NerveExecutor {
public:
    HostSyncTowerCamera(LiveActor* actor, AreaObjGroup* activationArea, const ActorInitInfo& info);
    void update();
    void activate();
    void deactivate();
    void tryEndCamera();
    void exeActive();
    void tryStartCamera();
    void exeDeactive();

private:
    LiveActor* mActor = nullptr;
    CameraTicket* mTowerCamera = nullptr;
    AreaObjGroup* mActivationArea = nullptr;
};

static_assert(sizeof(HostSyncTowerCamera) == 0x28);

}  // namespace al
