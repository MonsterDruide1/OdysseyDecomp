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
    PlayerStateNormalWallJump(al::LiveActor*, const PlayerConst*, const PlayerInput*,
                              const IUsePlayerCollision*, const PlayerTrigger*, PlayerAnimator*,
                              PlayerActionDiveInWater*);

    void appear() override;
    void exeJump();

    const sead::Vector3f& getFront() const { return mFront; }

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
    bool _6c = false;
};
