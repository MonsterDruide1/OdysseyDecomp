#include "Player/HackCapJudgePreInputSeparateJump.h"

#include "Library/Math/MathUtil.h"
#include "PlayerUtil.h"

HackCapJudgePreInputSeparateJump::HackCapJudgePreInputSeparateJump(
    const PlayerInput* input, const PlayerSeparateCapFlag* separateCapFlag) : mInput(input), mSeparateCapFlag(separateCapFlag) {}

void HackCapJudgePreInputSeparateJump::reset() {
    mCounter = 0;
}

void HackCapJudgePreInputSeparateJump::update() {
    if (mSeparateCapFlag->someCheck()) {
        reset();
        return;
    }

    mCounter = al::converge(mCounter, 0, 1);
    if(mInput->isTriggerCapSeparateJump())
        mCounter = 5;
}

bool HackCapJudgePreInputSeparateJump::judge() const {
    if(mSeparateCapFlag->someCheck())
        return false;

    return mInput->isTriggerCapSeparateJump() || mCounter > 0;
}
