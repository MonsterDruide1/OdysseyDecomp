#include "MapObj/BarrelStack2D.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/ActorDimensionUtil.h"

namespace {
NERVE_IMPL(BarrelStack2D, Wait)
NERVE_IMPL(BarrelStack2D, Break)

NERVES_MAKE_NOSTRUCT(BarrelStack2D, Wait, Break)
}  // namespace

BarrelStack2D::BarrelStack2D(const char* name) : al::LiveActor(name) {}

void BarrelStack2D::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "BarrelStack2D", nullptr);
    al::initNerve(this, &Wait, 0);

    mDimensionKeeper = rs::createDimensionKeeper(this);

    rs::updateDimensionKeeper(mDimensionKeeper);

    if (!rs::isIn2DArea(this))
        makeActorDead();

    rs::snap2D(this, this, 500.0f);
    makeActorAlive();
}

bool BarrelStack2D::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                               al::HitSensor* self) {
    return al::isMsgPlayerDisregard(message);
}

void BarrelStack2D::doBreak() {
    al::setNerve(this, &Break);
}

void BarrelStack2D::exeWait() {}

void BarrelStack2D::exeBreak() {
    if (al::isFirstStep(this))
        al::startAction(this, "Break");

    if (al::isActionEnd(this))
        kill();
}
