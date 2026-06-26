#include "Player/HackCapJudgePreInputSeparateJump.h"

HackCapJudgePreInputSeparateJump::HackCapJudgePreInputSeparateJump(HackCap* hackCap)
    : mHackCap(hackCap), mPreInputFrames(0), mIsPreInput(false) {}

void HackCapJudgePreInputSeparateJump::update() {
    // NON_MATCHING
}

bool HackCapJudgePreInputSeparateJump::judge() const {
    return mIsPreInput;
}
