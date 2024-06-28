#pragma once

#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveStateBase.h"
#include "Player/IUsePlayerCollision.h"
#include "Player/PlayerActionGroundMoveControl.h"
#include "Player/PlayerAnimator.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerInput.h"

class PlayerStateGroundSpin : public al::ActorStateBase {
public:
    PlayerStateGroundSpin(al::LiveActor*, IUsePlayerCollision const*, PlayerInput const*,
                          PlayerConst const*, PlayerAnimator*);
    ~PlayerStateGroundSpin() override;

    void appear() override;
    void exeGroundSpin();

public:
    const IUsePlayerCollision* mCollision;
    const PlayerInput* mPlayerInput;
    const PlayerConst* mPlayerConst;
    PlayerAnimator* mPlayerAnimator;
    PlayerActionGroundMoveControl* mGroundMoveCtrl = nullptr;
    bool mIsSpinClockwise = false;
};
