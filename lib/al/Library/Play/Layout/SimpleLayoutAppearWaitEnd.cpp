#include "Library/Play/Layout/SimpleLayoutAppearWaitEnd.h"

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActor.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
using namespace al;
NERVE_HOST_TYPE_IMPL(SimpleLayoutAppearWaitEnd, Appear);
NERVE_HOST_TYPE_IMPL(SimpleLayoutAppearWaitEnd, End);
NERVES_MAKE_NOSTRUCT(HostType, End);
NERVE_HOST_TYPE_IMPL(SimpleLayoutAppearWaitEnd, Wait);

NERVES_MAKE_STRUCT(HostType, Appear, Wait);
}  // namespace

al::SimpleLayoutAppearWaitEnd::SimpleLayoutAppearWaitEnd(const char* name, const char* layoutName,
                                                         const LayoutInitInfo& info,
                                                         const char* archiveName, bool localize)
    : LayoutActor(name) {
    if (localize)
        initLayoutActorLocalized(this, info, layoutName, archiveName);
    else
        initLayoutActor(this, info, layoutName, archiveName);
    initNerve(&NrvHostType.Appear, 0);
}

al::SimpleLayoutAppearWaitEnd::SimpleLayoutAppearWaitEnd(LayoutActor* parentActor, const char* name,
                                                         const char* layoutName,
                                                         const LayoutInitInfo& info,
                                                         const char* archiveName)
    : LayoutActor(name) {
    initLayoutPartsActor(this, parentActor, info, layoutName, archiveName);
    initNerve(&NrvHostType.Appear, 0);
};

void al::SimpleLayoutAppearWaitEnd::appear() {
    startAction(this, "Appear", nullptr);
    LayoutActor::appear();
    setNerve(this, &NrvHostType.Appear);
}

void al::SimpleLayoutAppearWaitEnd::end() {
    if (!isNerve(this, &End))
        setNerve(this, &End);
}

void al::SimpleLayoutAppearWaitEnd::startWait() {
    startAction(this, "Wait", nullptr);
    LayoutActor::appear();
    setNerve(this, &NrvHostType.Wait);
}

void al::SimpleLayoutAppearWaitEnd::exeAppear() {
    if (isActionEnd(this, nullptr))
        setNerve(this, &NrvHostType.Wait);
}

void al::SimpleLayoutAppearWaitEnd::exeWait() {
    if (isFirstStep(this))
        startAction(this, "Wait", nullptr);
    if (mLifetime >= 0 && isGreaterEqualStep(this, mLifetime))
        setNerve(this, &End);
}

void al::SimpleLayoutAppearWaitEnd::exeEnd() {
    if (isFirstStep(this))
        startAction(this, "End", nullptr);
    if (isActionEnd(this, nullptr))
        kill();
}

bool al::SimpleLayoutAppearWaitEnd::isWait() const {
    return isNerve(this, &NrvHostType.Wait);
}

bool al::SimpleLayoutAppearWaitEnd::isAppearOrWait() const {
    return isNerve(this, &NrvHostType.Wait) || isNerve(this, &NrvHostType.Appear);
}
