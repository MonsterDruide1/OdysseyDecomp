#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
struct ActorInitInfo;
class LayoutActor;
class SimpleLayoutAppearWaitEnd;
}  // namespace al

class DecideIconLayout;
class StageSceneStateTimeBalloon;
class TimeBalloonDistanceLayout;
class TimeBalloonNpc;

class TimeBalloonStateFindBalloonFailed : public al::HostStateBase<StageSceneStateTimeBalloon> {
public:
    TimeBalloonStateFindBalloonFailed(StageSceneStateTimeBalloon* timeBalloon, TimeBalloonNpc* npc,
                                      al::SimpleLayoutAppearWaitEnd* failedLayout,
                                      al::LayoutActor* actionLayout,
                                      al::SimpleLayoutAppearWaitEnd* guideLayout,
                                      const al::ActorInitInfo& info,
                                      TimeBalloonDistanceLayout* distanceLayout);

    void init() override;
    void control() override;

    void exeFailedWait();
    void exeFailedEnd();
    void exeViewAroundStageDefaultCamera();
    void exeViewAroundStageDecideWait();
    void exeViewAroundStageEnd();

    bool isNerveFailedEnd() const;
    bool isNerveViewAroundStageEnd() const;

    void setNerveFailedWait();
    void setNerveViewAroundStage();

private:
    TimeBalloonNpc* mNpc = nullptr;
    al::SimpleLayoutAppearWaitEnd* mFailedLayout = nullptr;
    al::LayoutActor* mActionLayout = nullptr;
    s32 _38 = 0;
    u8 _3c[4];
    al::SimpleLayoutAppearWaitEnd* mGuideLayout = nullptr;
    TimeBalloonDistanceLayout* mDistanceLayout = nullptr;
    DecideIconLayout* mDecideIconLayout = nullptr;
};

static_assert(sizeof(TimeBalloonStateFindBalloonFailed) == 0x58);
