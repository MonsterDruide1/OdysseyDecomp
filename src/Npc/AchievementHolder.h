#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>
#include "System/GameDataHolderAccessor.h"

namespace al {
class ActorInitInfo;
}
class Achievement;
class AchievementInfoReader;

class AchievementHolder {
public:
    AchievementHolder();
    void init(const al::ActorInitInfo&);
    void init();
    bool isGetMoon(s32, GameDataHolderAccessor) const;
    bool isAchieve(s32, GameDataHolderAccessor) const;
    s32 getAchievementProgressCurrent(s32, GameDataHolderAccessor) const;
    s32 getAchievementProgressMax(s32, GameDataHolderAccessor) const;
    s32 getAchievementProgressCurrentRow(s32, GameDataHolderAccessor) const;
    s32 calcAchieveTotalNum(GameDataHolderAccessor) const;
    s32 calcMoonGetTotalNum(GameDataHolderAccessor) const;
    Achievement* tryGetNewAchievement(GameDataHolderAccessor) const;

private:
    sead::PtrArray<Achievement> mArray;
    AchievementInfoReader* mAchievementInfoReader = nullptr;
};
