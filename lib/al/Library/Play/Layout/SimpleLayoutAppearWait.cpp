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

al::SimpleLayoutAppearWait::SimpleLayoutAppearWait(const char* name, const char* layoutName,
                                                   const LayoutInitInfo& info,
                                                   const char* archiveName)
    : LayoutActor(name) {
    initLayoutActor(this, info, layoutName, archiveName);
    initNerve(&Appear, 0);
}

al::SimpleLayoutAppearWait::SimpleLayoutAppearWait(LayoutActor* parentActor, const char* name,
                                                   const char* layoutName,
                                                   const LayoutInitInfo& info,
                                                   const char* archiveName)
    : LayoutActor(name) {
    initLayoutPartsActor(this, parentActor, info, layoutName, archiveName);
    initNerve(&Appear, 0);
}

void al::SimpleLayoutAppearWait::appear() {
    startAction(this, "Appear", nullptr);
    LayoutActor::appear();
    setNerve(this, &Appear);
}

bool al::SimpleLayoutAppearWait::isWait() const {
    return isNerve(this, &Wait);
}

void al::SimpleLayoutAppearWait::exeAppear() {
    if (isActionEnd(this, 0))
        setNerve(this, &Wait);
}

void al::SimpleLayoutAppearWait::exeWait() {
    if (isFirstStep(this))
        startAction(this, "Wait", 0);
}
