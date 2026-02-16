#include "ModeBalloon/RankingLocalFunction.h"

#include <prim/seadStringUtil.h>

#include "Library/Base/StringUtil.h"

#include "Util/SequentialUtil.h"

namespace RankingLocalFunction {
bool isValidCourceIdYukimaruRace(s32 courseId) {
    return courseId == 0 || courseId == 1;
}

RankingCategory getRankingCategoryYukimaruRace(s32 courseId) {
    RankingCategory yukimaruCourseIdToCategory[2]{
        RankingCategory::Yukimaru_1,
        RankingCategory::Yukimaru_2,
    };

    return yukimaruCourseIdToCategory[courseId];
}

bool isValidWorldIdRaceManRace(s32 worldId) {
    // NOTE: Cloud (5), Ruined (11), Dark (15) and Darker Side (16) do not have a KFR
    return worldId >= 0 && worldId != 5 && worldId != 11 && (u32)worldId < 15;
}

RankingCategory getRankingCategoryRaceManRace(s32 worldId) {
    switch (worldId) {
    case 0:
        return RankingCategory::RaceManRace_1;
    case 1:
        return RankingCategory::RaceManRace_2;
    case 2:
        return RankingCategory::RaceManRace_3;
    case 3:
        return RankingCategory::RaceManRace_4;
    case 4:
        return RankingCategory::RaceManRace_5;
    case 6:
        return RankingCategory::RaceManRace_6;
    case 7:
        return RankingCategory::RaceManRace_7;
    case 8:
        return RankingCategory::RaceManRace_8;
    case 9:
        return RankingCategory::RaceManRace_9;
    case 10:
        return RankingCategory::RaceManRace_10;
    case 12:
        return RankingCategory::RaceManRace_11;
    case 13:
        return RankingCategory::RaceManRace_12;
    case 14:
        return RankingCategory::RaceManRace_13;

    default:
        return RankingCategory::RaceManRace_1;
    }
}

bool isRankingCategoryYukimaruRace(RankingCategory category) {
    return category == RankingCategory::Yukimaru_1 || category == RankingCategory::Yukimaru_2;
}

bool isRankingCategoryRaceManRace(RankingCategory category) {
    return category >= RankingCategory::RaceManRace_1 &&
           category <= RankingCategory::RaceManRace_13;
}

s32 getRaceManRaceWorldId(RankingCategory category) {
    switch (category) {
    case RankingCategory::RaceManRace_1:
        return 0;
    case RankingCategory::RaceManRace_2:
        return 1;
    case RankingCategory::RaceManRace_3:
        return 2;
    case RankingCategory::RaceManRace_4:
        return 3;
    case RankingCategory::RaceManRace_5:
        return 4;
    case RankingCategory::RaceManRace_6:
        return 6;
    case RankingCategory::RaceManRace_7:
        return 7;
    case RankingCategory::RaceManRace_8:
        return 8;
    case RankingCategory::RaceManRace_9:
        return 9;
    case RankingCategory::RaceManRace_10:
        return 10;
    case RankingCategory::RaceManRace_11:
        return 12;
    case RankingCategory::RaceManRace_12:
        return 13;
    case RankingCategory::RaceManRace_13:
        return 14;

    default:
        return -1;
    }
}

bool isRankingCategoryRace(RankingCategory category) {
    return isRankingCategoryYukimaruRace(category) || isRankingCategoryRaceManRace(category) ||
           category == RankingCategory::Radicon;
}

RankingCategory findRankingCategoryByRaceRecordName(const char* name) {
    if (al::isEqualString(name, "Yukimaru_1"))
        return RankingCategory::Yukimaru_1;
    if (al::isEqualString(name, "Yukimaru_2"))
        return RankingCategory::Yukimaru_2;
    if (al::isEqualString(name, "Radicon"))
        return RankingCategory::Radicon;

    if (al::isEqualSubString(name, "RaceManRace")) {
        const char* worldIdStr = al::searchSubString(name, "_") + 1;
        s32 worldId = 0;
        sead::StringUtil::tryParseS32(&worldId, worldIdStr,
                                      sead::StringUtil::CardinalNumber::BaseAuto);
        return getRankingCategoryRaceManRace(worldId);
    }

    return RankingCategory::Yukimaru_1;
}

bool isRankingUploadableScore(u32 score, RankingCategory category) {
    if (category == RankingCategory::JumpingRope)
        return score != 0;

    if (category == RankingCategory::Volleyball)
        return score != 0;

    if (isRankingCategoryRace(category))
        return score <= RaceTimeFunction::getRaceTimeMaxCsec();

    return true;
}
}  // namespace RankingLocalFunction
