#include "MapObj/EchoBlockMapParts.h"

#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

#include "MapObj/EchoEmitterHolder.h"

EchoBlockMapParts::EchoBlockMapParts(const char* name) : al::LiveActor(name) {}

void EchoBlockMapParts::init(const al::ActorInitInfo& info) {
    using EchoBlockMapPartsFunctor =
        al::FunctorV0M<EchoBlockMapParts*, void (EchoBlockMapParts::*)()>;

    al::initMapPartsActor(this, info, nullptr);
    rs::initEchoEmitterHolder(this);
    al::trySyncStageSwitchAppear(this);
    al::tryListenStageSwitchKill(this);

    if (al::listenStageSwitchOnAppear(this,
                                      EchoBlockMapPartsFunctor(this, &EchoBlockMapParts::begin)))
        makeActorDead();
}

void EchoBlockMapParts::begin() {
    makeActorAlive();
}

bool EchoBlockMapParts::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                   al::HitSensor* self) {
    return false;
}
