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
NERVE_HOST_TYPE_IMPL(SimpleLayoutAppearWaitEnd, Wait);

NERVES_MAKE_NOSTRUCT(HostType, End);
NERVES_MAKE_STRUCT(HostType, Appear, Wait);
}  // namespace

namespace al {

SimpleLayoutAppearWaitEnd::SimpleLayoutAppearWaitEnd(const char* name, const char* layoutName,
                                                     const LayoutInitInfo& info,
                                                     const char* archiveName, bool localize)
    : LayoutActor(name) {
    if (localize)
        initLayoutActorLocalized(this, info, layoutName, archiveName);
    else
        initLayoutActor(this, info, layoutName, archiveName);
    initNerve(&NrvHostType.Appear);
}

SimpleLayoutAppearWaitEnd::SimpleLayoutAppearWaitEnd(LayoutActor* parentActor, const char* name,
                                                     const char* layoutName,
                                                     const LayoutInitInfo& info,
                                                     const char* archiveName)
    : LayoutActor(name) {
    initLayoutPartsActor(this, parentActor, info, layoutName, archiveName);
    initNerve(&NrvHostType.Appear);
};

void SimpleLayoutAppearWaitEnd::appear() {
    startAction(this, "Appear");
    LayoutActor::appear();
    setNerve(this, &NrvHostType.Appear);
}

void SimpleLayoutAppearWaitEnd::end() {
    if (!isNerve(this, &End))
        setNerve(this, &End);
}

void SimpleLayoutAppearWaitEnd::startWait() {
    startAction(this, "Wait");
    LayoutActor::appear();
    setNerve(this, &NrvHostType.Wait);
}

void SimpleLayoutAppearWaitEnd::exeAppear() {
    if (isActionEnd(this))
        setNerve(this, &NrvHostType.Wait);
}

void SimpleLayoutAppearWaitEnd::exeWait() {
    if (isFirstStep(this))
        startAction(this, "Wait");
    if (mLifetime >= 0 && isGreaterEqualStep(this, mLifetime))
        setNerve(this, &End);
}

void SimpleLayoutAppearWaitEnd::exeEnd() {
    if (isFirstStep(this))
        startAction(this, "End");
    if (isActionEnd(this))
        kill();
}

bool SimpleLayoutAppearWaitEnd::isWait() const {
    return isNerve(this, &NrvHostType.Wait);
}

bool SimpleLayoutAppearWaitEnd::isAppearOrWait() const {
    return isWait() || isNerve(this, &NrvHostType.Appear);
}

}  // namespace al
