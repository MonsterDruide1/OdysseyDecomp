#pragma once

#include <basis/seadTypes.h>

class PlayerOxygen {
public:
    PlayerOxygen();

    bool isTriggerDamage() const;
    void recovery();
    void reduce();
    void reset();
    void setup(s32 no_reduce_frame, s32 reduce_frame, s32 recovery_frame, s32 damage_interval);

private:
    f32 mOxygenLevel = 1.0;
    s32 mFramesReducing = 0;
    s32 mFramesWithoutOxygen = 0;
    s32 mOxygenNoReduceFrame = 0;
    s32 mOxygenReduceFrame = 0;
    s32 mOxygenRecoveryFrame = 0;
    s32 mOxygenDamageInterval = 0;
};
