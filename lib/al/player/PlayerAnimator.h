#pragma once

#include "PlayerAnimFrameCtrl.h"
#include "sead/prim/seadSafeString.h"

class PlayerAnimator {
    public:
        void startAnim(const sead::SafeStringBase<char> &animName);
        
        bool isAnim(const sead::SafeStringBase<char> &animName) const;

        float getAnimFrame(void) const;
        float getAnimFrameMax(void) const;
        float getAnimFrameRate(void) const;

        unsigned char padding_18[0x18];
        PlayerAnimFrameCtrl *mAnimFrameCtrl;
};