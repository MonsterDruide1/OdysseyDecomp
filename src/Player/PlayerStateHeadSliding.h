#pragma once

#include "CUSTOM/PlayerAnimator.h"
#include "Library/Nerve/NerveStateBase.h"
#include "Player/IUsePlayerCollision.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerInput.h"
#include "PlayerUtil.h"

class PlayerStateHeadSliding : public al::ActorStateBase {
public:
    PlayerStateHeadSliding(al::LiveActor*, PlayerConst const*, IUsePlayerCollision const*,
                           PlayerInput const*, PlayerActionDiveInWater const*, PlayerAnimator*);
    ~PlayerStateHeadSliding() override;

    void appear() override;
    void kill() override;

    bool isEnableDiveInWater() const;

    void exeDive();

private:
    const PlayerConst* mConst;
    const IUsePlayerCollision* mCollider;
    const PlayerInput* mInput;
    const PlayerActionDiveInWater* mActionDiveInWater;
    PlayerAnimator* mAnimator;
    bool mIsEnableDiveInWater = false;
};
