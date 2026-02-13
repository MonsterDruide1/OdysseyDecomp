#pragma once

#include <basis/seadTypes.h>
#include <container/seadObjArray.h>

#include "ModeBalloon/RankingCategory.h"

namespace RankingLocalFunction {
bool isValidCourceIdYukimaruRace(s32);
RankingCategory getRankingCategoryYukimaruRace(s32);
bool isValidWorldIdRaceManRace(s32);
RankingCategory getRankingCategoryRaceManRace(s32);
bool isRankingCategoryYukimaruRace(RankingCategory);
bool isRankingCategoryRaceManRace(RankingCategory);
s32 getRaceManRaceWorldId(RankingCategory);
bool isRankingCategoryRace(RankingCategory);
bool tryGetRankingCategoryIndex(s32*, RankingCategory, const sead::ObjArray<RankingCategory>&);
bool isExistRankingCategory(RankingCategory, const sead::ObjArray<RankingCategory>&);
RankingCategory findRankingCategoryByRaceRecordName(const char*);
bool isRankingUploadableScore(u32, RankingCategory);
}  // namespace RankingLocalFunction

struct RankingDataOneRecord {
    void clear();

    void* filler_0[14];
};

static_assert(sizeof(RankingDataOneRecord) == 0x70);

class RankingDataOneCategory {
public:
    RankingDataOneCategory(RankingCategory, s32);
    void clear();
    void setData(s32, u64, u32, u32, const char16*);

private:
    RankingCategory _0;
    void* filler_8[3];
};

static_assert(sizeof(RankingDataOneCategory) == 0x20);
