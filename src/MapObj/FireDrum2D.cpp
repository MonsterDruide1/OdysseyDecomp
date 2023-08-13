#include "MapObj/FireDrum2D.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/LiveActor/LiveActorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Util/ActorDimensionKeeper.h"
#include "Util/Sensor.h"

namespace {
NERVE_IMPL(FireDrum2D, Wait);
NERVE_IMPL(FireDrum2D, Burn);

NERVE_MAKE(FireDrum2D, Wait);
NERVE_MAKE(FireDrum2D, Burn);

}  // namespace

FireDrum2D::FireDrum2D(const char* name) : LiveActor(name) {}

void FireDrum2D::init(const al::ActorInitInfo& info) {
    al::initActor(this, info);
    al::initNerve(this, &Wait, 0);
    mActorDimensionKeeper = rs::createDimensionKeeper(this);
    rs::updateDimensionKeeper(mActorDimensionKeeper);

    if (rs::isIn2DArea(this)) {
        rs::snap2D(this, this, 500.0f);
        makeActorAlive();
    } else {
        makeActorDead();
    }
}

ActorDimensionKeeper* FireDrum2D::getActorDimensionKeeper() const {
    return mActorDimensionKeeper;
}

void FireDrum2D::exeWait() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Burn");
    }
}

void FireDrum2D::exeBurn() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Wait");
    }
    if (al::isActionEnd(this)) {
        al::setNerve(this, &Wait);
    }
}

void FireDrum2D::attackSensor(al::HitSensor* source, al::HitSensor* target) {
    if (rs::sendMsgTouchFireDrum2D(target, source) || rs::sendMsgEnemyAttack2D(target, source)) {
        al::setNerve(this, &Burn);
    }
}

bool FireDrum2D::receiveMsg(const al::SensorMsg* message, al::HitSensor* source,
                            al::HitSensor* target) {
    return al::isMsgPlayerDisregard(message);
}
