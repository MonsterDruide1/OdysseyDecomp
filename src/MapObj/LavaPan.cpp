#include "MapObj/LavaPan.h"

#include "Library/Demo/DemoFunction.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Player/PlayerUtil.h"

namespace {
NERVE_IMPL(LavaPan, Wait)
NERVE_IMPL(LavaPan, Rumble)

NERVES_MAKE_NOSTRUCT(LavaPan, Wait, Rumble)
}  // namespace

LavaPan::LavaPan(const char* name) : al::LiveActor(name) {}

void LavaPan::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "LavaWorldHomePan000", nullptr);
    al::initNerve(this, &Wait, 0);
    al::registActorToDemoInfo(this, info);
    makeActorAlive();
}

void LavaPan::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Stop");
    if (!al::isNearPlayer(this, 8000.0f))
        al::setNerve(this, &Rumble);
}

void LavaPan::exeRumble() {
    if (al::isFirstStep(this))
        al::startAction(this, "LavaWorldHomePan000");
    if (al::isNearPlayer(this, 8000.0f))
        al::setNerve(this, &Wait);
}
