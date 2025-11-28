#pragma once

#include <basis/seadTypes.h>

class CapMessageBossData {
public:
    CapMessageBossData();

    void init();
    bool invalidateMessage(s32);
    bool isValidateMessage(s32) const;
    void incrementBossBattleCount(s32);
    s32 getBattleCount(s32) const;

private:
    char filler[0x10];
};

static_assert(sizeof(CapMessageBossData) == 0x10);
