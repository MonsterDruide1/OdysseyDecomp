#pragma once

#include <math/seadQuat.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
}

class PlayerRecoverySafetyPoint;
class PlayerColliderHakoniwa;
class PlayerAnimator;
class PlayerConst;
class ActorDimensionKeeper;

class PlayerStateRecoveryDead : public al::ActorStateBase {
public:
    PlayerStateRecoveryDead(al::LiveActor* player, PlayerRecoverySafetyPoint* recoverySafetyPoint,
                            PlayerColliderHakoniwa* playerCollider, PlayerAnimator* playerAnimator,
                            const PlayerConst* playerConst, al::LiveActor* playerModelHolder);

    ~PlayerStateRecoveryDead() override;
    void appear() override;
    void kill() override;

    void exeFall();
    void exeRecovery();
    void exeStart();
    bool isLandFall() const;

private:
    PlayerRecoverySafetyPoint* mPlayerRecoverySafetyPoint;
    PlayerColliderHakoniwa* mPlayerColliderHakoniwa;
    PlayerAnimator* mPlayerAnimator;
    const PlayerConst* mPlayerConst;
    al::LiveActor* mPlayerModelHolder;
    f32 _48;
    f32 _4c;
    sead::Vector3f mTrans;
    sead::Vector3f mNegativeGravity;
    sead::Vector3f _68;
    s32 _74;
    s32 _78;
    f32 _7c;
    sead::Quatf _80;
    sead::Vector3f _90;
    sead::Vector3f _9c;
    ActorDimensionKeeper* mDimensionKeeper;
    sead::Vector3f _b0;
    sead::Vector3f _bc;
};

static_assert(sizeof(PlayerStateRecoveryDead) == 0xc8);
