#include "Layout/TestFilterGlasses.h"

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(TestFilterGlasses, Appear);
NERVE_IMPL(TestFilterGlasses, End);
NERVE_IMPL(TestFilterGlasses, Wait);

NERVES_MAKE_NOSTRUCT(TestFilterGlasses, Appear, End, Wait);
}  // namespace

TestFilterGlasses::TestFilterGlasses(const char* name, const al::LayoutInitInfo& info,
                                     const char* suffix)
    : al::LayoutActor(name) {
    al::initLayoutActor(this, info, "FilterGlasses", suffix);
    initNerve(&Appear, 0);
    kill();
}

void TestFilterGlasses::startAppear() {
    appear();
    al::setNerve(this, &Appear);
}

void TestFilterGlasses::end() {
    al::setNerve(this, &End);
}

void TestFilterGlasses::exeAppear() {
    if (al::isFirstStep(this))
        al::startAction(this, "Appear", nullptr);
    if (al::isActionEnd(this, nullptr))
        al::setNerve(this, &Wait);
}

void TestFilterGlasses::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait", nullptr);
}

void TestFilterGlasses::exeEnd() {
    if (al::isFirstStep(this))
        al::startAction(this, "End", nullptr);
    if (al::isActionEnd(this, nullptr))
        kill();
}

bool TestFilterGlasses::isEnd() const {
    return !isAlive() || al::isNerve(this, &End);
}
