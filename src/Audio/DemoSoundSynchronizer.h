#pragma once

#include <basis/seadTypes.h>

namespace al {
class ApplicationMessageReceiver;
class AudioDirector;
class DemoSyncedEventKeeper;
}  // namespace al

class DemoSoundSynchronizer {
public:
    DemoSoundSynchronizer(const al::ApplicationMessageReceiver* receiver,
                          al::AudioDirector* audioDirector);

    void startSync();
    void endSync();
    void resume(s32 delay);
    void update();
    void tryResume();
    void trySync(s32 currentTime, al::DemoSyncedEventKeeper* syncedEventKeeper);
    void trySyncCommon(s32 currentTime, s32 delayFrames, s64 sample);
    void trySync(s32 currentTime, s32 delayFrames, s64 sample);
    void pause();

private:
    const al::ApplicationMessageReceiver* mApplicationMessageReceiver = nullptr;
    al::AudioDirector* mAudioDirector = nullptr;
    bool mIsHandheld = false;
    bool mIsSyncActive = false;
    s32 mResumeDelay = -1;
    bool mIsPaused = false;
};

static_assert(sizeof(DemoSoundSynchronizer) == 0x20, "DemoSoundSynchronizer size");
