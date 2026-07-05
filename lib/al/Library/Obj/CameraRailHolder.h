#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class CameraLimitRailKeeper;

class CameraRailHolder : public LiveActor {
public:
    CameraRailHolder(const char* name);
    void init(const ActorInitInfo& info) override;

    s32 getRailCount() const { return mCameraRailCount; }

    CameraLimitRailKeeper* getRail(s32 index) { return mCameraRails[index]; }

private:
    s32 mCameraRailCount = 0;
    CameraLimitRailKeeper** mCameraRails = nullptr;
};

static_assert(sizeof(CameraRailHolder) == 0x118);
}  // namespace al
