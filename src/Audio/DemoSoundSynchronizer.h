#pragma once

#include <basis/seadTypes.h>

namespace al {
class ApplicationMessageReceiver;
class AudioDirector;
class DemoSyncedEventKeeper;
}  // namespace al

class DemoSoundSynchronizer {
public:
    DemoSoundSynchronizer(const al::ApplicationMessageReceiver* receiver, al::AudioDirector*);

    void startSync();
    void endSync();
    void resume(s32 delay);
    void update();
    void tryResume();
    void trySync(s32 delay, al::DemoSyncedEventKeeper* syncedEventKeeper);
    void trySyncCommon(s32, s32, s64);
    void trySync(s32, s32, s64);
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
