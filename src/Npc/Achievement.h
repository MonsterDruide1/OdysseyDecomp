#pragma once

#include "System/GameDataHolderAccessor.h"

struct AchievementInfo;

class Achievement {
public:
    Achievement(const AchievementInfo* info);
    bool isGet(GameDataHolderAccessor accessor) const;

private:
    bool mIsGet = false;
    const AchievementInfo* mInfo;
};
