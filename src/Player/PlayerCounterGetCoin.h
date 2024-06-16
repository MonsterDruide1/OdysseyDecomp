#pragma once

#include <basis/seadTypes.h>

class PlayerCounterGetCoin {
public:
    PlayerCounterGetCoin();

    f32 getCoinBoostRate() const;
    void countUp();
    void update();

private:
    s32 mTimer = 0;
    s32 mDuration = 0;
};
