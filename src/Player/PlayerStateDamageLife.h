#pragma once

#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

class PlayerConst;
class IUsePlayerCollision;
class PlayerInput;
class IUsePlayerCeilingCheck;
class PlayerAnimator;
class PlayerTrigger;
class PlayerActionAirMoveControl;

class PlayerStateDamageLife : public al::ActorStateBase {
public:
    PlayerStateDamageLife(al::LiveActor* player, const PlayerConst* pConst,
                          const IUsePlayerCollision* collider, const PlayerInput* input,
                          const IUsePlayerCeilingCheck* ceilingCheck, PlayerAnimator* animator,
                          PlayerTrigger* trigger);
    void appear() override;
    bool isLand() const;
    bool isEnableCancel() const;
    bool isFormSquat() const;
    void exeDamage();
    void exeLand();
    void exeDead();

private:
    const PlayerConst* mConst;
    const IUsePlayerCollision* mCollider;
    const IUsePlayerCeilingCheck* mCeilingCheck;
    PlayerAnimator* mAnimator;
    PlayerTrigger* mTrigger;
    PlayerActionAirMoveControl* mActionAirMoveControl = nullptr;
    sead::Vector3f mUp = sead::Vector3f::zero;
    f32 mBrakeLandUpFactor = 0.0f;
    bool mIsNoDamageDown = false;
};
