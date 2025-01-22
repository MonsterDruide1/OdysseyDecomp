#include "MapObj/PictureNoStageChange.h"

#include "Library/LiveActor/LiveActorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"

#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(PictureNoStageChange, Wait);
NERVES_MAKE_NOSTRUCT(PictureNoStageChange, Wait);
}  // namespace

PictureNoStageChange::PictureNoStageChange(const char* actorName, const char* archiveName)
    : al::LiveActor(actorName), mArchiveName(archiveName) {}

void PictureNoStageChange::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, mArchiveName, nullptr);
    al::initNerve(this, &Wait, 0);
    makeActorAlive();
}

bool PictureNoStageChange::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                      al::HitSensor* self) {
    if (rs::isMsgPlayerDisregardHomingAttack(message))
        return true;
    return rs::isMsgPlayerDisregardTargetMarker(message);
}

void PictureNoStageChange::exeWait() {}
