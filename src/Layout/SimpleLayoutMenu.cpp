#include "Layout/SimpleLayoutMenu.h"

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_HOST_TYPE_IMPL(SimpleLayoutMenu, Appear);
NERVE_HOST_TYPE_IMPL(SimpleLayoutMenu, Wait);
NERVE_HOST_TYPE_IMPL(SimpleLayoutMenu, End);
NERVE_HOST_TYPE_IMPL(SimpleLayoutMenu, EndWait);

NERVES_MAKE_STRUCT(HostType, Appear, End, EndWait, Wait);
}  // namespace

SimpleLayoutMenu::SimpleLayoutMenu(const char* name, const char* layoutName,
                                   const al::LayoutInitInfo& info, const char* archiveName,
                                   bool localize)
    : al::LayoutActor(name) {
    if (localize)
        al::initLayoutActorLocalized(this, info, layoutName, archiveName);
    else
        al::initLayoutActor(this, info, layoutName, archiveName);

    initNerve(&NrvHostType.Appear);
}

SimpleLayoutMenu::SimpleLayoutMenu(al::LayoutActor* parent, const char* name,
                                   const char* layoutName, const al::LayoutInitInfo& info,
                                   const char* archiveName)
    : al::LayoutActor(name) {
    al::initLayoutPartsActor(this, parent, info, layoutName, archiveName);
    initNerve(&NrvHostType.Appear);
}

void SimpleLayoutMenu::startAppear(const char* actionName) {
    al::startAction(this, actionName);
    al::LayoutActor::appear();
    setNerve(this, &NrvHostType.Appear);
}

void SimpleLayoutMenu::startEnd(const char* actionName) {
    if (al::isNerve(this, &NrvHostType.End) || isEndWait())
        return;
    al::startAction(this, actionName);
    al::setNerve(this, &NrvHostType.End);
}

void SimpleLayoutMenu::exeAppear() {
    if (al::isActionEnd(this))
        al::setNerve(this, &NrvHostType.Wait);
}

void SimpleLayoutMenu::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");
    if (mWaitDuration >= 0 && al::isGreaterEqualStep(this, mWaitDuration))
        al::setNerve(this, &NrvHostType.End);
}

void SimpleLayoutMenu::exeEnd() {
    if (al::isActionEnd(this))
        al::setNerve(this, &NrvHostType.EndWait);
}

void SimpleLayoutMenu::exeEndWait() {}

bool SimpleLayoutMenu::isAppearOrWait() const {
    return isWait() || al::isNerve(this, &NrvHostType.Appear);
}

bool SimpleLayoutMenu::isWait() const {
    return al::isNerve(this, &NrvHostType.Wait);
}

bool SimpleLayoutMenu::isEndWait() const {
    return al::isNerve(this, &NrvHostType.EndWait);
}
