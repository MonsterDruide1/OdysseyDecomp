#include "game/Npc/Achievement.h"

#include "game/Npc/AchievementInfoReader.h"
#include "game/Util/ResourceUtil.h"

Achievement::Achievement(const AchievementInfo* info) : mInfo(info) {}

bool Achievement::isGet(GameDataHolderAccessor accessor) const {
    return mGet || rs::checkGetAchievement(accessor, mInfo->mName);
}
