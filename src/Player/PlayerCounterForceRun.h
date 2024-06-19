#pragma once

#include <basis/seadTypes.h>

class PlayerCounterForceRun {
public:
    PlayerCounterForceRun();

    void setupForceRun(s32 frames, f32 speed);
    void update();

    bool isForceRun() const { return mCounter > 0; }
    s32 getCounter() const { return mCounter; }
    f32 getSpeed() const { return mSpeed; }

private:
    s32 mCounter = 0;
    f32 mSpeed = 0.0f;
};
