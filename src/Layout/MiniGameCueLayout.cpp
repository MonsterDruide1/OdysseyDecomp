#include "Layout/MiniGameCueLayout.h"

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(MiniGameCueLayout, Wait);
NERVE_IMPL(MiniGameCueLayout, Appear);

NERVES_MAKE_NOSTRUCT(MiniGameCueLayout, Wait, Appear);
}  // namespace

MiniGameCueLayout::MiniGameCueLayout(const char* name, const al::LayoutInitInfo& info)
    : al::LayoutActor(name) {
    al::initLayoutActor(this, info, "MiniGameCue");
    initNerve(&Wait);
    kill();
}

void MiniGameCueLayout::appearMiss() {
    al::setNerve(this, &Appear);
    al::startAction(this, "Miss");
    appear();
}

void MiniGameCueLayout::appearCount(s32 count) {
    al::setPaneNumberDigit1(this, "TxtCount", count, 0);
    al::setNerve(this, &Appear);
    al::startAction(this, "Count");
    appear();
}

void MiniGameCueLayout::appearGo() {
    al::setNerve(this, &Appear);
    al::startAction(this, "Go");
    appear();
}

void MiniGameCueLayout::appearFinish() {
    al::setNerve(this, &Appear);
    al::startAction(this, "Finish");
    appear();
}

void MiniGameCueLayout::exeWait() {}

void MiniGameCueLayout::exeAppear() {
    if (al::isActionEnd(this))
        kill();
}
