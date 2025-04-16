#pragma once

#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class HitSensor;
class LiveActor;
}  // namespace al
class IUsePlayerCollision;
class PlayerAnimator;
class PlayerConst;
class PlayerInput;
class PlayerTrigger;

class PlayerStateHipDrop : public al::ActorStateBase {
public:
    PlayerStateHipDrop(al::LiveActor* player, const PlayerConst* pConst, const PlayerInput* input,
                       const IUsePlayerCollision* collider, PlayerAnimator* animator,
                       PlayerTrigger* trigger);
    void appear() override;
    bool attackHipDropKnockDown(al::HitSensor* sender, al::HitSensor* receiver);
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
    const PlayerConst* mConst;
    const PlayerInput* mInput;
    const IUsePlayerCollision* mCollision;
    PlayerAnimator* mAnimator;
    PlayerTrigger* mTrigger;
    s32 mHipDropMsgIntervalCounter = 0;
    bool mIsLandGround = false;
    sead::Vector3f mLandPos = {0.0f, 0.0f, 0.0f};
};
