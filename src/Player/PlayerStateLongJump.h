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
    PlayerStateLongJump(al::LiveActor* player, const PlayerConst* pConst, const PlayerInput* input,
                        const IUsePlayerCollision* collider, PlayerTrigger* trigger,
                        PlayerAnimator* animator, PlayerContinuousLongJump* continuousLongJump,
                        PlayerActionDiveInWater* actionDiveInWater);

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
