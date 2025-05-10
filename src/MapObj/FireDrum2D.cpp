#include "MapObj/FireDrum2D.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/ActorDimensionKeeper.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(FireDrum2D, Wait);
NERVE_IMPL(FireDrum2D, Burn);

NERVES_MAKE_NOSTRUCT(FireDrum2D, Wait, Burn);
}  // namespace

FireDrum2D::FireDrum2D(const char* name) : LiveActor(name) {}

void FireDrum2D::init(const al::ActorInitInfo& info) {
    al::initActor(this, info);
    al::initNerve(this, &Wait, 0);
    mDimensionKeeper = rs::createDimensionKeeper(this);
    rs::updateDimensionKeeper(mDimensionKeeper);

    if (rs::isIn2DArea(this)) {
        rs::snap2D(this, this, 500.0f);
        makeActorAlive();
    } else {
        makeActorDead();
    }
}

void FireDrum2D::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Burn");
}

void FireDrum2D::exeBurn() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");
    if (al::isActionEnd(this))
        al::setNerve(this, &Wait);
}

void FireDrum2D::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (rs::sendMsgTouchFireDrum2D(other, self) || rs::sendMsgEnemyAttack2D(other, self))
        al::setNerve(this, &Burn);
}

bool FireDrum2D::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                            al::HitSensor* self) {
    return al::isMsgPlayerDisregard(message);
}
