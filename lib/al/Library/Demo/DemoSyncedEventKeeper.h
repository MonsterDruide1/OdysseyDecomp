#pragma once

#include <basis/seadTypes.h>

namespace al {
class AudioDirector;
class DemoSyncedBgmCtrl;
class DemoSyncedSeCtrl;

class DemoSyncedEventKeeper {
public:
    DemoSyncedEventKeeper(AudioDirector*);

    void updateOnlyWhenDemoRun(s32);
    void finalize();
    void startDemo(const char* demoName);
    void endDemo(bool, bool);

    DemoSyncedSeCtrl* getDemoSyncedSeCtrl() const { return mDemoSyncedSeCtrl; }

private:
    DemoSyncedBgmCtrl* mDemoSyncedBgmCtrl = nullptr;
    DemoSyncedSeCtrl* mDemoSyncedSeCtrl = nullptr;
    AudioDirector* mAudioDirector = nullptr;
    const char* mDemoName = nullptr;
};
}  // namespace al

static_assert(sizeof(al::DemoSyncedEventKeeper) == 0x20, "DemoSyncedEventKeeper size");

namespace alDemoSyncedEventFunction {
void tryCreateAndSetSeKeeper(al::DemoSyncedEventKeeper* keeper, al::AudioDirector* audioDirector,
                             const char* seName);
}
