#include "al/resource/Achievement.h"

#include "al/resource/AchievementInfoReader.h"
#include "al/util/ResourceUtil.h"

Achievement::Achievement(const AchievementInfo* info) : mInfo(info) {}

bool Achievement::isGet(GameDataHolderAccessor accessor) const {
    return mGet || rs::checkGetAchievement(accessor, mInfo->mName);
}
