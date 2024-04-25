#pragma once

#include "System/GameDataHolderAccessor.h"

class AchievementInfo;

class Achievement {
public:
    Achievement(const AchievementInfo*);
    bool isGet(GameDataHolderAccessor) const;

private:
    bool mIsGet = false;
    const AchievementInfo* mInfo;
};
