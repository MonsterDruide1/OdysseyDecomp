#include "Npc/VocalMike.h"

#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"

#include "Util/SensorMsgFunction.h"

VocalMike::VocalMike(const char* actorName) : al::LiveActor(actorName) {}

void VocalMike::init(const al::ActorInitInfo& info) {
    al::initActor(this, info);
    makeActorAlive();
}

bool VocalMike::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                           al::HitSensor* self) {
    if (al::isMsgPlayerDisregard(message) || rs::isMsgPlayerDisregardHomingAttack(message))
        return true;
    return rs::isMsgPlayerDisregardTargetMarker(message);
}
