#pragma once

#include "Library/Nerve/NerveStateBase.h"

class IUsePlayerCollision;
class PlayerInput;
class PlayerConst;
class PlayerAnimator;
class PlayerActionGroundMoveControl;

class PlayerStateGroundSpin : public al::ActorStateBase {
public:
    PlayerStateGroundSpin(al::LiveActor* parent, const IUsePlayerCollision* collision,
                          const PlayerInput* input, const PlayerConst* pConst,
                          PlayerAnimator* animator);

    void appear() override;
    void exeGroundSpin();

private:
    const IUsePlayerCollision* mCollision;
    const PlayerInput* mPlayerInput;
    const PlayerConst* mPlayerConst;
    PlayerAnimator* mPlayerAnimator;
    PlayerActionGroundMoveControl* mGroundMoveCtrl = nullptr;
    bool mIsSpinClockwise = false;
};
