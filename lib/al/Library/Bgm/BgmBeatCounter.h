#pragma once

#include <basis/seadTypes.h>

namespace al {
class IUseAudioKeeper;

class BgmBeatCounter {
public:
    BgmBeatCounter(IUseAudioKeeper* audioKeeper, f32 threshold);
    void update();
    bool isTriggerBeat(s32 interval, s32 delayTime) const;

    bool isAboveThreshold() const { return mIsAboveThreshold; }

private:
    IUseAudioKeeper* mAudioKeeper = nullptr;
    f32 mThreshold = 0.0f;
    f32 mTriggerBeatValue = 0.0f;
    f32 mCurrentBeat = 0.0f;
    bool mIsAboveThreshold = false;
};

}  // namespace al
