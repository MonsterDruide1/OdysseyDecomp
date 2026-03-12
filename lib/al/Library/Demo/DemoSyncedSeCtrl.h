#pragma once

#include <basis/seadTypes.h>

#include "Library/Audio/AudioInfo.h"

namespace al {
class AudioDirector;
class SeKeeper;
class BgmDataBase;
class SeDemoEventController;
class SeDemoProcInfo;
class SeDemoSyncedProcInfo;

class DemoSyncedSeCtrl {
public:
    static DemoSyncedSeCtrl* tryCreate(AudioDirector*);
    DemoSyncedSeCtrl(AudioDirector* audioDirector);

    void update(s32);
    void finalize();
    void startDemo(const char* demoName);
    void endDemo(const char* demoName, bool);
    void notifyDemoSkip();

    const char* getCurPlayingSeName() const;
    s32 getCurPlayingSeStartFrame() const;

    void setSeKeeper(SeKeeper*);
    SeKeeper* getSeKeeper() const;

private:
    AudioDirector* mAudioDirector;
    SeDemoEventController* mEventController;
    AudioInfoListWithParts<SeDemoSyncedProcInfo>* mDemoSyncedProcInfos;
    SeDemoSyncedProcInfo* mCurrentProcInfo;
};
}  // namespace al

static_assert(sizeof(al::DemoSyncedSeCtrl) == 0x20, "DemoSyncedSeCtrl size");
