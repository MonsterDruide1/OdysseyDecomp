#include "Library/Math/IntervalTrigger.h"

#include "Library/Math/MathUtil.h"

namespace al {
IntervalTrigger::IntervalTrigger(f32 intervalTime) : mIntervalTime{intervalTime} {};

void IntervalTrigger::update(f32 deltaTime) {
    mIsTriggered = false;
    mTime += deltaTime;

    if (mTime >= mIntervalTime) {
        mIsTriggered = true;
        mTime = modf(mTime, mIntervalTime);
    }

    if (mTime < 0.0f) {
        mIsTriggered = true;
        mTime += ((s32)(-mTime / mIntervalTime) + 1) * mIntervalTime;
    }
}
}  // namespace al
