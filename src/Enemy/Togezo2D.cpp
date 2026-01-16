#include "Enemy/Togezo2D.h"

#include "Library/Item/ItemUtil.h"
#include "Library/Joint/JointControllerKeeper.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/ActorDimensionUtil.h"
#include "Util/ItemUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(Togezo2D, Walk)
NERVE_IMPL(Togezo2D, Damage)
NERVE_IMPL(Togezo2D, HideWait)

NERVES_MAKE_STRUCT(Togezo2D, Walk, Damage, HideWait)
}  // namespace

Togezo2D::Togezo2D(const char* name) : al::LiveActor(name) {}

void Togezo2D::init(const al::ActorInitInfo& info) {
    al::initActor(this, info);
    al::initNerve(this, &NrvTogezo2D.Walk, 0);
    rs::createAndSetFilter2DOnly(this);
    mDimensionKeeper = rs::createDimensionKeeper(this);
    rs::updateDimensionKeeper(mDimensionKeeper);
    if (!rs::isIn2DArea(this)) {
        makeActorDead();
        return;
    }
    rs::snap2DGravity(this, this, 500.0f);
    makeActorAlive();
    mLocalZRotator = 0.0f;
    al::initJointControllerKeeper(this, 1);
    al::initJointLocalZRotator(this, &mLocalZRotator, "Center");
    mInitTrans = al::getTrans(this);
    mInitFront = al::getFront(this);
}

void Togezo2D::checkFacingTarget(al::HitSensor* target) {
    if (al::isFaceToTargetDegreeH(this, al::getSensorPos(target), al::getFront(this), 5.0f)) {
        al::turnFront(this, 180.0f);
        al::setVelocityToFront(this, mVelocityFront);
        al::addVelocityToGravity(this, 0.65f);
        updateCollider();
    }
}

bool Togezo2D::receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self) {
    if (al::isNerve(this, &NrvTogezo2D.Damage))
        return false;
    if (rs::isMsgPlayerDisregardHomingAttack(message) ||
        rs::isMsgPlayerDisregardTargetMarker(message) || al::isMsgPlayerDisregard(message))
        return true;
    if (rs::isMsgPush2D(message) && al::isNerve(this, &NrvTogezo2D.Walk)) {
        checkFacingTarget(other);
        return true;
    }
    if (rs::isMsgBlockUpperPunch2D(message)) {
        al::setNerve(this, &NrvTogezo2D.Damage);
        rs::setAppearItemFactorAndOffsetByMsg(this, message, other);
        al::appearItem(this);
        return true;
    }
    if (rs::isMsgKouraAttack2D(message)) {
        al::setNerve(this, &NrvTogezo2D.Damage);
        rs::requestHitReactionToAttacker(message, self, other);
        rs::setAppearItemFactorAndOffsetByMsg(this, message, other);
        al::appearItem(this);
        return true;
    }
    return false;
}

void Togezo2D::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isSensorEnemyBody(self) && al::isSensorEnemyBody(other) &&
        rs::sendMsgPush2D(other, self))
        checkFacingTarget(other);
    if (al::isSensorPlayer(other) && al::isSensorEnemyAttack(self))
        rs::sendMsgEnemyAttack2D(other, self);
}

void Togezo2D::startMove() {
    al::onCollide(this);
    al::setNerve(this, &NrvTogezo2D.Walk);
}

void Togezo2D::control() {
    rs::updateDimensionKeeper(mDimensionKeeper);
    if (al::isNerve(this, &NrvTogezo2D.HideWait))
        return;
    al::addVelocityToGravity(this, 0.65f);
    al::scaleVelocityDirection(this, al::getGravity(this), 0.98f);
    if (al::isCollidedWall(this)) {
        al::turnFront(this, 150.0f);
        if (al::isOnGround(this, 0)) {
            al::setVelocityToFront(this, mVelocityFront);
            al::addVelocityToGravity(this, 0.65f);
        } else {
            al::addVelocityToFront(this, 2 * mVelocityFront);
        }
        updateCollider();
    }
    if (al::isNerve(this, &NrvTogezo2D.Damage))
        return;
    if (!rs::isIn2DArea(this)) {
        al::setNerve(this, &NrvTogezo2D.HideWait);
        return;
    }
    if (al::isOnGround(this, 0)) {
        al::setVelocityToFront(this, mVelocityFront);
        al::addVelocityToGravity(this, 0.65f);
    }
    rs::snap2DGravity(this, this, 500.0f);
}

void Togezo2D::exeWalk() {
    if (al::isFirstStep(this)) {
        al::invalidateClipping(this);
        al::showModelIfHide(this);
        al::onCollide(this);
        al::validateHitSensors(this);
        mVelocityFront = 2.5f;
        al::setVelocityToFront(this, mVelocityFront);
        al::addVelocityToGravity(this, 0.65f);
        al::startAction(this, "Move");
    }
    if (al::isOnGround(this, 0)) {
        al::setVelocityToFront(this, mVelocityFront);
        al::addVelocityToGravity(this, 0.65f);
    }
}

void Togezo2D::exeDamage() {
    if (al::isFirstStep(this)) {
        al::offCollide(this);
        al::invalidateHitSensors(this);
        al::turnFront(this, 180.0f);
        mVelocityFront = 2.5f;
        mLocalZRotator = 180.0f;
        al::setVelocityToFront(this, mVelocityFront);
        al::addVelocity(this, al::getGravity(this) * -20.0f);
    }
    if (al::isGreaterStep(this, 90))
        al::setNerve(this, &NrvTogezo2D.HideWait);
}

void Togezo2D::exeHideWait() {
    if (al::isFirstStep(this)) {
        al::invalidateClipping(this);
        al::offCollide(this);
        al::hideModelIfShow(this);
        al::invalidateHitSensors(this);
        al::setVelocityZero(this);
        al::setTrans(this, mInitTrans);
        al::setFront(this, mInitFront);
        if (mIsAlwaysFalse)
            mIsAlwaysFalse = false;
        al::resetPosition(this);
    }
}
