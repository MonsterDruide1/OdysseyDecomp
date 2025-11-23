#pragma once

enum alLanguage {
    alLanguage_JPja,
    alLanguage_USen,
    alLanguage_USes,
    alLanguage_USfr,
    alLanguage_EUen,
    alLanguage_EUes,
    alLanguage_EUfr,
    alLanguage_EUde,
    alLanguage_EUit,
    alLanguage_EUpt,
    alLanguage_EUnl,
    alLanguage_EUru,
    alLanguage_KRko,
    alLanguage_CNzh,
    alLanguage_TWzh,
};

namespace al {
void initRegionAndLanguage();
void forceInitLanguage(alLanguage language);
const char* getLanguageString();
void forceInitLanguage(const char* language);
bool isLanguageEmQuad();
alLanguage getLanguageCode();
}  // namespace al
