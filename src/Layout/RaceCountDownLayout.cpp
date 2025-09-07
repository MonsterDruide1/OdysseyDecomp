#include "Layout/RaceCountDownLayout.h"

#include "Library/Base/StringUtil.h"
#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Se/SeFunction.h"

namespace {
NERVE_IMPL(RaceCountDownLayout, Wait);
NERVE_IMPL(RaceCountDownLayout, CountDown);
NERVE_IMPL(RaceCountDownLayout, CountDownEnd);
NERVE_IMPL(RaceCountDownLayout, Finish);
NERVE_IMPL(RaceCountDownLayout, FinishEnd);

NERVES_MAKE_NOSTRUCT(RaceCountDownLayout, Wait, CountDown, CountDownEnd, Finish, FinishEnd);
}  // namespace

RaceCountDownLayout::RaceCountDownLayout(const al::LayoutInitInfo& info)
    : al::LayoutActor("[レース]カウントダウン") {
    mCountLayout = new al::LayoutActor("[レース]カウントダウン数字");
    al::initLayoutActor(this, info, "MiniGameCue");
    al::initLayoutActor(mCountLayout, info, "MiniGameCount");
    initNerve(&Wait);
}

void RaceCountDownLayout::startCountDown(s32 count) {
    mCount = count;
    al::startAction(this, "Hide");
    al::setNerve(this, &CountDown);
    appear();
}

void RaceCountDownLayout::startFinish() {
    al::setNerve(this, &Finish);
    appear();
}

bool RaceCountDownLayout::isEndCountDown() const {
    return al::isNerve(this, &CountDownEnd);
}

bool RaceCountDownLayout::isEndFinish() const {
    return al::isNerve(this, &FinishEnd);
}

void RaceCountDownLayout::reset() {
    al::setNerve(this, &Wait);
}

void RaceCountDownLayout::exeWait() {}

inline void toggleNumberLayouts(al::LayoutActor* countLayout, s32 count) {
    for (s32 i = 0; i < 11; i++) {
        al::startAction(countLayout, (count == i) ? "On" : "Off",
                        al::StringTmp<32>("Number%02d", i).cstr());
    }
}

void RaceCountDownLayout::exeCountDown() {
    if (al::isFirstStep(this)) {
        al::startSe(this, "Count1");

        toggleNumberLayouts(mCountLayout, mCount);

        al::startAction(mCountLayout, "Count");
        mCountLayout->appear();
    }

    if (al::isFirstStep(this) || al::getNerveStep(this) % 60 != 0)
        return;

    mCount--;

    al::startAction(mCountLayout, "Count");
    toggleNumberLayouts(mCountLayout, mCount);

    if (mCount != 0) {
        al::startSe(this, "Count1");
    } else {
        al::startSe(this, "Count2");
        mCountLayout->kill();
        al::startAction(this, "Go");
        al::setNerve(this, &CountDownEnd);
    }
}

void RaceCountDownLayout::exeCountDownEnd() {
    if (al::isActionEnd(this))
        kill();
}

void RaceCountDownLayout::exeFinish() {
    if (al::isFirstStep(this))
        al::startAction(this, "Finish");
    if (al::isActionEnd(this) && al::isGreaterEqualStep(this, 120)) {
        al::setNerve(this, &FinishEnd);
        kill();
    }
}

void RaceCountDownLayout::exeFinishEnd() {}
