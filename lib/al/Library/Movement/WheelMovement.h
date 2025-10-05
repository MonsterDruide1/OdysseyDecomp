#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/Math/Axis.h"
#include "Library/Nerve/NerveExecutor.h"

namespace al {
class LiveActor;
struct ActorInitInfo;
class SensorMsg;
class HitSensor;

class WheelMovement : public NerveExecutor {
public:
    WheelMovement(LiveActor* actor, const ActorInitInfo& info);

    bool receiveMsg(LiveActor* actor, const SensorMsg* message, HitSensor* other, HitSensor* self);
    void update(LiveActor* actor);
    void updateRotate();
    void updateActorPoseAndTrans(LiveActor* actor);
    void reset();
    void reset(LiveActor* actor);

    const sead::Vector3f& getMoveDir() const { return mMoveDir; }

    f32 previousDeltaAngle() const { return mPreviousDeltaAngle; }

    f32 deltaAngle() const { return mDeltaAngle; }

    bool isInvertDirection() const { return mIsInvertDirection; }

private:
    sead::Quatf mActorQuat = sead::Quatf::unit;
    sead::Quatf mInitialActorQuat = sead::Quatf::unit;
    sead::Vector3f mMoveDir = sead::Vector3f::ez;
    Axis mRotateAxis = Axis::None;
    f32 mMoveEndDegree = 360.0f;
    f32 mWheelAngle = 0.0f;
    f32 mPreviousDeltaAngle = 0.0f;
    f32 mRotateAccel = 20.0f;
    f32 mDeltaAngle = 0.0f;
    f32 mRotateWidth = 0.0f;
    f32 mNoRotateWidth = 25.0f;
    f32 mRailProgress = 0.0f;
    f32 mInitialRailProgress = 0.0f;
    bool mIsOnRail = false;
    bool mIsRailPlusDir = true;
    bool mIsInvertDirection = false;
};

static_assert(sizeof(WheelMovement) == 0x68);
}  // namespace al
