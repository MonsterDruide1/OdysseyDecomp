#include "Player/HackCapJudgePreInputSeparateThrow.h"

HackCapJudgePreInputSeparateThrow::HackCapJudgePreInputSeparateThrow(HackCap* hackCap)
    : mHackCap(hackCap), mPreInputFrames(0), mIsPreInput(false) {}

void HackCapJudgePreInputSeparateThrow::update() {
    // NON_MATCHING
}

bool HackCapJudgePreInputSeparateThrow::judge() const {
    return mIsPreInput;
}

bool HackCapJudgePreInputSeparateThrow::isEnablePreInput() const {
    return mIsPreInput;
}
