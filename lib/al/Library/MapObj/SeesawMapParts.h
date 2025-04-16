#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class SeesawMapParts : public LiveActor {
public:
    SeesawMapParts(const char* name);

    void init(const ActorInitInfo& info) override;
    bool receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) override;
    void appearAndSetStart();

    void exeWait();

private:
    sead::Quatf mQuat = sead::Quatf::unit;
    sead::Vector3f mSide = sead::Vector3f::ey;
    sead::Vector3f mFront = sead::Vector3f::ez;
    f32 mRotateDegree = 0.0f;
    f32 mRotateSpeed = 0.0f;
    f32 mRotateAccelOn = 0.025f;
    f32 mRotateAccelOff = 0.0125f;
    f32 mMaxDegree = 45.0f;
    f32 mWeight = 0.0f;
    s32 mRemainingAccelOnFrames = 0;
};

static_assert(sizeof(SeesawMapParts) == 0x150);
}  // namespace al
