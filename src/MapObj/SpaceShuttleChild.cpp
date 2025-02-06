#include "MapObj/SpaceShuttleChild.h"

#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/Nerve/NerveSetupUtil.h"

namespace {
NERVE_IMPL(SpaceShuttleChild, Wait);

NERVES_MAKE_NOSTRUCT(SpaceShuttleChild, Wait);
}  // namespace

SpaceShuttleChild::SpaceShuttleChild(const char* name) : al::LiveActor(name) {}

void SpaceShuttleChild::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "SpaceShuttleB", nullptr);
    al::initNerve(this, &Wait, 5);
    makeActorAlive();
}

void SpaceShuttleChild::attackSensor(al::HitSensor* self, al::HitSensor* other) {}

bool SpaceShuttleChild::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                   al::HitSensor* self) {
    return false;
}

void SpaceShuttleChild::exeWait() {}
