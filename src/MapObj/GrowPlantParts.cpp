#include "MapObj/GrowPlantParts.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "MapObj/GrowPlantLeaf.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(GrowPlantParts, Wait);
NERVE_IMPL(GrowPlantParts, Grow);
NERVE_IMPL(GrowPlantParts, GrowEnd);

NERVES_MAKE_NOSTRUCT(GrowPlantParts, Wait, Grow, GrowEnd);
}  // namespace

GrowPlantParts::GrowPlantParts(const char* name) : al::LiveActor(name) {}

void GrowPlantParts::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "GrowPlantParts", nullptr);
    al::startAction(this, "Wait");

    mGrowPlantLeaf = new GrowPlantLeaf();
    mGrowPlantLeaf->init(info);

    al::initSubActorKeeperNoFile(this, info, 1);
    al::registerSubActorSyncAll(this, mGrowPlantLeaf);
    al::setRotateY(mGrowPlantLeaf, al::getRandom(360));
    al::initNerve(this, &Wait, 0);
    makeActorDead();
}

bool GrowPlantParts::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                al::HitSensor* self) {
    if (al::isMsgPlayerDisregard(message))
        return true;
    if (rs::isMsgPlayerDisregardHomingAttack(message))
        return true;
    if (rs::isMsgPlayerDisregardTargetMarker(message))
        return true;
    if (rs::isMsgPlayerPoleClimbKeep(message)) {
        mIsPoleClimbKeep = true;
        return true;
    }
    return false;
}

void GrowPlantParts::control() {
    al::setTrans(mGrowPlantLeaf, al::getTrans(this));
}

void GrowPlantParts::exeWait() {}

void GrowPlantParts::exeGrow() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Grow");
        mGrowPlantLeaf->startGrow();
    }
}

void GrowPlantParts::exeGrowEnd() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Wait");
        mGrowPlantLeaf->endGrow();
    }
}

void GrowPlantParts::trySetNerveGrow() {
    if (al::isNerve(this, &Grow))
        return;

    al::invalidateClipping(this);
    al::setTrans(mGrowPlantLeaf, al::getTrans(this));
    appear();
    al::setNerve(this, &Grow);
}

void GrowPlantParts::trySetNerveGrowEnd() {
    if (al::isNerve(this, &GrowEnd))
        return;

    al::validateClipping(this);
    al::setNerve(this, &GrowEnd);
}
