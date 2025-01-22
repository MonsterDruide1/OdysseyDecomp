#include "MapObj/BlinkRateCalculator.h"

#include "Library/Math/MathUtil.h"

BlinkRateCalculator::BlinkRateCalculator(s32 maxFrames) : mMaxFrames(maxFrames) {}

BlinkRateCalculator::BlinkRateCalculator() {}

void BlinkRateCalculator::reset() {
    mCurrentFrame = 0;
    mCurrentRate = 1.0f;
}

void BlinkRateCalculator::update() {
    if (mCurrentFrame < mMaxFrames)
        mCurrentFrame = ++mCurrentFrame;

    if (mMaxFrames - mCurrentFrame > 150) {
        mCurrentRate = 1.0f;
        return;
    }

    s32 interval = mCurrentFrame % 50;

    if (interval < 5) {
        mCurrentRate = al::lerpValue(1.0f, mRateThreshold, interval / 5.0f);
        return;
    }
    if (interval < 20) {
        mCurrentRate = mRateThreshold;
        return;
    }
    if (interval < 25) {
        mCurrentRate = al::lerpValue(mRateThreshold, 1.0f, (interval - 20) / 5.0f);
        return;
    }

    mCurrentRate = 1.0f;
}
