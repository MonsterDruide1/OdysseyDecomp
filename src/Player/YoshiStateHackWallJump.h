#pragma once

#include "Player/HackerStateBase.h"

class IUsePlayerCollision;
class IUsePlayerHack;
class PlayerActionAirMoveControl;
class PlayerAnimator;
class PlayerConst;
class PlayerTrigger;
class YoshiTongue;

namespace al {
class LiveActor;
}  // namespace al

class YoshiStateHackWallJump : public HackerStateBase {
public:
    YoshiStateHackWallJump(al::LiveActor* actor, IUsePlayerHack** hacker, const PlayerConst* pConst,
                           const IUsePlayerCollision* collision, const PlayerTrigger* trigger,
                           const YoshiTongue* tongue, PlayerAnimator* animator);

    void appear() override;
    void exeJump();

private:
    const PlayerConst* mConst;
    const IUsePlayerCollision* mCollision;
    const PlayerTrigger* mTrigger;
    const YoshiTongue* mTongue;
    PlayerAnimator* mAnimator;
    PlayerActionAirMoveControl* mAirMoveControl;
};

static_assert(sizeof(YoshiStateHackWallJump) == 0x58);
