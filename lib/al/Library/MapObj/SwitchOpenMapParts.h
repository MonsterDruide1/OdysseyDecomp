#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class SwitchOpenMapParts : public LiveActor {
public:
    SwitchOpenMapParts(const char* name);

    void init(const ActorInitInfo& info) override;

    void open();
    void close();

    void exeWait();
    void exeDelayOpen();
    void exeOpen();
    void exeWaitOpend();
    void exeDelayClose();
    void exeClose();

private:
    sead::Vector3f mTrans = sead::Vector3f::zero;
    sead::Vector3f mEndPointDir = sead::Vector3f::zero;
    f32 mEndPointDist = 0.0f;
    f32 mCoord = 0.0f;
    f32 mSpeedOpen = 0.0f;
    f32 mSpeedClose = 0.0f;
    s32 mDelayTimeOpen = 0;
    s32 mDelayTimeClose = 0;
};

static_assert(sizeof(SwitchOpenMapParts) == 0x138);
}  // namespace al
