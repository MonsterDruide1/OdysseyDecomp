#pragma once

#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

class CloudStepKeyMoveComponent;

class CloudStep : public al::LiveActor {
public:
    CloudStep(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void control() override;

    void exeWait();
    void exeWaitOnPlayer();
    void exeAppear();
    void endAppear();
    void exeDisappear();
    void endDisappear();
    void exeThrough();
    void exeLand();

private:
    s32 _108 = -1;
    s32 _10c = -1;
    s32 _110 = -1;
    bool mIsSteppedOn = false;
    sead::Vector3f mClippingOffset = sead::Vector3f::zero;
    sead::Vector3f mThroughEffectPos = sead::Vector3f::zero;
    CloudStepKeyMoveComponent* mKeyMoveComponent = nullptr;
};

static_assert(sizeof(CloudStep) == 0x138);
