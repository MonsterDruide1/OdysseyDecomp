#pragma once

#include "Library/Nerve/NerveStateBase.h"
#include "math/seadVector.h"

class PlayerConst;
class PlayerInput;
class IUsePlayerCollision;
class PlayerTrigger;
class PlayerAnimator;
class PlayerActionDiveInWater;
class PlayerActionAirMoveControl;

class PlayerStateNormalWallJump : public al::ActorStateBase {
public:
    PlayerStateNormalWallJump(al::LiveActor*, PlayerConst const*, PlayerInput const*,
                              IUsePlayerCollision const*, PlayerTrigger const*, PlayerAnimator*,
                              PlayerActionDiveInWater*);
    ~PlayerStateNormalWallJump() override;

    void appear() override;
    void exeJump();

public:
    const PlayerConst* mConst;
    const PlayerInput* mInput;
    const IUsePlayerCollision* mCollider;
    const PlayerTrigger* mTrigger;
    PlayerAnimator* mAnimator;
    PlayerActionDiveInWater* mActionDiveInWater;
    PlayerActionAirMoveControl* mAirMoveControl = nullptr;
    const char* mAnimationName = "WallJump";
    sead::Vector3f mFront;
    bool _6C = false;
};
