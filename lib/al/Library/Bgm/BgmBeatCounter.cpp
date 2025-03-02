#include "Library/Bgm/BgmBeatCounter.h"

#include "Library/Bgm/BgmLineFunction.h"

namespace al {

BgmBeatCounter::BgmBeatCounter(IUseAudioKeeper* audioKeeper, f32 threshold)
    : mAudioKeeper(audioKeeper), mThreshold(threshold) {}

void BgmBeatCounter::update() {
    if (!isEnableRhythmAnim(mAudioKeeper, nullptr))
        return;

    f32 currentBeat = getCurBeatOnMeasure(mAudioKeeper);
    if (currentBeat - (s32)currentBeat > mThreshold + 1.0f &&
        mCurrentBeat - (s32)currentBeat <= mThreshold + 1.0f) {
        mIsAboveThreshold = true;
    } else {
        mIsAboveThreshold = false;
    }

    mTriggerBeatValue = getCurBeat(mAudioKeeper) - mThreshold;
    mCurrentBeat = currentBeat;
}

bool BgmBeatCounter::isTriggerBeat(s32 interval, s32 delayTime) const {
    if (!mIsAboveThreshold)
        return false;

    s32 beatValue = mTriggerBeatValue + (mTriggerBeatValue >= 0.0f ? 0.5f : -0.5f);
    return beatValue - (s32)(beatValue / interval) * interval == delayTime;
}

}  // namespace al
