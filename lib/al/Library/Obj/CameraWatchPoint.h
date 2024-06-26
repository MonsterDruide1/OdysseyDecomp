#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class CameraSubTargetBase;

class CameraWatchPoint : public LiveActor {
public:
    CameraWatchPoint(const char* name);
    void init(const ActorInitInfo& info) override;
    void makeActorAlive() override;
    void kill() override;

private:
    CameraSubTargetBase* mCameraSubTarget = nullptr;
};

static_assert(sizeof(CameraWatchPoint) == 0x110);
}  // namespace al
