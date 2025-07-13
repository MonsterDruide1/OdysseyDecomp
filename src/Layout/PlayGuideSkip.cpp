#include "Layout/PlayGuideSkip.h"

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActor.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(PlayGuideSkip, Appear);
NERVE_IMPL(PlayGuideSkip, Wait);
NERVE_IMPL(PlayGuideSkip, End);

NERVES_MAKE_NOSTRUCT(PlayGuideSkip, Appear, Wait, End);
}  // namespace

PlayGuideSkip::PlayGuideSkip(const char* name, const al::LayoutInitInfo& info)
    : al::LayoutActor(name) {
    al::initLayoutActorLocalized(this, info, "PlayGuideSkip");
    al::setPaneSystemMessage(this, "TxtGuide", "PlayGuideSkip", "PlayGuideDemoSkip");
    initNerve(&Appear);
}

void PlayGuideSkip::kill() {
    al::LayoutActor::kill();
}

bool PlayGuideSkip::tryAppear() {
    if (isAlive())
        return true;

    appearCore();
    return true;
}

void PlayGuideSkip::appearCore() {
    al::startAction(this, "Appear");
    al::LayoutActor::appear();
    al::setNerve(this, &Appear);
}

void PlayGuideSkip::end() {
    if (!al::isNerve(this, &End))
        al::setNerve(this, &End);
}

bool PlayGuideSkip::isEnableSkipDemo() const {
    return isAlive() && al::isNerve(this, &Wait);
}

void PlayGuideSkip::exeAppear() {
    if (al::isActionEnd(this))
        al::setNerve(this, &Wait);
}

void PlayGuideSkip::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");
}

void PlayGuideSkip::exeEnd() {
    if (al::isFirstStep(this))
        al::startAction(this, "End");

    if (al::isActionEnd(this))
        kill();
}
