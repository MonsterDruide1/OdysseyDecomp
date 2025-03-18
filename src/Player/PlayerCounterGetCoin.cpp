#include "Player/PlayerCounterGetCoin.h"

#include <math/seadMathCalcCommon.h>

#include "Library/Math/MathUtil.h"

PlayerCounterGetCoin::PlayerCounterGetCoin() = default;

f32 PlayerCounterGetCoin::getCoinBoostRate() const {
    return sead::Mathf::clampMin(mTimer / 90.0f, 0.0f);
}

void PlayerCounterGetCoin::countUp() {
    mTimer = sead::Mathi::clampMax(mTimer + 90, 720);
    mDuration = 90;
}

void PlayerCounterGetCoin::update() {
    mTimer = al::converge(mTimer, 0, 1);
    mDuration = al::converge(mDuration, 0, 1);
    if (!mDuration)
        mTimer = 0;
}
