#pragma once

#include <basis/seadTypes.h>

namespace al {

enum class alLanguage : s32 {
    JPja,
    USen,
    USes,
    USfr,
    EUen,
    EUes,
    EUfr,
    EUde,
    EUit,
    EUpt,
    EUnl,
    EUru,
    KRko,
    CNzh,
    TWzh,
};

void initRegionAndLanguage();
void forceInitLanguage(alLanguage language);
const char* getLanguageString();
void forceInitLanguage(const char* languageString);
bool isLanguageEmQuad();
alLanguage getLanguageCode();

}  // namespace al
