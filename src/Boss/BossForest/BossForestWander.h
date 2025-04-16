#pragma once

#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class KeyPoseKeeper;
}

class BossForestWander : public al::LiveActor {
public:
    BossForestWander(const char* name);
    void init(const al::ActorInitInfo& info) override;

    void exeWaitSwitchStart();
    void exeWait();
    void exeMove();
    void exeEnd();

private:
    al::KeyPoseKeeper* mKeyPoseKeeper = nullptr;
    s32 mMoveTime = -1;
    s32 mWaitTime = -1;
    sead::Vector3f mClippingRadius = sead::Vector3f::zero;
};
