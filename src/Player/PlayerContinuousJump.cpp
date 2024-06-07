#include "Player/PlayerContinuousJump.h"

#include "Player/PlayerConst.h"

PlayerContinuousJump::PlayerContinuousJump(const PlayerConst* pConst) : mConst(pConst) {}

void PlayerContinuousJump::update(bool shouldCountDown) {
    if (!shouldCountDown) {
        clear();
        return;
    }

    if (mCount > 0) {
        mTimer++;
        if (mTimer >= mConst->getContinuousJumpTimer()) {
            clear();
        }
    }
}

void PlayerContinuousJump::clear() {
    mLastJumpDir = {0.0f, 0.0f, 0.0f};
    mCount = 0;
    mTimer = mConst->getContinuousJumpTimer();
}

void PlayerContinuousJump::countUp(const sead::Vector3f& jumpDir) {
    mLastJumpDir = jumpDir;
    mCount = (mCount + 1) % mConst->getContinuousJumpCount();
    mTimer = 0;
}
