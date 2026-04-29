#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

class CapAppearTarget;

class CapAppearMapParts : public al::LiveActor {
public:
    CapAppearMapParts(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void appear() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void control() override;

    void listenAppear();
    void startWait();
    void killAll();

    void exeAppear();
    void exeAppearEnd();
    void exeWait();
    void exeReaction();
    void exeWaitForever();
    void exeDisappear();

private:
    CapAppearTarget* mTarget = nullptr;
    s32 mAppearTime = 0;
    sead::Vector3f mTargetTrans = sead::Vector3f::zero;
    f32 mDisappearDistance = 0.0f;
    bool mHasDisappearSign = false;
    sead::Vector3f mSensorPos = sead::Vector3f::zero;
    bool mIsUseShadowMask = false;
};

static_assert(sizeof(CapAppearMapParts) == 0x138);
