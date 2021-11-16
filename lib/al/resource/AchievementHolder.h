#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>
#include "al/resource/GameDataHolderAccessor.h"

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
    bool isGetMoon(int, GameDataHolderAccessor) const;
    bool isAchieve(int, GameDataHolderAccessor) const;
    int getAchievementProgressCurrent(int, GameDataHolderAccessor) const;
    int getAchievementProgressMax(int, GameDataHolderAccessor) const;
    int getAchievementProgressCurrentRow(int, GameDataHolderAccessor) const;
    int calcAchieveTotalNum(GameDataHolderAccessor) const;
    int calcMoonGetTotalNum(GameDataHolderAccessor) const;
    Achievement* tryGetNewAchievement(GameDataHolderAccessor) const;

private:
    sead::PtrArray<Achievement> mArray;
    AchievementInfoReader* mAchievementInfoReader = nullptr;
};
