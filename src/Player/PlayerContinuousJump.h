#pragma once

#include <math/seadVector.h>

class PlayerConst;

class PlayerContinuousJump {
public:
    PlayerContinuousJump(const PlayerConst* pConst);
    void update(bool shouldCountDown);
    void clear();
    void countUp(const sead::Vector3f& jumpDir);

public:
    const PlayerConst* mConst;
    u32 mCount = 0;
    u32 mTimer = 0;
    sead::Vector3f mLastJumpDir = {0.0f, 0.0f, 0.0f};
};
