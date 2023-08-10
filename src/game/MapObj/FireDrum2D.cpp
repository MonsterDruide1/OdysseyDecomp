#include "game/MapObj/FireDrum2D.h"

#include "al/Library/LiveActor/ActorActionFunction.h"
#include "al/Library/LiveActor/ActorSensorMsgFunction.h"
#include "al/Library/LiveActor/LiveActorUtil.h"
#include "al/Library/Nerve/NerveSetupUtil.h"
#include "al/Library/Nerve/NerveUtil.h"
#include "al/util/OtherUtil.h"
#include "rs/Dimension.h"
#include "rs/Sensor.h"

namespace {
MAKE_NERVE(FireDrum2D, Wait);
MAKE_NERVE(FireDrum2D, Burn);
}  // namespace

FireDrum2D::FireDrum2D(const char* name) : LiveActor(name) {}

void FireDrum2D::init(const al::ActorInitInfo& info) {
    al::initActor(this, info);
    al::initNerve(this, &FireDrum2DNrvWait::sInstance, 0);
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
        al::setNerve(this, &FireDrum2DNrvWait::sInstance);
    }
}

void FireDrum2D::attackSensor(al::HitSensor* source, al::HitSensor* target) {
    if (rs::sendMsgTouchFireDrum2D(target, source) || rs::sendMsgEnemyAttack2D(target, source)) {
        al::setNerve(this, &FireDrum2DNrvBurn::sInstance);
    }
}

bool FireDrum2D::receiveMsg(const al::SensorMsg* message, al::HitSensor* source,
                            al::HitSensor* target) {
    return al::isMsgPlayerDisregard(message);
}
