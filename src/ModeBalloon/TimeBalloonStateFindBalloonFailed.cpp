#include "ModeBalloon/TimeBalloonStateFindBalloonFailed.h"

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Play/Layout/SimpleLayoutAppearWaitEnd.h"
#include "Library/Se/SeFunction.h"

#include "Layout/DecideIconLayout.h"
#include "ModeBalloon/TimeBalloonDistanceLayout.h"
#include "Npc/TimeBalloonNpc.h"

namespace {
NERVE_IMPL(TimeBalloonStateFindBalloonFailed, FailedWait)
NERVE_IMPL(TimeBalloonStateFindBalloonFailed, FailedEnd)
NERVE_IMPL(TimeBalloonStateFindBalloonFailed, ViewAroundStageDecideWait)
NERVE_IMPL(TimeBalloonStateFindBalloonFailed, ViewAroundStageEnd)
NERVE_IMPL(TimeBalloonStateFindBalloonFailed, ViewAroundStageDefaultCamera)

NERVES_MAKE_NOSTRUCT(TimeBalloonStateFindBalloonFailed, FailedWait, FailedEnd,
                     ViewAroundStageDecideWait, ViewAroundStageEnd, ViewAroundStageDefaultCamera)
}  // namespace

TimeBalloonStateFindBalloonFailed::TimeBalloonStateFindBalloonFailed(
    StageSceneStateTimeBalloon* timeBalloon, TimeBalloonNpc* npc,
    al::SimpleLayoutAppearWaitEnd* failedLayout, al::LayoutActor* actionLayout,
    al::SimpleLayoutAppearWaitEnd* guideLayout, const al::ActorInitInfo& info,
    TimeBalloonDistanceLayout* distanceLayout)
    : al::HostStateBase<StageSceneStateTimeBalloon>("探す状態(風船ゲーム)", timeBalloon), mNpc(npc),
      mFailedLayout(failedLayout), mActionLayout(actionLayout), mGuideLayout(guideLayout),
      mDistanceLayout(distanceLayout) {
    mDecideIconLayout =
        new DecideIconLayout("[風船ゲーム]決定アイコン", al::getLayoutInitInfo(info));
}

void TimeBalloonStateFindBalloonFailed::init() {
    initNerve(&FailedWait, 0);
}

void TimeBalloonStateFindBalloonFailed::control() {
    mDecideIconLayout->updateNerve();
}

void TimeBalloonStateFindBalloonFailed::exeFailedWait() {
    if (al::isFirstStep(this)) {
        mDistanceLayout->tryEnd();
        TimeBalloon::appearAndStartAction(mActionLayout, "TimeUp");
        al::startSe(mNpc, "SysTimeUp2");
    }

    if (al::isActionEnd(mActionLayout, nullptr)) {
        mFailedLayout->end();
        TimeBalloon::appearGuideLayout(mGuideLayout, "TimeBalloonNpc");
        al::setNerve(this, &FailedEnd);
    }
}

void TimeBalloonStateFindBalloonFailed::exeFailedEnd() {}

void TimeBalloonStateFindBalloonFailed::exeViewAroundStageDefaultCamera() {
    if (al::isFirstStep(this))
        mDistanceLayout->tryAppear();

    if (mDecideIconLayout->isDecide())
        al::setNerve(this, &ViewAroundStageDecideWait);
}

void TimeBalloonStateFindBalloonFailed::exeViewAroundStageDecideWait() {
    if (al::isFirstStep(this))
        mGuideLayout->end();

    if (mDecideIconLayout->isEnd())
        al::setNerve(this, &ViewAroundStageEnd);
}

void TimeBalloonStateFindBalloonFailed::exeViewAroundStageEnd() {
    if (al::isFirstStep(this))
        mDistanceLayout->tryEnd();
}

bool TimeBalloonStateFindBalloonFailed::isNerveFailedEnd() const {
    return al::isNerve(this, &FailedEnd);
}

bool TimeBalloonStateFindBalloonFailed::isNerveViewAroundStageEnd() const {
    return al::isNerve(this, &ViewAroundStageEnd);
}

void TimeBalloonStateFindBalloonFailed::setNerveFailedWait() {
    al::setNerve(this, &FailedWait);
}

void TimeBalloonStateFindBalloonFailed::setNerveViewAroundStage() {
    mDecideIconLayout->appear();
    al::setNerve(this, &ViewAroundStageDefaultCamera);
}
