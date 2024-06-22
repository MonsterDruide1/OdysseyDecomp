#pragma once

#include <basis/seadTypes.h>
#include "PlayerUtil.h"

class PlayerCarryKeeper {
public:
    PlayerCarryKeeper(al::LiveActor const*,al::HitSensor *,PlayerAnimator *,IPlayerModelChanger const*,IUsePlayerCeilingCheck const*,PlayerJointParamHandLegAngle *);
    void startCancelAndRelease();
    bool isCarry();
    bool isThrowRelease();
    bool isCarryUp();
    bool updateCollideLockUp(IUsePlayerCollision const*,PlayerPushReceiver const*);
    bool isThrowHold() const;
    bool isThrow() const;
    bool isCarry() const;

private:
    u8 padding[0x70];
};
