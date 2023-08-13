#include "Npc/Achievement.h"

#include "Npc/AchievementInfoReader.h"
#include "Util/ResourceUtil.h"

Achievement::Achievement(const AchievementInfo* info) : mInfo(info) {}

bool Achievement::isGet(GameDataHolderAccessor accessor) const {
    return mGet || rs::checkGetAchievement(accessor, mInfo->mName);
}
