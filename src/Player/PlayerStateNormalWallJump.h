#pragma once

#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

class IUsePlayerCollision;
class PlayerActionAirMoveControl;
class PlayerActionDiveInWater;
class PlayerAnimator;
class PlayerConst;
class PlayerInput;
class PlayerTrigger;

namespace al {
class LiveActor;
}  // namespace al

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
