#include "Player/PlayerOxygen.h"

#include <math/seadMathCalcCommon.h>

PlayerOxygen::PlayerOxygen() = default;

void PlayerOxygen::setup(s32 no_reduce_frame, s32 reduce_frame, s32 recovery_frame,
                         s32 damage_interval) {
    mOxygenNoReduceFrame = no_reduce_frame;
    mOxygenReduceFrame = reduce_frame;
    mOxygenRecoveryFrame = recovery_frame;
    mOxygenDamageInterval = damage_interval;
}

void PlayerOxygen::reset() {
    mOxygenLevel = 1.0;
    mFramesReducing = 0;
    mFramesWithoutOxygen = 0;
}

void PlayerOxygen::reduce() {
    if (mFramesReducing++ >= mOxygenNoReduceFrame) {
        if (mOxygenLevel <= 0)
            mFramesWithoutOxygen++;
        mOxygenLevel = sead::Mathf::max(mOxygenLevel - (1.0f / mOxygenReduceFrame), 0.0f);
    }
}

#ifdef NON_MATCHING
void PlayerOxygen::recovery() {
    // different regalloc
    mOxygenLevel = sead::Mathf::min(1.0f, mOxygenLevel + (1.0f / mOxygenRecoveryFrame));
    mFramesReducing = 0;
    mFramesWithoutOxygen = 0;
}
#endif

bool PlayerOxygen::isTriggerDamage() const {
    if (mFramesWithoutOxygen) {
        return (mFramesWithoutOxygen % mOxygenDamageInterval) == 0;
    }
    return false;
}
