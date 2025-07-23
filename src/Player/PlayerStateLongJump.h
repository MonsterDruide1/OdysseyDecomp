#pragma once

#include "Library/Nerve/NerveStateBase.h"

class PlayerConst;
class PlayerInput;
class IUsePlayerCollision;
class PlayerTrigger;
class PlayerAnimator;
class PlayerContinuousLongJump;
class PlayerActionDiveInWater;

class PlayerStateLongJump : public al::ActorStateBase {
public:
    PlayerStateLongJump(al::LiveActor*,const PlayerConst *,const PlayerInput *,
                      const  IUsePlayerCollision *, PlayerTrigger*, PlayerAnimator*,
                        PlayerContinuousLongJump*, PlayerActionDiveInWater*);
    ~PlayerStateLongJump() override;

    void appear() override;
    void exeJump();

private:
    const PlayerConst* mConst;
    const PlayerInput* mInput;
    const IUsePlayerCollision* mCollision;
    PlayerTrigger* mTrigger;
    PlayerAnimator* mAnimator;
    PlayerContinuousLongJump* mContinuousLongJump;
    PlayerActionDiveInWater* mDiveInWater;
};
