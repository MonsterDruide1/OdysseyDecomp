#include "MapObj/KinokoUfo.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Obj/CollisionObj.h"
#include "Library/Obj/PartsFunction.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

#include "System/GameDataUtil.h"

KinokoUfo::KinokoUfo(const char* actorName) : al::LiveActor(actorName) {}

void KinokoUfo::init(const al::ActorInitInfo& info) {
    using KinokoUfoFunctor = al::FunctorV0M<KinokoUfo*, void (KinokoUfo::*)()>;

    al::initActor(this, info);
    if (al::listenStageSwitchOnOff(this, "SwitchClose",
                                   KinokoUfoFunctor(this, &KinokoUfo::listenClose),
                                   KinokoUfoFunctor(this, &KinokoUfo::listenOpen)) ||
        rs::isSequenceTimeBalloonOrRace(this)) {
        mCollisionObj = al::createCollisionObj(
            this, info, "Close", al::getHitSensor(this, "Collision"), nullptr, nullptr);
        mCollisionObj->makeActorDead();
    }
    makeActorAlive();
}

void KinokoUfo::listenClose() {
    al::invalidateCollisionParts(this);
    mCollisionObj->appear();
    al::startAction(this, "Close");
}

void KinokoUfo::listenOpen() {
    al::validateCollisionParts(this);
    mCollisionObj->kill();
    al::startAction(this, "Open");
}

void KinokoUfo::initAfterPlacement() {
    const char* actionName = "Open";
    if (rs::isSequenceTimeBalloonOrRace(this)) {
        al::invalidateCollisionParts(this);
        mCollisionObj->appear();
        actionName = "Close";
    }
    al::startAction(this, actionName);
}
