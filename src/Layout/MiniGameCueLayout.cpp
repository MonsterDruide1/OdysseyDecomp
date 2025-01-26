#include "Layout/MiniGameCueLayout.h"

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Nerve/NerveSetupUtil.h"

namespace {
NERVE_IMPL(MiniGameCueLayout, Wait);
NERVE_IMPL(MiniGameCueLayout, Appear);

NERVES_MAKE_NOSTRUCT(MiniGameCueLayout, Wait, Appear);
}  // namespace

MiniGameCueLayout::MiniGameCueLayout(const char* name, const al::LayoutInitInfo& info)
    : al::LayoutActor(name) {
    al::initLayoutActor(this, info, "MiniGameCue", nullptr);
    initNerve(&Wait, 0);
    kill();
}

void MiniGameCueLayout::appearMiss() {
    al::setNerve(this, &Appear);
    al::startAction(this, "Miss", nullptr);
    appear();
}

void MiniGameCueLayout::appearCount(s32 count) {
    al::setPaneNumberDigit1(this, "TxtCount", count, 0);
    al::setNerve(this, &Appear);
    al::startAction(this, "Count", nullptr);
    appear();
}

void MiniGameCueLayout::appearGo() {
    al::setNerve(this, &Appear);
    al::startAction(this, "Go", nullptr);
    appear();
}

void MiniGameCueLayout::appearFinish() {
    al::setNerve(this, &Appear);
    al::startAction(this, "Finish", nullptr);
    appear();
}

void MiniGameCueLayout::exeWait() {}

void MiniGameCueLayout::exeAppear() {
    if (al::isActionEnd(this, nullptr))
        kill();
}
