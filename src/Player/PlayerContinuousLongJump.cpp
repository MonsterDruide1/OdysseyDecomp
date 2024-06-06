#include "Player/PlayerContinuousLongJump.h"

#include "Player/PlayerConst.h"

PlayerContinuousLongJump::PlayerContinuousLongJump(const PlayerConst* pConst) : mConst(pConst) {}

void PlayerContinuousLongJump::countUp() {
    mCount = (mCount + 1) % mConst->getContinuousLongJumpCount();
    mTimer = 0;
}

void PlayerContinuousLongJump::update() {
    if (mCount != 0) {
        mTimer++;
        if (mTimer >= mConst->getContinuousLongJumpTimer()) {
            mCount = 0;
            mTimer = 0;
        }
    }
}

constexpr const char* longJumpAnimNames[] = {"JumpBroad",  "JumpBroad2", "JumpBroad3",
                                             "JumpBroad4", "JumpBroad5", "JumpBroad6",
                                             "JumpBroad7", "JumpBroad8"};

const char* PlayerContinuousLongJump::getLongJumpAnimName() const {
    if (mCount >= 0 && mCount <= 7)
        return longJumpAnimNames[mCount];
    return nullptr;
}
