#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class KeyPoseKeeper;
}

class HipDropMoveLift : public al::LiveActor {
public:
    HipDropMoveLift(const char* name);

    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void start();
    void exeStandBy();
    void exeWait();
    void setWaitEndNerve();
    void exeMove();

private:
    al::KeyPoseKeeper* mKeyPoseKeeper = nullptr;
    s32 mMoveTime = 0;
    s32 mWaitTime = 30;
};

static_assert(sizeof(HipDropMoveLift) == 0x118);
