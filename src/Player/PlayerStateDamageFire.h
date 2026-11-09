#pragma once

#include "Library/Nerve/NerveStateBase.h"

class PlayerStateDamageFire : public al::ActorStateBase {
public:
    bool isEndFirstLand() const;
    bool isEnableJump() const;
    s32 getEnableCancelCollisionSnapFrame() const;

    void exeJump();
    void exeJump2nd();
    void exeRun();
    void exeFall();
    void exeDead();

private:
    // TODO: class layout not yet determined.
    u8 _20[0xC8];
};
