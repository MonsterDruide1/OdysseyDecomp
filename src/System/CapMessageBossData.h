#pragma once

#include <basis/seadTypes.h>

class CapMessageBossData {
public:
    CapMessageBossData();
    void init();
    void invalidateMessage(s32 worldId);
    bool isValidateMessage(s32 worldId) const;
    void incrementBossBattleCount(s32 worldId);
    s32 getBattleCount(s32 worldId) const;

private:
    bool* mMessageFlags;
    s32* mBattleCounts;
};

static_assert(sizeof(CapMessageBossData) == 0x10);
