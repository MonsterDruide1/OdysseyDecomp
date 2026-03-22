#pragma once

#include <basis/seadTypes.h>

#include "Library/Audio/AudioInfo.h"
#include "Library/Audio/IUseAudioKeeper.h"
#include "Library/HostIO/HioNode.h"

namespace al {
class AudioDirector;
class BgmDataBase;
template <typename, typename>
class BgmEventController;
class BgmDemoProcInfo;
class BgmDemoSyncedProcInfo;

class DemoSyncedBgmCtrl : public HioNode, public IUseAudioKeeper {
public:
    static DemoSyncedBgmCtrl* tryCreate(AudioDirector*);
    DemoSyncedBgmCtrl(AudioDirector* audioDirector);

    void update(s32);
    void finalize();
    void startDemo(const char* demoName);
    void endDemo(const char* demoName, bool);
    void notifyDemoSkip();

    AudioKeeper* getAudioKeeper() const override { return mAudioKeeper; }

private:
    AudioDirector* mAudioDirector;
    AudioKeeper* mAudioKeeper;
    BgmEventController<IUseAudioKeeper, BgmDemoProcInfo>* mBgmEventController;
    AudioInfoListWithParts<BgmDemoSyncedProcInfo>* mDemoSyncedProcInfos;
    BgmDemoSyncedProcInfo* mCurrentProcInfo;
};

static_assert(sizeof(DemoSyncedBgmCtrl) == 0x30);
}  // namespace al
