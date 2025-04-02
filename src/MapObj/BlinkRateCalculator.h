#pragma once

#include <basis/seadTypes.h>

class BlinkRateCalculator {
public:
    BlinkRateCalculator(s32 maxFrames);
    BlinkRateCalculator();
    void reset();
    void update();

private:
    s32 mMaxFrames = 50;
    s32 mCurrentFrame = 0;
    f32 mCurrentRate = 0.0f;
    f32 mRateThreshold = 0.498f;
};
