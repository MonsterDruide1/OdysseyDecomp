#include "Library/Play/Layout/SimpleLayoutAppearWait.h"
#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
using namespace al;
NERVE_IMPL(SimpleLayoutAppearWait, Appear);
NERVE_IMPL(SimpleLayoutAppearWait, Wait);

NERVES_MAKE_NOSTRUCT(SimpleLayoutAppearWait, Appear, Wait);

}  // namespace

al::SimpleLayoutAppearWait::SimpleLayoutAppearWait(const char* a3, const char* a4,
                                                   const al::LayoutInitInfo& info, const char* a6)
    : al::LayoutActor(a3) {
    al::initLayoutActor(this, info, a4, a6);
    initNerve(&Appear, 0);
}

al::SimpleLayoutAppearWait::SimpleLayoutAppearWait(al::LayoutActor* a2, const char* a3,
                                                   const char* a4, const al::LayoutInitInfo& info,
                                                   const char* a6)
    : al::LayoutActor(a3) {
    al::initLayoutPartsActor(this, a2, info, a4, a6);
    initNerve(&Appear, 0);
}

void al::SimpleLayoutAppearWait::appear() {
    al::startAction(this, "Appear", nullptr);
    al::LayoutActor::appear();
    al::setNerve(this, &Appear);
}

bool al::SimpleLayoutAppearWait::isWait() const {
    return al::isNerve(this, &Wait);
}

void al::SimpleLayoutAppearWait::exeAppear() {
    if (al::isActionEnd(this, 0))
        al::setNerve(this, &Wait);
}

void al::SimpleLayoutAppearWait::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait", 0);
}
