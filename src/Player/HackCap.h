#pragma once

#include "Library/LiveActor/LiveActor.h"
#include "PlayerUtil.h"

class HackCap : public al::LiveActor {
public:
    HackCap(al::LiveActor const*,char const*,PlayerInput const*,PlayerAreaChecker const*,PlayerWallActionHistory const*,PlayerCapActionHistory const*,PlayerEyeSensorHitHolder const*,PlayerSeparateCapFlag const*,IUsePlayerCollision const*,IUsePlayerHeightCheck const*,PlayerWetControl const*,PlayerJointControlKeeper const*,HackCapJudgePreInputSeparateThrow *,HackCapJudgePreInputSeparateJump *);
    void calcReturnTargetPos(sead::Vector3f*);

    void hide(bool);
    void forcePutOn(void);

    bool isFlying(void) const;

    bool isThrowTypeSpiral(void) const;

    unsigned char padding_to_118[0x10];
    al::LiveActor* mActorA;
    unsigned char padding_08[0x08];
    al::LiveActor* mPlayerActor;
};
