#include "Player/PlayerCounterGetCoin.h"

#include "Library/Math/MathUtil.h"

PlayerCounterGetCoin::PlayerCounterGetCoin() = default;

f32 PlayerCounterGetCoin::getCoinBoostRate() const {
    return sead::Mathf::clampMin(mTimer / 90.0f, 0.0f);
}

void PlayerCounterGetCoin::countUp() {
    s32 newTime = mTimer + 90;
    if (newTime > 720)
        newTime = 720;
    mTimer = newTime;
    mDuration = 90;
}

void PlayerCounterGetCoin::update() {
    mTimer = al::converge(mTimer, 0, 1);
    mDuration = al::converge(mDuration, 0, 1);
    if (!mDuration)
        mTimer = 0;
}
