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

    void setSeKeeper(SeKeeper*);
    SeKeeper* getSeKeeper() const;

    const char* getCurPlayingSeName() const;
    s32 getCurPlayingSeStartFrame() const;

private:
    AudioDirector* mAudioDirector;
    SeDemoEventController* mEventController;
    AudioInfoListWithParts<SeDemoSyncedProcInfo>* mDemoSyncedProcInfos;
    SeDemoSyncedProcInfo* mCurrentProcInfo;
};

static_assert(sizeof(DemoSyncedSeCtrl) == 0x20);
}  // namespace al
