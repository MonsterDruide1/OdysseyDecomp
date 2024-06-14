#pragma once

#include "Library/Base/StringUtil.h"
#include "Player/PlayerFunction.h"
#include "Player/PlayerModelHolder.h"

namespace game {

class PlayerAnimator {
public:
    PlayerAnimator(PlayerModelHolder const*,al::ActorDitherAnimator *){}
    void startAnim(const sead::SafeString& sName) {
        const char* name = sName.cstr();
        if(al::isEqualString(name, "HipDrop"))
            maxFrame = 44;
        else {
            printf("unknown animation: %s\n", name);
            CRASH
        }
        currentFrame = 0;
        currentAnim = name;
    }
    bool isAnimEnd() {
        return currentFrame >= maxFrame;
    }
    void updateAnimFrame() {
        currentFrame += 1;
    }
    bool isAnim(const sead::SafeString& name) {
        if(currentAnim == nullptr) return false;
        return al::isEqualString(name.cstr(), currentAnim);
    }

private:
    f32 currentFrame = 0.0f;
    f32 maxFrame = 0.0f;
    const char* currentAnim = nullptr;


// stuff required for matching original signatures/calls
public:
    f32 getModelAlpha() { return 1.0f; }

    bool mIsNeedFullFaceAnim = false;
};

}

class PlayerAnimator : public game::PlayerAnimator {
public:
    PlayerAnimator(PlayerModelHolder const*a,al::ActorDitherAnimator *b) : game::PlayerAnimator(a, b){}
};
