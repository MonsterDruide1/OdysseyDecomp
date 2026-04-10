#include "MapObj/FixMapPartsBgmChangeAction.h"

#include "Library/Bgm/BgmLineFunction.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Placement/PlacementFunction.h"

namespace {
NERVE_ACTION_IMPL(FixMapPartsBgmChangeAction, Wait)
NERVE_ACTION_IMPL(FixMapPartsBgmChangeAction, PlayingBgm)

NERVE_ACTIONS_MAKE_STRUCT(FixMapPartsBgmChangeAction, Wait, PlayingBgm)
}  // namespace

FixMapPartsBgmChangeAction::FixMapPartsBgmChangeAction(const char* actorName)
    : al::LiveActor(actorName) {}

void FixMapPartsBgmChangeAction::init(const al::ActorInitInfo& info) {
    al::initNerveAction(this, "Wait", &NrvFixMapPartsBgmChangeAction.collector, 0);
    al::initMapPartsActor(this, info, nullptr);
    al::getStringArg(&mChangeBgm, info, "ChangeBgm");
    makeActorAlive();
}

bool FixMapPartsBgmChangeAction::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                            al::HitSensor* self) {
    if (al::isMsgAskSafetyPoint(message))
        return true;
    return false;
}

void FixMapPartsBgmChangeAction::exeWait() {
    if (al::isRunningBgm(this, mChangeBgm))
        al::startNerveAction(this, "PlayingBgm");
}

void FixMapPartsBgmChangeAction::exePlayingBgm() {
    if (!al::isRunningBgm(this, mChangeBgm))
        al::startNerveAction(this, "Wait");
}
