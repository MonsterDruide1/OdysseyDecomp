#pragma once

#include "al/resource/GameDataHolderAccessor.h"

class AchievementInfo;

class Achievement {
public:
    Achievement(const AchievementInfo*);
    bool isGet(GameDataHolderAccessor) const;

private:
    bool mGet = false;
    const AchievementInfo* mInfo;
};
