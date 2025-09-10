#pragma once

class CollectBgm {
public:
    inline static const char* const cSituationNameList[9] = {
        "BossForestBattle3rd",
        "BossGolemBattle3rd",
        "CollectBgmGiantWanderBoss",
        "CollectBgmGiantWanderBoss8Bit",
        "CollectBgmWorldMap",
        "In2DArea",
        "InWater",
        "MuteOffPaulineVocal",
        "TankZone",
    };

    inline static const char* cCeremonyCollectBgmName = "StmRsBgmCityScenario03Clct";
    inline static const char* cBonusCollectBgmName01 = "StmRsBgmEndRockSpecial";
    inline static const char* cBonusCollectBgmName02 = "StmRsBgmCityScenario03Special";
    inline static const char* cBonusCollectBgmSituationName02 = "MuteOffPaulineVocal";

    CollectBgm();
    CollectBgm(const char*, const char*, const char*, const char*, const char*);
    bool isEqualRequest(const char*, const char*, const char*);

private:
    const char* _0;
    const char* _8;
    const char* _10;
    const char* _18;
    const char* _20;
};

static_assert(sizeof(CollectBgm) == 0x28);
