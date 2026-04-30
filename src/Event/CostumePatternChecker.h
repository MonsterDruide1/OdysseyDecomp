#pragma once

#include <basis/seadTypes.h>

class CostumePatternChecker {
public:
    CostumePatternChecker();

    u32 checkMatchCostume(bool* isCapMatch, bool* isClothMatch, const char* currentCap,
                          const char* currentCloth, const char* pattern) const;
    void* tryFindPattern(const char* pattern) const;
    bool checkMatchCostumePair(const char* currentCap, const char* currentCloth,
                               const char* pattern) const;

private:
    void* mPatterns;
    s32 mPatternNum;
};

static_assert(sizeof(CostumePatternChecker) == 0x10);
