#include "Library/Movement/FlashingTimer.h"

#include <math/seadMathCalcCommon.h>

namespace al {
FlashingTimer::FlashingTimer(s32 param_1, s32 startHurry, s32 blinkVisibleFrames,
                             s32 hurryBlinkVisibleFrames)
    : _4(param_1), mStartHurry(startHurry), mBlinkVisibleFrames(blinkVisibleFrames),
      mHurryBlinkVisibleFrames(hurryBlinkVisibleFrames) {}

void FlashingTimer::start(s32 param_1, s32 startHurry, s32 blinkVisibleFrames,
                          s32 hurryBlinkVisibleFrames) {
    mLastTime = param_1;
    _4 = param_1;
    mStartHurry = startHurry;
    mBlinkVisibleFrames = blinkVisibleFrames;
    mHurryBlinkVisibleFrames = hurryBlinkVisibleFrames;
    mIsVisible = false;
    mIsPrevVisible = false;
}

void FlashingTimer::update() {
    mIsPrevVisible = mIsVisible;

    if (mLastTime < 0) {
        mIsVisible = false;

        return;
    }

    mLastTime--;

    if (isHurryStart()) {
        if (((mLastTime / mHurryBlinkVisibleFrames) % 2) != 0) {
            mIsVisible = true;

            return;
        }
    } else if ((((mLastTime - mStartHurry) / mBlinkVisibleFrames) % 2) != 0) {
        mIsVisible = true;

        return;
    }

    mIsVisible = false;
}

bool FlashingTimer::isHurryStart() const {
    return mLastTime < mStartHurry;
}

s32 FlashingTimer::getLastTime() const {
    return sead::Mathi::clampMin(mLastTime, 0);
}
}  // namespace al
