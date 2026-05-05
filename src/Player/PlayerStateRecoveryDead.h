#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
}  // namespace al

class ActorDimensionKeeper;
class PlayerColliderHakoniwa;
class PlayerAnimator;
class PlayerConst;
class PlayerRecoverySafetyPoint;

class PlayerStateRecoveryDead : public al::ActorStateBase {
public:
    PlayerStateRecoveryDead(al::LiveActor* player, PlayerRecoverySafetyPoint* recoverySafetyPoint,
                            PlayerColliderHakoniwa* collider, PlayerAnimator* animator,
                            const PlayerConst* pConst, al::LiveActor* capActor);

    void appear() override;
    void kill() override;
    bool isLandFall() const;
    void exeStart();
    void exeRecovery();
    void exeFall();

private:
    PlayerRecoverySafetyPoint* mRecoverySafetyPoint = nullptr;
    PlayerColliderHakoniwa* mCollider = nullptr;
    PlayerAnimator* mAnimator = nullptr;
    const PlayerConst* mConst = nullptr;
    al::LiveActor* mCapActor = nullptr;
    f32 mRecoveryAccel = 0.0f;
    f32 mRecoverySpeed = 0.0f;
    sead::Vector3f mStartTrans = sead::Vector3f::zero;
    sead::Vector3f mStartUp = sead::Vector3f::zero;
    sead::Vector3f mVelocityH = sead::Vector3f::zero;
    s32 mRecoveryFrame = 0;
    s32 mFallStartFrame = 0;
    f32 mQuatRotationRate = 0.0f;
    sead::Quatf mStartQuat = sead::Quatf::unit;
    sead::Vector3f mSideDir = sead::Vector3f::ex;
    sead::Vector3f mRecoveryUp = sead::Vector3f::ey;
    ActorDimensionKeeper* mDimensionKeeper = nullptr;
    sead::Vector3f mMoveDir2D = sead::Vector3f::zero;
    sead::Vector3f mMoveDir2DPrev = sead::Vector3f::zero;
};

static_assert(sizeof(PlayerStateRecoveryDead) == 0xc8);
