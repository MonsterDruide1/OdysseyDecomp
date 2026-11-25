#include "Boss/Koopa/KoopaFire2D.h"

#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/Collision/CollisionPartsTriangle.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/ActorDimensionUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(KoopaFire2D, FlyWait)

NERVES_MAKE_STRUCT(KoopaFire2D, FlyWait)
}  // namespace

KoopaFire2D::KoopaFire2D(const char* name) : al::LiveActor(name) {}

void KoopaFire2D::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "KoopaFire2D", nullptr);
    al::initNerve(this, &NrvKoopaFire2D.FlyWait, 0);
    mDimensionKeeper = rs::createDimensionKeeper(this);
    mCollisionPartsFilter = rs::createCollisionPartsFilter2DOnly();
    al::setColliderFilterCollisionParts(this, mCollisionPartsFilter);
    makeActorDead();
}

void KoopaFire2D::appear() {
    LiveActor::appear();

    if (!tryUpdate2D())
        return;

    sead::Vector3f pos = al::getTrans(this) - al::getFront(this) * 150.0f;
    al::Triangle triangle;
    if (alCollisionUtil::getFirstPolyOnArrow(this, nullptr, &triangle, pos,
                                             al::getFront(this) *
                                                 (al::getColliderRadius(this) + 150.0f),
                                             mCollisionPartsFilter, nullptr)) {
        attackAndKill(triangle.getSensor());
    } else {
        al::setNerve(this, &NrvKoopaFire2D.FlyWait);
    }
}

void KoopaFire2D::exeFlyWait() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "FlyWait");
        al::setVelocityToDirection(this, al::getFront(this), 15.0f);
    }

    if (!tryUpdate2D())
        return;

    if (al::isCollidedWallVelocity(this))
        attackAndKill(al::getCollidedWallSensor(this));
    else if (al::isGreaterEqualStep(this, 300))
        kill();
}

bool KoopaFire2D::tryUpdate2D() {
    rs::updateDimensionKeeper(mDimensionKeeper);
    if (!rs::is2D(this)) {
        kill();
        return false;
    }

    rs::snap2DGravity(this, this, 500.0f);
    return true;
}

void KoopaFire2D::attackAndKill(al::HitSensor* sensor) {
    al::HitSensor* hit_sensor = al::getHitSensor(this, "Attack");
    rs::sendMsgKoopaFire2D(sensor, hit_sensor);
    kill();
}
