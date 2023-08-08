#pragma once

class PlayerOxygen {
public:
    PlayerOxygen();

    bool isTriggerDamage() const;
    void recovery();
    void reduce();
    void reset();
    void setup(int no_reduce_frame, int reduce_frame, int recovery_frame, int damage_interval);

private:
    float mOxygenLevel = 1.0;
    int mFramesReducing = 0;
    int mFramesWithoutOxygen = 0;
    int mOxygenNoReduceFrame = 0;
    int mOxygenReduceFrame = 0;
    int mOxygenRecoveryFrame = 0;
    int mOxygenDamageInterval = 0;
};
