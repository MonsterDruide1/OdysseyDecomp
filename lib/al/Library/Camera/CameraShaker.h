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
            if (strength < other.strength)
                return false;
            if (other.strength < strength)
                return true;

            if (other.steps > 0 && steps < 0)
                return false;
            if (other.steps < 0 && steps > 0)
                return true;

            if (steps < other.steps)
                return false;
            if (other.steps < steps)
                return true;

            s32 otherDirection = (s32)other.direction;
            s32 directionCopy = (s32)direction;
            if (otherDirection < directionCopy)
                return false;
            if (directionCopy < otherDirection)
                return true;

            if (speed < other.speed)
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
