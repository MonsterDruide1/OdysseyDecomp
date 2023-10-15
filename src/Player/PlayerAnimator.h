#pragma once

#include <prim/seadSafeString.h>

#include "Player/PlayerAnimFrameCtrl.h"

class PlayerAnimator {
public:
    void startAnim(const sead::SafeStringBase<char>& animName);

    bool isAnim(const sead::SafeStringBase<char>& animName) const;

    f32 getAnimFrame(void) const;
    f32 getAnimFrameMax(void) const;
    f32 getAnimFrameRate(void) const;

    unsigned char padding_18[0x18];
    PlayerAnimFrameCtrl* mAnimFrameCtrl;
};
