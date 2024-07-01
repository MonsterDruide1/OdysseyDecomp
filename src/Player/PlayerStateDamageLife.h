#pragma once

#include "Library/Nerve/NerveStateBase.h"
#include "math/seadVector.h"

class PlayerConst;
class IUsePlayerCollision;
class PlayerInput;
class IUsePlayerCeilingCheck;
class PlayerAnimator;
class PlayerTrigger;
class PlayerActionAirMoveControl;

class PlayerStateDamageLife : public al::ActorStateBase {
public:
    PlayerStateDamageLife(al::LiveActor*, PlayerConst const*, IUsePlayerCollision const*,
                          PlayerInput const*, IUsePlayerCeilingCheck const*, PlayerAnimator*,
                          PlayerTrigger*);
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
    sead::Vector3f mUp = {0.0f, 0.0f, 0.0f};
    float _5C = 0.0f;
    bool mIsNoDamageDown = false;
};
