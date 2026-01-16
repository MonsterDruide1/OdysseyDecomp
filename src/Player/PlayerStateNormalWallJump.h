#pragma once

#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

class PlayerConst;
class PlayerInput;
class IUsePlayerCollision;
class PlayerTrigger;
class PlayerAnimator;
class PlayerActionDiveInWater;
class PlayerActionAirMoveControl;

class PlayerStateNormalWallJump : public al::ActorStateBase {
public:
    PlayerStateNormalWallJump(al::LiveActor* player, const PlayerConst* pConst,
                              const PlayerInput* input, const IUsePlayerCollision* collider,
                              const PlayerTrigger* trigger, PlayerAnimator* animator,
                              PlayerActionDiveInWater* actionDiveInWater);

    void appear() override;
    void exeJump();

    const sead::Vector3f& getFront() const { return mFront; }

private:
    const PlayerConst* mConst;
    const IUsePlayerCollision* mCollider;
    const PlayerInput* mInput;
    const PlayerTrigger* mTrigger;
    PlayerAnimator* mAnimator;
    PlayerActionDiveInWater* mActionDiveInWater;
    PlayerActionAirMoveControl* mAirMoveControl = nullptr;
    const char* mAnimationName = "WallJump";
    sead::Vector3f mFront;
    bool mIsJumpTowardsWall = false;
};
