#include "Layout/GaugeAir.h"

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(GaugeAir, Appear);
NERVE_IMPL(GaugeAir, Wait);
NERVE_IMPL(GaugeAir, End);
NERVE_IMPL(GaugeAir, FastEnd);

NERVES_MAKE_NOSTRUCT(GaugeAir, Appear, FastEnd);
NERVES_MAKE_STRUCT(GaugeAir, Wait, End);
}  // namespace

GaugeAir::GaugeAir(const char* name, const al::LayoutInitInfo& info) : al::LayoutActor(name) {
    al::initLayoutActor(this, info, "GaugeAir", nullptr);
    initNerve(&Appear, 0);
    kill();
}

bool GaugeAir::isWait() const {
    return isAlive() && al::isNerve(this, &NrvGaugeAir.Wait);
}

void GaugeAir::start() {
    appear();
    mIsActive = false;
    exeWait();
    al::setNerve(this, &Appear);
}

void GaugeAir::updateStateAnim() {
    if (mIsActive && mRate >= 1.0f) {
        if (!al::isActionPlaying(this, "Max", "State"))
            al::startAction(this, "Max", "State");
        return;
    }

    if (al::isNearZeroOrLess(mRate)) {
        if (!al::isActionPlaying(this, "Empty", "State"))
            al::startAction(this, "Empty", "State");
        return;
    }

    if (!al::isActionPlaying(this, "Normal", "State"))
        al::startAction(this, "Normal", "State");
}

void GaugeAir::endMax() {
    if (!isWait())
        return;
    al::startFreezeAction(this, "Decrease", 0.0f, "Gauge");
    updateStateAnim();
    al::setNerve(this, &NrvGaugeAir.End);
}

void GaugeAir::fastEnd() {
    if (isAlive() && !al::isNerve(this, &FastEnd))
        al::setNerve(this, &FastEnd);
}

void GaugeAir::setRate(f32 rate) {
    mRate = rate;
    if (rate < 0.99f)
        mIsActive = true;
}

void GaugeAir::exeAppear() {
    if (al::isFirstStep(this))
        al::startAction(this, "Appear", nullptr);
    if (al::isActionEnd(this, nullptr))
        return al::setNerve(this, &NrvGaugeAir.Wait);
}

void GaugeAir::exeWait() {
    al::startFreezeAction(this, "Decrease",
                          al::getActionFrameMax(this, "Decrease", "Gauge") * (1.0f - mRate),
                          "Gauge");
    updateStateAnim();
}

void GaugeAir::exeEnd() {
    if (al::isLessStep(this, 30))
        return;

    if (al::isStep(this, 30))
        al::startAction(this, "End", nullptr);

    if (al::isActionEnd(this, nullptr))
        kill();
}

void GaugeAir::exeFastEnd() {
    if (al::isFirstStep(this))
        al::startAction(this, "End", nullptr);

    if (al::isActionEnd(this, nullptr))
        kill();
}
