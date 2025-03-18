#include "Npc/Achievement.h"

#include "Npc/AchievementInfoReader.h"
#include "System/GameDataHolderAccessor.h"
#include "Util/AchievementUtil.h"

Achievement::Achievement(const AchievementInfo* info) : mInfo(info) {}

bool Achievement::isGet(GameDataHolderAccessor accessor) const {
    return mIsGet || rs::checkGetAchievement(accessor, mInfo->name);
}
