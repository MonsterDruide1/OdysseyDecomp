#pragma once

#include "Library/Camera/CameraLimitRailKeeper.h"
#include "Library/LiveActor/LiveActor.h"

namespace al {
class CameraRailHolder : public LiveActor {
public:
    CameraRailHolder(const char* name);
    void init(const ActorInitInfo& info) override;

private:
    s32 mCameraRailCount = 0;
    CameraLimitRailKeeper** mCameraRails = nullptr;
};

static_assert(sizeof(CameraRailHolder) == 0x118);
}  // namespace al
