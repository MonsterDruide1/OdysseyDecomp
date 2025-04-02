#include "Layout/CounterLife.h"

#include "Library/Base/StringUtil.h"
#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "System/GameDataFunction.h"
#include "System/GameDataHolderAccessor.h"

namespace {
NERVE_IMPL(CounterLife, End);
NERVE_IMPL(CounterLife, None);
NERVE_IMPL(CounterLife, Gauge);
NERVE_IMPL(CounterLife, Appear);
NERVE_IMPL(CounterLife, Wait);
NERVE_IMPL(CounterLife, GaugeWait);

NERVES_MAKE_NOSTRUCT(CounterLife, End, None, Gauge, Appear, Wait, GaugeWait);
}  // namespace

CounterLife::CounterLife(const char* name, const char* resName, const al::LayoutInitInfo& info)
    : al::LayoutActor(name) {
    if (al::isEqualString(resName, "CounterLifeUp"))
        mIsCounterUp = true;

    al::initLayoutActor(this, info, resName, nullptr);
    initNerve(&End, 0);

    if (mIsCounterUp)
        return;

    if (GameDataFunction::getPlayerHitPoint(this) == 1)
        al::startAction(this, "Loop", "Loop");
    else
        al::startFreezeAction(this, "Loop", 0.0f, "Loop");
}

void CounterLife::appear() {
    al::LayoutActor::appear();
    al::setNerve(this, &None);
}

void CounterLife::kill() {
    al::LayoutActor::kill();
    al::setNerve(this, &End);
}

void CounterLife::startGauge(f32 goalFrame) {
    mGoalFrame = goalFrame;

    f32 maxGaugeFrames = al::getActionFrameMax(this, "Gauge", "Gauge");
    f32 maxLifeFrames = al::getActionFrameMax(this, "Add", "Life");

    if (mCurFrame < mGoalFrame)
        maxLifeFrames = al::getActionFrameMax(this, "Damage", "Life");

    mStep = (maxGaugeFrames / 3) / maxLifeFrames;
    al::setNerve(this, &Gauge);
}

void CounterLife::startGaugeWithFrame(f32 goalFrame, f32 frames) {
    startGauge(goalFrame);

    mStep = sead::Mathf::abs(mGoalFrame - mCurFrame) / frames;
}

void CounterLife::setEmpty() {
    setCount(al::getActionFrameMax(this, "Gauge", "Gauge"));
}

void CounterLife::setGaugeAnim() {
    al::startFreezeAction(this, "Gauge", mCurFrame, "Gauge");
}

void CounterLife::setCount(f32 frame) {
    mGoalFrame = frame;
    mCurFrame = frame;

    setGaugeAnim();
}

void CounterLife::start() {
    if (al::isNerve(this, &Appear) || al::isNerve(this, &Wait) || al::isNerve(this, &Gauge))
        return;

    al::startAction(this, "Appear", "Main");
    al::LayoutActor::appear();
    al::setNerve(this, &Appear);
}

void CounterLife::end() {
    if (!al::isNerve(this, &End))
        al::setNerve(this, &End);
}

void CounterLife::wait() {
    if (!al::isNerve(this, &Wait))
        al::setNerve(this, &Wait);
}

bool CounterLife::isWait() const {
    return al::isNerve(this, &Wait);
}

void CounterLife::exeNone() {}

void CounterLife::exeAppear() {
    if (al::isActionEnd(this, nullptr))
        al::setNerve(this, &Wait);
}

void CounterLife::exeWait() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Wait", "Main");

        if (!mIsCounterUp) {
            if (GameDataFunction::getPlayerHitPoint(this) == 1)
                al::startAction(this, "Loop", "Loop");
            else
                al::startFreezeAction(this, "Loop", 0.0f, "Loop");
        }
    }
}

void CounterLife::exeEnd() {
    if (al::isFirstStep(this))
        al::startAction(this, "End", "Main");

    if (al::isActionEnd(this, nullptr))
        kill();
}

void CounterLife::exeGauge() {
    if (al::isNear(mGoalFrame, mCurFrame)) {
        mCurFrame = mGoalFrame;

        setGaugeAnim();
        al::setNerve(this, &GaugeWait);
    } else {
        if (mGoalFrame > mCurFrame)
            mCurFrame = sead::Mathf::min(mCurFrame + mStep, mGoalFrame);
        else
            mCurFrame = sead::Mathf::max(mCurFrame - mStep, mGoalFrame);

        setGaugeAnim();
    }
}

void CounterLife::exeGaugeWait() {}
