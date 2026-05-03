#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class KeyPoseKeeper;
}

namespace al {
class SwitchKeyMoveMapParts : public LiveActor {
public:
    SwitchKeyMoveMapParts(const char* name);

    void init(const ActorInitInfo& info) override;

    void on();
    void off();
    void reverse();
    void stop();

    void exeStandBy();
    void exeWait();
    void exeMoveSign();
    void exeMove();
    void exeStopSign();
    void exeStop();

private:
    KeyPoseKeeper* mKeyPoseKeeper = nullptr;
    sead::Vector3f mClippingOffset = sead::Vector3f::zero;
    s32 mKeyMoveWaitTime = 30;
    s32 mKeyMoveMoveTime = 0;
    f32 mMoveRate = 0.0f;
    bool mIsReverse = false;
};

static_assert(sizeof(SwitchKeyMoveMapParts) == 0x130);
}  // namespace al
