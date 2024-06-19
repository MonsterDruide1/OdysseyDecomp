#pragma once

#include "Library/Nerve/NerveStateBase.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerContinuousLongJump.h"
#include "Player/PlayerInput.h"
#include "Player/PlayerTrigger.h"
#include "PlayerUtil.h"

class PlayerStateLongJump : public al::ActorStateBase {
public:
    PlayerStateLongJump(al::LiveActor*, PlayerConst const*, PlayerInput const*,
                        IUsePlayerCollision const*, PlayerTrigger*, PlayerAnimator*,
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
