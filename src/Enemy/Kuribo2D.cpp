#include "Enemy/Kuribo2D.h"

#include "Library/Item/ItemUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

#include "Util/ActorDimensionUtil.h"
#include "Util/ItemUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(Kuribo2D, Walk)
NERVE_IMPL(Kuribo2D, Wait)
NERVE_IMPL(Kuribo2D, PressDown)
NERVE_IMPL(Kuribo2D, BlowDown)
NERVE_IMPL(Kuribo2D, FallAfterGenerate)

NERVES_MAKE_STRUCT(Kuribo2D, Walk, Wait, PressDown, BlowDown, FallAfterGenerate)
}  // namespace

Kuribo2D::Kuribo2D(const char* name) : al::LiveActor(name) {}

void Kuribo2D::init(const al::ActorInitInfo& info) {
    using Kuribo2DFunctor = al::FunctorV0M<Kuribo2D*, void (Kuribo2D::*)()>;

    al::initActorWithArchiveName(this, info, "Kuribo2D", nullptr);
    al::initNerve(this, &NrvKuribo2D.Walk, 0);
    rs::createAndSetFilter2DOnly(this);

    mDimensionKeeper = rs::createDimensionKeeper(this);
    rs::updateDimensionKeeper(mDimensionKeeper);
    if (!rs::isIn2DArea(this)) {
        makeActorDead();
        return;
    }

    rs::snap2D(this, this, 500.0f);

    if (al::listenStageSwitchOnStart(this, Kuribo2DFunctor(this, &Kuribo2D::startWalk)))
        al::setNerve(this, &NrvKuribo2D.Wait);

    mInitTrans.set(al::getTrans(this));
    mInitFront.set(al::getFront(this));
    al::listenStageSwitchOn(this, "SwitchReset", Kuribo2DFunctor(this, &Kuribo2D::reset));
    makeActorAlive();
}

void Kuribo2D::startWalk() {
    al::onCollide(this);
    al::validateClipping(this);
    rs::updateDimensionKeeper(mDimensionKeeper);
    rs::snap2D(this, this, 500.0f);
    if (al::isOnGround(this, 0))
        al::setNerve(this, &NrvKuribo2D.Walk);
    else
        al::setNerve(this, &NrvKuribo2D.FallAfterGenerate);
}

void Kuribo2D::reset() {
    al::setTrans(this, mInitTrans);
    al::setFront(this, mInitFront);
    al::onCollide(this);
    appear();
    al::setNerve(this, &NrvKuribo2D.Walk);
}

void Kuribo2D::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isNerve(this, &NrvKuribo2D.PressDown) || al::isNerve(this, &NrvKuribo2D.BlowDown))
        return;

    if (al::isSensorEnemyBody(self) && al::isSensorEnemyBody(other) &&
        rs::sendMsgPush2D(other, self)) {
        if (al::isFaceToTargetDegreeH(this, al::getSensorPos(other), al::getFront(this), 5.0f)) {
            al::turnFront(this, 180.0f);
            al::setVelocityToFront(this, 2.5f);
        }
    }

    if (al::isSensorEnemyAttack(self))
        rs::sendMsgEnemyAttack2D(other, self);
}

bool Kuribo2D::receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self) {
    if (al::isMsgPlayerDisregard(message))
        return true;

    if (al::isNerve(this, &NrvKuribo2D.PressDown) || al::isNerve(this, &NrvKuribo2D.BlowDown))
        return false;

    if (rs::isMsgBlockUpperPunch2D(message))
        return receiveDefeatMsg(message, other, self, &NrvKuribo2D.BlowDown);

    if (rs::isMsgPush2D(message)) {
        if (al::isFaceToTargetDegreeH(this, al::getSensorPos(other), al::getFront(this), 5.0f)) {
            al::turnFront(this, 180.0f);
            al::addVelocityToFront(this, 5.0f);
        }
        return true;
    }

    if (rs::isMsgPlayerTrample2D(message))
        return receiveDefeatMsg(message, other, self, &NrvKuribo2D.PressDown);

    if (rs::isMsgKouraAttack2D(message))
        return receiveDefeatMsg(message, other, self, &NrvKuribo2D.BlowDown);

    return false;
}

void Kuribo2D::control() {
    if (al::isInDeathArea(this)) {
        al::startHitReaction(this, "消滅");
        kill();
    }
}

void Kuribo2D::appearByGenerator(const sead::Vector3f& trans, const sead::Vector3f& front) {
    al::setTrans(this, trans);
    al::setFront(this, front);
    rs::updateDimensionKeeper(mDimensionKeeper);
    if (!rs::isIn2DArea(this)) {
        kill();
        return;
    }

    sead::Vector3f dimensionGravity;
    rs::calcDimensionGravity(&dimensionGravity, this, al::getGravity(this));
    al::setGravity(this, dimensionGravity);
    al::setVelocityZero(this);
    al::offCollide(this);
    al::invalidateClipping(this);
    appear();
    al::setNerve(this, &NrvKuribo2D.Wait);
}

bool Kuribo2D::isWait() const {
    return al::isNerve(this, &NrvKuribo2D.Wait);
}

void Kuribo2D::exeWalk() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Walk");
        al::setVelocityToFront(this, 2.5f);
    }

    rs::updateDimensionKeeper(mDimensionKeeper);
    if (!rs::isIn2DArea(this)) {
        kill();
        return;
    }

    rs::snap2D(this, this, 500.0f);

    if (al::isCollidedWall(this)) {
        const sead::Vector3f& wallNormal = al::getCollidedWallNormal(this);
        if (wallNormal.dot(al::getFront(this)) < 0.1f) {
            al::turnFront(this, 180.0f);
            al::addVelocityToFront(this, 5.0f);
            updateCollider();
        }
    }

    al::addVelocityToGravity(this, 0.65f);
    al::scaleVelocityDirection(this, al::getGravity(this), 0.98f);
    al::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);

    sead::Vector3f horizVel = al::getVelocity(this);
    al::verticalizeVec(&horizVel, al::getGravity(this), horizVel);
    f32 horizSpeed = horizVel.length();
    if (horizSpeed != 0.0f && !al::isNear(horizSpeed, 2.5f)) {
        if (al::tryNormalizeOrZero(&horizVel))
            al::scaleVelocityDirection(this, horizVel, 2.5f / horizSpeed);
    }
}

void Kuribo2D::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");
}

void Kuribo2D::exePressDown() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "PressDown");
        al::setVelocityZero(this);
    }

    if (al::isActionEnd(this)) {
        al::startHitReaction(this, "消滅");
        kill();
    }
}

void Kuribo2D::exeBlowDown() {
    sead::Vector3f dimensionGravity;
    rs::calcDimensionGravity(&dimensionGravity, this, al::getGravity(this));

    if (al::isFirstStep(this)) {
        al::startAction(this, "Down");
        al::offCollide(this);
        al::invalidateClipping(this);
        al::addVelocityToDirection(this, -dimensionGravity, 15.6f);
    }

    al::addVelocityToDirection(this, dimensionGravity, 0.73f);
    al::scaleVelocity(this, 0.98f);

    if (al::isGreaterEqualStep(this, 270)) {
        al::onCollide(this);
        kill();
    }
}

void Kuribo2D::exeFallAfterGenerate() {
    rs::updateDimensionKeeper(mDimensionKeeper);
    if (!rs::isIn2DArea(this)) {
        kill();
        return;
    }

    rs::snap2D(this, this, 500.0f);

    if (al::isOnGround(this, 0)) {
        al::setNerve(this, &NrvKuribo2D.Walk);
        return;
    }

    al::addVelocityToGravity(this, 0.65f);
    al::scaleVelocityDirection(this, al::getGravity(this), 0.98f);
    al::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
}

bool Kuribo2D::receiveDefeatMsg(const al::SensorMsg* message, al::HitSensor* other,
                                al::HitSensor* self, al::Nerve* nextNerve) {
    rs::setAppearItemFactorAndOffsetByMsg(this, message, other);
    rs::requestHitReactionToAttacker(message, self, other);
    al::appearItem(this);
    al::setNerve(this, nextNerve);
    return true;
}
