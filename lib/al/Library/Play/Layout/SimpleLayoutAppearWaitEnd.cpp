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

al::SimpleLayoutAppearWaitEnd::SimpleLayoutAppearWaitEnd(const char* a2, const char* a3,
                                                         const al::LayoutInitInfo& info,
                                                         const char* a5, bool localize)
    : al::LayoutActor(a2) {
    if (localize)
        al::initLayoutActorLocalized(this, info, a3, a5);
    else
        al::initLayoutActor(this, info, a3, a5);
    initNerve(&NrvHostType.Appear, 0);
}

al::SimpleLayoutAppearWaitEnd::SimpleLayoutAppearWaitEnd(al::LayoutActor* a2, const char* a3,
                                                         const char* a4,
                                                         const al::LayoutInitInfo& info,
                                                         const char* a6)
    : al::LayoutActor(a3) {
    al::initLayoutPartsActor(this, a2, info, a4, a6);
    initNerve(&NrvHostType.Appear, 0);
};

void al::SimpleLayoutAppearWaitEnd::appear() {
    al::startAction(this, "Appear", nullptr);
    al::LayoutActor::appear();
    al::setNerve(this, &NrvHostType.Appear);
}

void al::SimpleLayoutAppearWaitEnd::end() {
    if (!al::isNerve(this, &End))
        al::setNerve(this, &End);
}

void al::SimpleLayoutAppearWaitEnd::startWait() {
    al::startAction(this, "Wait", nullptr);
    al::LayoutActor::appear();
    al::setNerve(this, &NrvHostType.Wait);
}

void al::SimpleLayoutAppearWaitEnd::exeAppear() {
    if (al::isActionEnd(this, nullptr))
        al::setNerve(this, &NrvHostType.Wait);
}

void al::SimpleLayoutAppearWaitEnd::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait", nullptr);
    if (field_12C >= 0 && al::isGreaterEqualStep(this, field_12C))
        al::setNerve(this, &End);
}

void al::SimpleLayoutAppearWaitEnd::exeEnd() {
    if (al::isFirstStep(this))
        al::startAction(this, "End", nullptr);
    if (al::isActionEnd(this, nullptr))
        kill();
}

bool al::SimpleLayoutAppearWaitEnd::isWait() const {
    return al::isNerve(this, &NrvHostType.Wait);
}

bool al::SimpleLayoutAppearWaitEnd::isAppearOrWait() const {
    return al::isNerve(this, &NrvHostType.Wait) || al::isNerve(this, &NrvHostType.Appear);
}
