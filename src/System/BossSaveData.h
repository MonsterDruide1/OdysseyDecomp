#pragma once

#include <basis/seadTypes.h>

#include "System/ByamlSave.h"

class BossSaveData : public ByamlSave {
public:
    BossSaveData();
    void init();
    bool isAlreadyShowDemoBossBattleStart(s32, s32) const;
    void showDemoBossBattleStart(s32, s32);
    bool isAlreadyDeadGK(s32, s32) const;
    void onAlreadyDeadGK(s32, s32);
    bool isAlreadyShowDemoBattleEndKoopaLv2() const;
    void saveDemoBattleEndKoopaLv2();
    void resetLv3Data();
    void write(al::ByamlWriter* writer) override;
    void read(const al::ByamlIter& save) override;

private:
    void* _padding[0x6];
};

static_assert(sizeof(BossSaveData) == 0x38);
