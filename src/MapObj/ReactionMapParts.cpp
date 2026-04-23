#include "MapObj/ReactionMapParts.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(ReactionMapParts, Wait)
NERVE_IMPL(ReactionMapParts, Reaction)

NERVES_MAKE_NOSTRUCT(ReactionMapParts, Wait, Reaction)
}  // namespace

ReactionMapParts::ReactionMapParts(const char* actorName) : al::LiveActor(actorName) {}

void ReactionMapParts::init(const al::ActorInitInfo& info) {
    al::initMapPartsActor(this, info, nullptr);
    al::initNerve(this, &Wait, 0);
    al::trySyncStageSwitchAppearAndKill(this);
}

bool ReactionMapParts::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                  al::HitSensor* self) {
    if (rs::isMsgCapTouchWall(message) || rs::isMsgPlayerRollingWallHitDown(message)) {
        al::setNerve(this, &Reaction);
        return true;
    }
    return false;
}

void ReactionMapParts::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");
}

void ReactionMapParts::exeReaction() {
    if (al::isFirstStep(this))
        al::startAction(this, "Reaction");
    if (al::isActionEnd(this))
        al::setNerve(this, &Wait);
}
