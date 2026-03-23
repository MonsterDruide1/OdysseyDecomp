#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LayoutInitInfo;
class SimpleLayoutAppearWaitEnd;
}  // namespace al

class IBalloonFindMyAchievementHolder;
class StageSceneStateTimeBalloon;
class TimeBalloonAchievementLayout;

class TimeBalloonStateShowResult : public al::HostStateBase<StageSceneStateTimeBalloon> {
public:
    TimeBalloonStateShowResult(StageSceneStateTimeBalloon* timeBalloon,
                               al::SimpleLayoutAppearWaitEnd* layout,
                               const IBalloonFindMyAchievementHolder* achievementHolder,
                               const al::LayoutInitInfo& info);

    void init() override;
    void appear() override;

    void exeAppear();
    void exeWait();
    void exeEnd();

    void setNerveWait();

private:
    StageSceneStateTimeBalloon* mTimeBalloon = nullptr;
    const IBalloonFindMyAchievementHolder* mAchievementHolder = nullptr;
    TimeBalloonAchievementLayout* mAchievementLayout = nullptr;
};
