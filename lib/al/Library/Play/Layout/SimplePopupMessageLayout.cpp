#include "Library/Play/Layout/SimplePopupMessageLayout.h"

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActor.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Screen/ScreenFunction.h"

namespace {
using namespace al;
NERVE_HOST_TYPE_IMPL(SimplePopupMessageLayout, Appear);
NERVE_HOST_TYPE_IMPL(SimplePopupMessageLayout, End);
NERVES_MAKE_NOSTRUCT(HostType, End);
NERVE_HOST_TYPE_IMPL(SimplePopupMessageLayout, Wait);

NERVES_MAKE_STRUCT(HostType, Appear, Wait);
}  // namespace

namespace al {

SimplePopupMessageLayout::SimplePopupMessageLayout(const char* name, const char* layoutName,
                                                   const LayoutInitInfo& info,
                                                   const char* archiveName, bool localize)
    : LayoutActor(name) {
    if (localize)
        initLayoutActorLocalized(this, info, layoutName, archiveName);
    else
        initLayoutActor(this, info, layoutName, archiveName);
    initNerve(&NrvHostType.Appear, 0);
}

void SimplePopupMessageLayout::appear() {
    startAction(this, "Appear", nullptr);
    LayoutActor::appear();
    setNerve(this, &NrvHostType.Appear);
}

void SimplePopupMessageLayout::end() {
    if (!isNerve(this, &End))
        setNerve(this, &End);
}

void SimplePopupMessageLayout::startWait() {
    startAction(this, "Wait", nullptr);
    LayoutActor::appear();
    setNerve(this, &NrvHostType.Wait);
}

void SimplePopupMessageLayout::exeAppear() {
    refreshPos();

    if (isActionEnd(this, nullptr))
        setNerve(this, &NrvHostType.Wait);
}

void SimplePopupMessageLayout::refreshPos() {
    sead::Vector2f layoutPos = sead::Vector2f::zero;
    calcLayoutPosFromWorldPos(&layoutPos, this, mWorldPos);
    setLocalTrans(this, layoutPos);
}

void SimplePopupMessageLayout::exeWait() {
    refreshPos();

    if (isFirstStep(this))
        startAction(this, "Wait", nullptr);
    if (mLifetime >= 0 && isGreaterEqualStep(this, mLifetime))
        setNerve(this, &End);
}

void SimplePopupMessageLayout::exeEnd() {
    refreshPos();

    if (isFirstStep(this))
        startAction(this, "End", nullptr);
    if (isActionEnd(this, nullptr))
        kill();
}

bool SimplePopupMessageLayout::isWait() const {
    return isNerve(this, &NrvHostType.Wait);
}

bool SimplePopupMessageLayout::isAppearOrWait() const {
    return isWait() || isNerve(this, &NrvHostType.Appear);
}

}  // namespace al
