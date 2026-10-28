#include "MapObj/PeachWorldGate.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Obj/PartsFunction.h"
#include "Library/Se/SeFunction.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

#include "System/GameDataUtil.h"

namespace {
NERVE_IMPL(PeachWorldGate, OpenWait)
NERVE_IMPL(PeachWorldGate, CloseWait)
NERVE_IMPL(PeachWorldGate, Open)

NERVES_MAKE_STRUCT(PeachWorldGate, OpenWait, CloseWait, Open)
}  // namespace

PeachWorldGate::PeachWorldGate(const char* name) : al::LiveActor(name) {}

void PeachWorldGate::init(const al::ActorInitInfo& info) {
    using PeachWorldGateFunctor = al::FunctorV0M<PeachWorldGate*, void (PeachWorldGate::*)()>;

    al::initActor(this, info);
    mSaveObjInfo = rs::createSaveObjInfoNoWriteSaveDataInSameWorldResetMiniGame(info);
    mCollisionObj = al::createCollisionObj(this, info, "OpenGate",
                                           al::getHitSensor(this, "Collision"), nullptr, nullptr);

    if (!rs::isOnSaveObjInfo(mSaveObjInfo) || rs::isSequenceTimeBalloonOrRace(this)) {
        mCollisionObj->makeActorDead();
        al::initNerve(this, &NrvPeachWorldGate.CloseWait, 0);
        al::listenStageSwitchOnStart(this, PeachWorldGateFunctor(this, &PeachWorldGate::start));
    } else {
        mCollisionObj->makeActorAlive();
        al::initNerve(this, &NrvPeachWorldGate.OpenWait, 0);
        al::invalidateCollisionParts(this);
    }
    makeActorAlive();
}

void PeachWorldGate::start() {
    if (!rs::isSequenceTimeBalloonOrRace(this) && al::isNerve(this, &NrvPeachWorldGate.CloseWait)) {
        al::invalidateClipping(this);
        al::setNerve(this, &NrvPeachWorldGate.Open);
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
        al::setNerve(this, &NrvPeachWorldGate.OpenWait);
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
