#pragma once

#include <basis/seadTypes.h>

struct CollectBgm {
    CollectBgm();
    CollectBgm(const char* name, const char* situation_name, const char* tag1, const char* tag2,
               const char* category);
    static bool isEqualRequest(const char* name, const char* situation_name, const char* tag);
    static const s32 cSituationNameListSize;
    static const char* cSituationNameList[];
    static const char* cCeremonyCollectBgmName;
    static const char* cBonusCollectBgmName01;
    static const char* cBonusCollectBgmName02;
    static const char* cBonusCollectBgmSituationName02;
    const char* name;
    const char* situationName;
    const char* tag1;
    const char* tag2;
    const char* category;
};

static_assert(sizeof(CollectBgm) == 0x28);

extern const s32 cCollectBgmListSize;
extern CollectBgm cCollectBgmList[];
