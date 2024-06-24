#include "Layout/FilterScope.h"

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(FilterScope, Appear);
NERVE_IMPL(FilterScope, Wait);
NERVE_IMPL(FilterScope, End);

NERVE_MAKE(FilterScope, Appear);
NERVE_MAKE(FilterScope, Wait);
NERVE_MAKE(FilterScope, End);
}  // namespace

FilterScope::FilterScope(const char* name, const al::LayoutInitInfo& info, const char* suffix)
    : al::LayoutActor(name) {
    al::initLayoutActor(this, info, "FilterScope", suffix);
    initNerve(&Appear, 0);
    kill();
}

void FilterScope::startAppear() {
    appear();
    al::setNerve(this, &Appear);
}

void FilterScope::end() {
    al::setNerve(this, &End);
}

void FilterScope::exeAppear() {
    if (al::isFirstStep(this))
        al::startAction(this, "Appear", 0);
    if (al::isActionEnd(this, 0))
        al::setNerve(this, &Wait);
}

void FilterScope::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait", 0);
}

void FilterScope::exeEnd() {
    if (al::isFirstStep(this))
        al::startAction(this, "End", 0);
    if (al::isActionEnd(this, 0))
        kill();
}

bool FilterScope::isEnd() const {
    return !isAlive() || al::isNerve(this, &End);
}
