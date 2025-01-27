#include "Layout/HardIconParts.h"

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActor.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(HardIconParts, Hide);
NERVE_IMPL(HardIconParts, Appear);
NERVE_IMPL(HardIconParts, Wait);
NERVE_IMPL(HardIconParts, PageNext);
NERVE_IMPL(HardIconParts, PageEnd);
NERVE_IMPL(HardIconParts, End);

NERVES_MAKE_NOSTRUCT(HardIconParts, Hide, Appear, Wait, PageNext, PageEnd, End);
}  // namespace

HardIconParts::HardIconParts(const char* name) : al::LayoutActor(name) {
    initNerve(&Hide, 0);
}

bool HardIconParts::isHide() const {
    return al::isNerve(this, &Hide);
}

bool HardIconParts::isWait() const {
    return al::isNerve(this, &Wait);
}

void HardIconParts::startHide() {
    al::setNerve(this, &Hide);
}

void HardIconParts::startAppear() {
    al::setNerve(this, &Appear);
}

void HardIconParts::startWait() {
    al::setNerve(this, &Wait);
}

void HardIconParts::startEnd() {
    al::setNerve(this, &End);
}

void HardIconParts::exeHide() {
    if (al::isFirstStep(this))
        al::startAction(this, "Hide", nullptr);
}

void HardIconParts::exeAppear() {
    if (al::isFirstStep(this))
        al::startAction(this, "Appear", nullptr);
    al::setNerveAtActionEnd(this, &Wait);
}

void HardIconParts::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait", nullptr);
}

void HardIconParts::exePageNext() {
    if (al::isFirstStep(this))
        al::startAction(this, "PageNext", nullptr);
    al::setNerveAtActionEnd(this, &Hide);
}

void HardIconParts::exePageEnd() {
    if (al::isFirstStep(this))
        al::startAction(this, "PageEnd", nullptr);
    al::setNerveAtActionEnd(this, &Hide);
}

void HardIconParts::exeEnd() {
    if (al::isFirstStep(this))
        al::startAction(this, "End", nullptr);
    al::setNerveAtActionEnd(this, &Hide);
}
