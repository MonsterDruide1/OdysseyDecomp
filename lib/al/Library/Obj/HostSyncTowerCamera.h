#pragma once

#include "Library/Nerve/NerveExecutor.h"

namespace al {
struct ActorInitInfo;
class AreaObjGroup;
class CameraTicket;
class LiveActor;

class HostSyncTowerCamera : public NerveExecutor {
public:
    HostSyncTowerCamera(LiveActor*, AreaObjGroup*, const ActorInitInfo&);
    void update();
    void activate();
    void deactivate();
    void tryEndCamera();
    void exeActive();
    void tryStartCamera();
    void exeDeactive();

private:
    LiveActor* _10;
    CameraTicket* _18;
    AreaObjGroup* _20;
};

static_assert(sizeof(HostSyncTowerCamera) == 0x28);

}  // namespace al
