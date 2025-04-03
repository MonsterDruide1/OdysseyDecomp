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

    f32 get_48() const { return _48; }

    f32 get_50() const { return _50; }

    bool get_66() const { return _66; }

private:
    sead::Quatf _10 = sead::Quatf::unit;
    sead::Quatf _20 = sead::Quatf::unit;
    sead::Vector3f mMoveDir = sead::Vector3f::ez;
    Axis mRotateAxis = Axis::None;
    f32 mMoveEndDegree = 360.0f;
    f32 _44 = 0.0f;
    f32 _48 = 0.0f;
    f32 mRotateAccel = 20.0f;
    f32 _50 = 0.0f;
    f32 _54 = 0.0f;
    f32 mNoRotateWidth = 25.0f;
    f32 _5c = 0.0f;
    f32 _60 = 0.0f;
    bool _64 = false;
    bool mIsRailPlusDir = true;
    bool _66 = false;
};

static_assert(sizeof(WheelMovement) == 0x68);
}  // namespace al
