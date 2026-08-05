#include "MapObj/PeachWorldGate.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Obj/CollisionObj.h"
#include "Library/Obj/PartsFunction.h"
#include "Library/Se/SeFunction.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

#include "System/GameDataUtil.h"

namespace {
NERVE_IMPL(PeachWorldGate, OpenWait)
NERVE_IMPL(PeachWorldGate, CloseWait)
NERVE_IMPL(PeachWorldGate, Open)

NERVES_MAKE_NOSTRUCT(PeachWorldGate, OpenWait, CloseWait, Open)
}  // namespace

PeachWorldGate::PeachWorldGate(const char* actorName) : al::LiveActor(actorName) {}

void PeachWorldGate::init(const al::ActorInitInfo& info) {
    al::initActor(this, info);
    mSaveObjInfo = rs::createSaveObjInfoNoWriteSaveDataInSameWorldResetMiniGame(info);
    mCollisionObj = al::createCollisionObj(this, info, "OpenGate",
                                           al::getHitSensor(this, "Collision"), nullptr, nullptr);

    if (rs::isOnSaveObjInfo(mSaveObjInfo) && !rs::isSequenceTimeBalloonOrRace(this)) {
        mCollisionObj->makeActorAlive();
        al::initNerve(this, &OpenWait, 0);
        al::invalidateCollisionParts(this);
    } else {
        mCollisionObj->makeActorDead();
        al::initNerve(this, &CloseWait, 0);

        using PeachWorldGateFunctor = al::FunctorV0M<PeachWorldGate*, void (PeachWorldGate::*)()>;

        al::listenStageSwitchOnStart(this, PeachWorldGateFunctor(this, &PeachWorldGate::start));
    }

    makeActorAlive();
}

void PeachWorldGate::start() {
    if (rs::isSequenceTimeBalloonOrRace(this))
        return;

    if (al::isNerve(this, &CloseWait)) {
        al::invalidateClipping(this);
        al::setNerve(this, &Open);
    }
}

void PeachWorldGate::exeCloseWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "CloseWait");
}

void PeachWorldGate::exeOpen() {
    if (al::isFirstStep(this))
        al::startAction(this, "Open");

    if (al::isStep(this, 80)) {
        al::invalidateCollisionParts(this);
        mCollisionObj->makeActorAlive();
    }

    if (al::isActionEnd(this)) {
        al::setNerve(this, &OpenWait);
        al::startSe(this, "PgOpenEnd");
    }
}

void PeachWorldGate::exeOpenWait() {
    if (al::isFirstStep(this)) {
        al::validateClipping(this);
        al::startAction(this, "OpenWait");
        rs::onSaveObjInfo(mSaveObjInfo);
    }
}
