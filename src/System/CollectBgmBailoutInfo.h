#pragma once

#include <basis/seadTypes.h>

struct CollectBgmBailoutInfo {
    CollectBgmBailoutInfo();
    CollectBgmBailoutInfo(const char* name1, const char* situation_name1, const char* name2,
                          const char* situation_name2);
    const char* name1;
    const char* situationName1;
    const char* name2;
    const char* situationName2;
};

static_assert(sizeof(CollectBgmBailoutInfo) == 0x20);

extern const s32 cCollectBgmBailoutInfoTableSize;
extern CollectBgmBailoutInfo cCollectBgmBailoutInfoTable[];
