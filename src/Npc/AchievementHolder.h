#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>

#include "System/GameDataHolderAccessor.h"

namespace al {
struct ActorInitInfo;
}
class Achievement;
class AchievementInfoReader;

class AchievementHolder {
public:
    AchievementHolder();
    void init(const al::ActorInitInfo&);
    void init();
    bool isGetMoon(s32 index, GameDataHolderAccessor accessor) const;
    bool isAchieve(s32 index, GameDataHolderAccessor accessor) const;
    s32 getAchievementProgressCurrent(s32 index, GameDataHolderAccessor accessor) const;
    s32 getAchievementProgressMax(s32 index, GameDataHolderAccessor) const;
    s32 getAchievementProgressCurrentRow(s32 index, GameDataHolderAccessor accessor) const;
    s32 calcAchieveTotalNum(GameDataHolderAccessor accessor) const;
    s32 calcMoonGetTotalNum(GameDataHolderAccessor accessor) const;
    Achievement* tryGetNewAchievement(GameDataHolderAccessor accessor) const;

private:
    sead::PtrArray<Achievement> mArray;
    AchievementInfoReader* mAchievementInfoReader = nullptr;
};
