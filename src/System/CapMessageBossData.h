#pragma once

#include <basis/seadTypes.h>

class CapMessageBossData {
public:
    CapMessageBossData();
    void init();
    void invalidateMessage(s32 flag);
    bool isValidateMessage(s32 flag) const;
    void incrementBossBattleCount(s32 bossType);
    s32 getBattleCount(s32 bossType) const;

private:
    bool* mMessageFlag;
    s32* mBattleCount;
};

static_assert(sizeof(CapMessageBossData) == 0x10);
