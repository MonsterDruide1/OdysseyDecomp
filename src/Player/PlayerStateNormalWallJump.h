#pragma once

#include "CUSTOM/PlayerAnimator.h"
#include "Library/Nerve/NerveStateBase.h"
#include "PlayerUtil.h"
#include "Stuff.h"

class PlayerStateNormalWallJump : public al::ActorStateBase {
public:
    PlayerStateNormalWallJump(al::LiveActor*, PlayerConst const*, PlayerInput const*,
                              IUsePlayerCollision const*, PlayerTrigger const*, PlayerAnimator*,
                              PlayerActionDiveInWater*);
    ~PlayerStateNormalWallJump() override;

    void appear() override;
    void exeJump();

private:
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
