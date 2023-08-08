#include "game/objects/FireDrum2D/FireDrum2D.h"

#include "al/sensor/SensorMsg.h"
#include "al/util/LiveActorUtil.h"
#include "al/util/NerveUtil.h"
#include "al/util/OtherUtil.h"
#include "rs/Dimension.h"
#include "rs/Sensor.h"

FireDrum2D::FireDrum2D(const char* name) : LiveActor(name) {}

void FireDrum2D::init(const al::ActorInitInfo& info) {
    al::initActor(this, info);
    al::initNerve(this, &nrvFireDrum2DWait, 0);
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
        al::setNerve(this, &nrvFireDrum2DWait);
    }
}

void FireDrum2D::attackSensor(al::HitSensor* source, al::HitSensor* target) {
    if (rs::sendMsgTouchFireDrum2D(target, source) || rs::sendMsgEnemyAttack2D(target, source)) {
        al::setNerve(this, &nrvFireDrum2DBurn);
    }
}

bool FireDrum2D::receiveMsg(const al::SensorMsg* message, al::HitSensor* source,
                            al::HitSensor* target) {
    return al::isMsgPlayerDisregard(message);
}

namespace {
NERVE_IMPL(FireDrum2D, Wait)
NERVE_IMPL(FireDrum2D, Burn)
}  // namespace
