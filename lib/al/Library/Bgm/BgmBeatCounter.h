#pragma once

namespace al {
class IUseAudioKeeper;

class BgmBeatCounter {
public:
    BgmBeatCounter(IUseAudioKeeper* audioKeeper, f32 triggerBeatOffset);
    void update();
    bool isTriggerBeat(s32 interval, s32 delayTime) const;

    bool isOnBeat() const { return mIsOnBeat; }

private:
    IUseAudioKeeper* mAudioKeeper = nullptr;
    f32 mTriggerBeatOffset = 0.0f;
    f32 mCurBeatWithOffset = 0.0f;
    f32 mPrevBeat = 0.0f;
    bool mIsOnBeat = false;
};

}  // namespace al
