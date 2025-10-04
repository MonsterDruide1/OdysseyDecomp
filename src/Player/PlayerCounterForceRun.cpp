#include "Player/PlayerCounterForceRun.h"

#include "Library/Math/MathUtil.h"

PlayerCounterForceRun::PlayerCounterForceRun() = default;

void PlayerCounterForceRun::setupForceRun(s32 frames, f32 speed) {
    mCounter = frames;
    mSpeed = speed;
}

void PlayerCounterForceRun::update() {
    mCounter = al::converge(mCounter, 0, 1);
}
