#include "MapObj/MoonBasementBreakParts.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(MoonBasementBreakParts, Wait)
NERVE_IMPL(MoonBasementBreakParts, Break)

NERVES_MAKE_NOSTRUCT(MoonBasementBreakParts, Wait, Break)
}  // namespace

MoonBasementBreakParts::MoonBasementBreakParts(const char* actorName) : al::LiveActor(actorName) {}

void MoonBasementBreakParts::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "MoonWorldBasementBreakParts000", nullptr);
    al::initNerve(this, &Wait, 0);
    makeActorAlive();
}

void MoonBasementBreakParts::appear() {
    al::setNerve(this, &Wait);
    al::LiveActor::appear();
}

void MoonBasementBreakParts::kill() {
    al::startAction(this, "Break");
    al::setNerve(this, &Break);
}

void MoonBasementBreakParts::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");
}

void MoonBasementBreakParts::exeBreak() {
    if (al::isActionEnd(this))
        al::LiveActor::kill();
}
