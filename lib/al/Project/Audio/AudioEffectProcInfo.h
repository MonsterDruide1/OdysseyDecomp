#pragma once

//#include <basis/seadTypes.h>

namespace al {

struct AudioDemoSyncedProcInfo {
    AudioDemoSyncedProcInfo();

    const char* name = nullptr;
    bool isEnableEvent = false;
    bool isPartsProc = false;
    void* _10 = nullptr;
};

static_assert(sizeof(AudioDemoSyncedProcInfo) == 0x18);


}  // namespace al
