#include "Library/Bgm/BgmBeatCounter.h"

#include "Library/Bgm/BgmLineFunction.h"

namespace al {

BgmBeatCounter::BgmBeatCounter(IUseAudioKeeper* audioKeeper, f32 triggerBeatOffset)
    : mAudioKeeper(audioKeeper), mTriggerBeatOffset(triggerBeatOffset) {}

void BgmBeatCounter::update() {
    if (!isEnableRhythmAnim(mAudioKeeper, nullptr))
        return;

    f32 currentBeat = getCurBeatOnMeasure(mAudioKeeper);
    if (currentBeat - (s32)currentBeat > mTriggerBeatOffset + 1.0f &&
        mPrevBeat - (s32)currentBeat <= mTriggerBeatOffset + 1.0f)
        mIsOnBeat = true;
    else
        mIsOnBeat = false;

    mCurBeatWithOffset = getCurBeat(mAudioKeeper) - mTriggerBeatOffset;
    mPrevBeat = currentBeat;
}

bool BgmBeatCounter::isTriggerBeat(s32 interval, s32 delayTime) const {
    if (!mIsOnBeat)
        return false;

    s32 beatValue = mCurBeatWithOffset + (mCurBeatWithOffset >= 0.0f ? 0.5f : -0.5f);
    return beatValue % interval == delayTime;
}

}  // namespace al
