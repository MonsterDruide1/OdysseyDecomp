#pragma once

#include "Library/LiveActor/LiveActor.h"

class CloudStep;

namespace al {
class KeyPoseKeeper;
}

class CloudStepKeyMoveComponent : public al::LiveActor {
public:
    CloudStepKeyMoveComponent(CloudStep* cloudStep);

    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void start();
    void stop();
    void exeWait();
    void exeStandBy();
    void exeMove();
    void exeStop();

private:
    CloudStep* mCloudStep = nullptr;
    al::KeyPoseKeeper* mKeyPoseKeeper = nullptr;
    s32 mWaitTime = 30;
    s32 mMoveTime = 0;
    bool mIsFloorTouchStart = false;
};

static_assert(sizeof(CloudStepKeyMoveComponent) == 0x128);
