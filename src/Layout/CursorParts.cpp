#include "Layout/CursorParts.h"

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(CursorParts, Appear);
NERVE_IMPL(CursorParts, End);
NERVE_IMPL(CursorParts, Hide);
NERVE_IMPL(CursorParts, Wait);

NERVES_MAKE_NOSTRUCT(CursorParts, End, Wait, Appear, Hide);
}  // namespace

CursorParts::CursorParts(al::LayoutActor* parentLayout, const al::LayoutInitInfo& info,
                         const char* name, const char* archiveName)
    : al::LayoutActor(name) {
    al::initLayoutPartsActor(this, parentLayout, info, archiveName);
    initNerve(&Appear);
}

void CursorParts::appearStart() {
    al::startAction(this, "Appear");
    al::LayoutActor::appear();
    al::setNerve(this, &Appear);
}

void CursorParts::end() {
    if (al::isNerve(this, &End))
        return;

    al::startAction(this, "End");
    al::setNerve(this, &End);
}

void CursorParts::hide() {
    al::startAction(this, "Hide");
    al::setNerve(this, &Hide);
}

void CursorParts::tryAppearIfHide() {
    if (al::isNerve(this, &Hide)) {
        al::startAction(this, "Appear");
        al::LayoutActor::appear();
        al::setNerve(this, &Appear);
    }
}

void CursorParts::exeAppear() {
    if (al::isActionEnd(this))
        al::setNerve(this, &Wait);
}

void CursorParts::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");
}

void CursorParts::exeEnd() {
    if (al::isActionEnd(this))
        kill();
}

void CursorParts::exeHide() {
    kill();
}
