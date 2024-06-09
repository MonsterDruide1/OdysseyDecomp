#include "Layout/BootLayout.h"

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(BootLayout, Appear);
NERVE_IMPL(BootLayout, Wait);
NERVE_IMPL(BootLayout, StartWipe);
NERVE_IMPL(BootLayout, EndWipe);
NERVE_IMPL(BootLayout, End);

NERVE_MAKE(BootLayout, Appear);
NERVE_MAKE(BootLayout, Wait);
NERVE_MAKE(BootLayout, StartWipe);
NERVE_MAKE(BootLayout, EndWipe);
NERVE_MAKE(BootLayout, End);
}  // namespace

BootLayout::BootLayout(const al::LayoutInitInfo& info) : al::LayoutActor("[起動]BootLoading") {
    al::initLayoutActor(this, info, "BootLoading", 0);
    mParBg = new al::LayoutActor("[起動]BG");
    al::initLayoutPartsActor(mParBg, this, info, "ParBG", 0);
    initNerve(&Appear, 0);
}

void BootLayout::appear() {
    al::startAction(this, "Appear", 0);
    al::startAction(mParBg, "Hide", 0);
    al::LayoutActor::appear();
    al::setNerve(this, &Appear);
}

void BootLayout::kill() {
    al::LayoutActor::kill();
}

void BootLayout::startWipe() {
    al::startAction(this, "Wait", 0);
    al::startAction(mParBg, "Hide", 0);
    al::LayoutActor::appear();
    al::setNerve(this, &StartWipe);
}

void BootLayout::endWipe() {
    al::startAction(mParBg, "Wait", "Main");
    al::LayoutActor::appear();
    al::setNerve(this, &EndWipe);
}

void BootLayout::end() {
    al::startAction(this, "End", "Main");
    al::LayoutActor::appear();
    al::setNerve(this, &End);
}

void BootLayout::endImmidiate() {
    al::startFreezeActionEnd(this, "End", "Main");
    al::LayoutActor::appear();
    al::setNerve(this, &End);
}

bool BootLayout::isEndWipe() const {
    return al::isNerve(this, &EndWipe);
}

f32 BootLayout::getBgFrame() const {
    return al::getActionFrame(mParBg, 0);
}

void BootLayout::exeAppear() {
    if (al::isActionEnd(this, 0))
        al::setNerve(this, &Wait);
}

void BootLayout::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait", "Main");
}

void BootLayout::exeStartWipe() {
    if (al::isFirstStep(this))
        al::startAction(mParBg, "Appear", "Main");
    if (!al::isActionEnd(mParBg, 0))
        return;
    al::startAction(mParBg, "Wait", "Main");
    al::setNerve(this, &EndWipe);
}

void BootLayout::exeEndWipe() {}

void BootLayout::exeEnd() {}
