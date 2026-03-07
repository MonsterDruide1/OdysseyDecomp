#pragma once

#include <basis/seadTypes.h>

#include "System/ByamlSave.h"

class BossSaveData : public ByamlSave {
public:
    BossSaveData();
    void init();
    bool isAlreadyShowDemoBossBattleStart(s32 world, s32 lv) const;
    void showDemoBossBattleStart(s32 world, s32 lv);
    bool isAlreadyDeadGK(s32 world, s32 lv) const;
    void onAlreadyDeadGK(s32 world, s32 lv);
    bool isAlreadyShowDemoBattleEndKoopaLv2() const;
    void saveDemoBattleEndKoopaLv2();
    void resetLv3Data();
    void write(al::ByamlWriter* writer) override;
    void read(const al::ByamlIter& save) override;

private:
    static constexpr s32 sBossBattleStartWorldNum = 13;
    static constexpr s32 sDeadGKNum = 4;

    bool* mIsAlreadyShowDemoBossBattleStartLv1;
    bool* mIsAlreadyShowDemoBossBattleStartLv2;
    bool* mIsAlreadyDeadGKLv1;
    bool* mIsAlreadyDeadGKLv2;
    bool* mIsAlreadyDeadGKLv3;
    bool mIsAlreadyShowDemoBattleEndKoopaLv2 = false;
    bool mIsAlreadyShowDemoMoonBasementCollapse = false;
};

static_assert(sizeof(BossSaveData) == 0x38);
