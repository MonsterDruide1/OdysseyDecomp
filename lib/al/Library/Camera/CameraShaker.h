#pragma once

#include <math/seadVector.h>

#include "Library/Nerve/NerveExecutor.h"

namespace al {

class CameraShaker : public NerveExecutor {
public:
    enum class ShakeDirection : s32 { Both, Vertical };

    struct ShakeInfo {
        const char* name = nullptr;
        s32 steps = 0;
        f32 speed = 0.0f;
        f32 strength = 0.0f;
        ShakeDirection direction = ShakeDirection::Both;

        bool operator>(const ShakeInfo& other) const {
            if (mStrength < other.mStrength)
                return false;
            if (other.mStrength < mStrength)
                return true;

            if (other.mSteps > 0 && mSteps < 0)
                return false;
            if (other.mSteps < 0 && mSteps > 0)
                return true;

            if (mSteps < other.mSteps)
                return false;
            if (other.mSteps < mSteps)
                return true;

            s32 otherDirection = (s32)other.mDirection;
            s32 direction = (s32)mDirection;
            if (otherDirection < direction)
                return false;
            if (direction < otherDirection)
                return true;

            if (mSpeed < other.mSpeed)
                return true;
            return false;
        }
    };

    CameraShaker();

    void update(const char* shakeLoop);
    void startShakeByAction(const char* name, const char* unused1, const char* unused2, s32 steps);
    void startShakeByName(const char* name, s32 steps);
    void startShakeByHitReaction(const char* name, const char* unused1, const char* unused2,
                                 s32 steps);

    void exeWait();
    void exeShake();
    void exeShakeLoop();

    void startShakeByIndex(s32 index, s32 steps);

private:
    sead::Vector2f mOffset = {0.0f, 0.0f};
    const ShakeInfo* mActiveShake = nullptr;
    const ShakeInfo* mShakeLoop = nullptr;
    ShakeInfo mEditedShake = {};
};

static_assert(sizeof(CameraShaker) == 0x40);

}  // namespace al
