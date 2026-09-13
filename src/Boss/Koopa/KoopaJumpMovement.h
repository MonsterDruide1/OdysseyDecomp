#pragma once

#include <basis/seadTypes.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
}

class KoopaJumpMovement {
public:
    KoopaJumpMovement();

    void start(const al::LiveActor* actor, const sead::Vector3f& targetTrans,
               const sead::Quatf& targetQuat, const sead::Vector3f& up);
    bool move(al::LiveActor* actor, bool isTurnEndPose);
    void turnEndPoseFaceToPlayer(const al::LiveActor* actor);

private:
    s32 mMoveStepMax = -1;
    s32 mMoveStep = 0;
    sead::Vector3f mStartTrans = sead::Vector3f::zero;
    sead::Vector3f mMoveVector = sead::Vector3f::zero;
    f32 mJumpSpeed = 0.0f;
    sead::Quatf mStartQuat = sead::Quatf::unit;
    sead::Quatf mEndQuat = sead::Quatf::unit;
    sead::Vector3f mUp = sead::Vector3f::ey;
};

static_assert(sizeof(KoopaJumpMovement) == 0x50);
