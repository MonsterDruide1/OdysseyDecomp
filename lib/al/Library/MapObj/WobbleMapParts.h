#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class WobbleMapParts : public LiveActor {
public:
    WobbleMapParts(const char* name);

    void init(const ActorInitInfo& info) override;
    bool receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) override;

    void appearAndSetStart();
    void exeWait();
    void updateMove();
    void exeMove();
    void exeAssistStop();

private:
    sead::Quatf mInitialQuat = sead::Quatf::unit;
    sead::Vector3f mInitialUp = sead::Vector3f::ey;
    sead::Quatf mCurrentQuat = sead::Quatf::unit;
    sead::Vector3f mMoment = sead::Vector3f::zero;
    sead::Vector3f mTargetUp = sead::Vector3f::ey;
    sead::Vector3f mUnused = sead::Vector3f::ey;
    f32 mMaxRotate = 10.0f;
    f32 mTiltSpeed = 0.0f;
    s32 mAssistStopTimer = 0;
    bool mIsStop = false;
};

static_assert(sizeof(WobbleMapParts) == 0x168);
}  // namespace al
