#pragma once

#include "Library/Nerve/NerveStateBase.h"
#include "Player/PlayerAnimator.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerInput.h"
#include "Player/PlayerTrigger.h"

class PlayerStateHipDrop : public al::ActorStateBase {
public:
    PlayerStateHipDrop(al::LiveActor*, PlayerConst const*, PlayerInput const*,
                       IUsePlayerCollision const*, PlayerAnimator*, PlayerTrigger*);
    ~PlayerStateHipDrop() override;
    void appear() override;
    bool attackHipDropKnockDown(al::HitSensor*, al::HitSensor*);
    bool isEnableHeadSliding() const;
    bool isEnableHipDropAttack() const;
    bool isEnableLandCancel() const;
    bool isEnableMove() const;
    bool isEnableInWater() const;
    bool isEnableIK() const;
    bool isLandTrigger() const;

    void exeStart();
    void exeFall();
    void exeLand();

private:
    const PlayerConst* mPlayerConst;
    const PlayerInput* mPlayerInput;
    const IUsePlayerCollision* mPlayerCollision;
    PlayerAnimator* mPlayerAnimator;
    PlayerTrigger* mPlayerTrigger;
    int mHipDropMsgIntervalCounter = 0;
    bool mIsLandGround = false;
    sead::Vector3f mLandPos = {0.0f, 0.0f, 0.0f};
};
