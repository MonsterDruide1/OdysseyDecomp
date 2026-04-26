#include "MapObj/PeachTrunk.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(PeachTrunk, Wait)
NERVE_IMPL(PeachTrunk, ReactionCap)

NERVES_MAKE_NOSTRUCT(PeachTrunk, Wait, ReactionCap)
}  // namespace

PeachTrunk::PeachTrunk(const char* actorName) : al::LiveActor(actorName) {}

void PeachTrunk::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "PeachTrunk", nullptr);
    al::initNerve(this, &Wait, 0);
    makeActorAlive();
}

bool PeachTrunk::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                            al::HitSensor* self) {
    if (al::isMsgPlayerDisregard(message))
        return true;
    if (rs::isMsgNpcCapReactionAll(message)) {
        rs::requestHitReactionToAttacker(message, self, other);
        al::setNerve(this, &ReactionCap);
        return true;
    }
    return false;
}

void PeachTrunk::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");
}

void PeachTrunk::exeReactionCap() {
    if (al::isFirstStep(this))
        al::startAction(this, "ReactionCap");
    al::setNerveAtActionEnd(this, &Wait);
}
