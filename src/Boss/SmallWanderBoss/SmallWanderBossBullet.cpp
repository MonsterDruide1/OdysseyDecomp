#include "Boss/SmallWanderBoss/SmallWanderBossBullet.h"

#include "Library/Demo/DemoFunction.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Math/ParabolicPath.h"
#include "Library/Nature/NatureUtil.h"
#include "Library/Nature/WaterSurfaceFinder.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Player/PlayerUtil.h"

#include "Util/PlayerCollisionUtil.h"
#include "Util/SensorMsgFunction.h"
#include "Util/WaterSurfaceShadow.h"

namespace {
NERVE_IMPL(SmallWanderBossBullet, Fly)
NERVE_IMPL(SmallWanderBossBullet, Explosion)
NERVE_IMPL(SmallWanderBossBullet, AppearAttach)
NERVE_IMPL_(SmallWanderBossBullet, AppearAttachParabolic, AppearAttach)
NERVE_IMPL(SmallWanderBossBullet, FlyDown)
NERVE_IMPL(SmallWanderBossBullet, SignExplosion)

NERVES_MAKE_NOSTRUCT(SmallWanderBossBullet, Fly, FlyDown, SignExplosion, Explosion, AppearAttach,
                     AppearAttachParabolic)
}  // namespace

SmallWanderBossBullet::SmallWanderBossBullet(const char* name) : al::LiveActor(name) {
    _108 = {1.0f, 1.0f, 1.0f};
    mParabolicPath = nullptr;
    mWaterSurfaceFinder = nullptr;
    mAttachMtx = nullptr;
    mAttachOffset = nullptr;
    mExplosionStepMax = 1200;
}

void SmallWanderBossBullet::init(const al::ActorInitInfo& initInfo) {
    al::initActorWithArchiveName(this, initInfo, "SmallWanderBossBullet", nullptr);
    al::initNerve(this, &Fly, 0);
    al::setSensorRadius(this, "Attack", 60.0f);
    al::setSensorRadius(this, "Body", 75.0f);
    al::setColliderRadius(this, 40.0f);

    mParabolicPath = new al::ParabolicPath();
    mWaterSurfaceFinder = new al::WaterSurfaceFinder(this);
    mWaterSurfaceShadow = new WaterSurfaceShadow(initInfo, "WaterSurfaceShadow");
    mWaterSurfaceShadow->setScale(8.0f);

    makeActorDead();
}

void SmallWanderBossBullet::appear() {
    al::LiveActor::appear();
    al::showModelIfHide(this);
    al::offCollide(this);
}

void SmallWanderBossBullet::kill() {
    al::LiveActor::kill();
    mWaterSurfaceShadow->disappearShadow();
}

void SmallWanderBossBullet::control() {
    if (al::isNerve(this, &Explosion))
        return;

    sead::Vector3f groundPos;
    groundPos.x = 0.0f;
    groundPos.y = 0.0f;
    const al::IUseCollision* collision = this;
    groundPos.z = 0.0f;
    f32 distance = 7500.0f;
    rs::calcGroundHeight(&distance, &groundPos, collision, al::getTrans(this),
                         -al::getGravity(this), 0.0f, 7500.0f);

    mWaterSurfaceShadow->update(al::getTrans(this), -al::getGravity(this), distance);
}

void SmallWanderBossBullet::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isNerve(this, &Explosion))
        return;

    if (!al::isSensorEnemyAttack(self))
        return;

    if (al::isNerve(this, &AppearAttach) || al::isNerve(this, &AppearAttachParabolic))
        return;

    if (al::sendMsgEnemyAttack(other, self))
        al::setNerve(this, &Explosion);
}

bool SmallWanderBossBullet::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                       al::HitSensor* self) {
    if (rs::isMsgPlayerDisregardTargetMarker(message) ||
        rs::isMsgPlayerDisregardHomingAttack(message)) {
        return true;
    }

    if (rs::isMsgKillByMoonRockDemo(message)) {
        al::killForceBeforeDemo(this);
        return true;
    }

    if (!al::isSensorEnemyBody(self) || al::isNerve(this, &Explosion))
        return false;

    if (rs::isMsgCapReflect(message) || al::isMsgKickStoneAttackReflect(message) ||
        rs::isMsgSeedAttack(message)) {
        rs::requestHitReactionToAttacker(message, self, other);
        al::setNerve(this, &Explosion);
        return true;
    }

    return false;
}

void SmallWanderBossBullet::appearAttach(const sead::Matrix34f* attachMtx,
                                         const sead::Vector3f* attachOffset) {
    const sead::Vector3f* offsetArg = attachOffset;
    const sead::Matrix34f* mtxArg = attachMtx;

    mAttachMtx = mtxArg;
    mAttachOffset = offsetArg;

    sead::Vector3f* trans = al::getTransPtr(this);
    const sead::Vector3f* offset = offsetArg ? offsetArg : &sead::Vector3f::zero;
    trans->setMul(*mtxArg, *offset);

    sead::Vector3f* front = al::getFrontPtr(this);
    mtxArg->getBase(*front, 2);

    appear();
    al::setNerve(this, &AppearAttach);
}

void SmallWanderBossBullet::appearAttachParabolic(const sead::Matrix34f* attachMtx,
                                                  const sead::Vector3f* attachOffset) {
    const sead::Vector3f* offsetArg = attachOffset;
    const sead::Matrix34f* mtxArg = attachMtx;

    mAttachMtx = mtxArg;
    mAttachOffset = offsetArg;

    sead::Vector3f* trans = al::getTransPtr(this);
    const sead::Vector3f* offset = offsetArg ? offsetArg : &sead::Vector3f::zero;
    trans->setMul(*mtxArg, *offset);

    sead::Vector3f* front = al::getFrontPtr(this);
    mtxArg->getBase(*front, 2);

    appear();
    al::setNerve(this, &AppearAttachParabolic);
}

bool SmallWanderBossBullet::tryStartFlyParabolic(const sead::Vector3f& front, f32 launchSpeed) {
    if (!al::isGreaterEqualStep(this, 5))
        return false;

    al::setFront(this, front);
    mLaunchSpeed = launchSpeed;
    al::setNerve(this, &FlyDown);
    return true;
}

void SmallWanderBossBullet::startLaunch() {
    sead::Vector3f velocity = al::getFront(this) * 10.0f;
    al::setVelocity(this, velocity);
    al::setNerve(this, &Fly);
}

void SmallWanderBossBullet::exeAppearAttach() {
    const sead::Matrix34f* attachMtx = mAttachMtx;
    const sead::Vector3f* attachOffset = mAttachOffset;
    sead::Vector3f* trans = al::getTransPtr(this);
    const sead::Vector3f* offset = attachOffset ? attachOffset : &sead::Vector3f::zero;
    trans->setMul(*attachMtx, *offset);

    sead::Vector3f* front = al::getFrontPtr(this);
    attachMtx->getBase(*front, 2);
}

void SmallWanderBossBullet::exeFly() {
    mWaterSurfaceFinder->update(al::getTrans(this), sead::Vector3f::ey, 100.0f);

    if (al::isFirstStep(this)) {
        sead::Vector3f jointPos = sead::Vector3f::zero;
        al::calcJointPos(&jointPos, this, "SmallWanderBossBullet");
        al::resetPosition(this, jointPos);
        al::startAction(this, "Fly");
        al::onCollide(this);
    }

    if (al::isCollided(this) || mWaterSurfaceFinder->isFoundSurface()) {
        if (mWaterSurfaceFinder->isFoundSurface())
            al::tryAddRippleLarge(this, mWaterSurfaceFinder->getSurfacePosition());

        al::startHitReaction(this, "破壊");
        kill();
        return;
    }

    if (al::isGreaterEqualStep(this, 600))
        kill();
}

void SmallWanderBossBullet::resetPositionByAnim() {
    sead::Vector3f jointPos = sead::Vector3f::zero;
    al::calcJointPos(&jointPos, this, "SmallWanderBossBullet");
    al::resetPosition(this, jointPos);
}

void SmallWanderBossBullet::exeFlyDown() {
    if (al::isFirstStep(this)) {
        al::onCollide(this);
        const sead::Vector3f& front = al::getFront(this);
        f32 randomSpeed = al::getRandom(-200.0f, 200.0f);
        al::setVelocitySeparateHV(this, front, randomSpeed + 300.0f, 100.0f);
    }

    al::addVelocityToGravity(this, 0.98f);
    al::scaleVelocityHV(this, 0.96f, 0.98f);
    mWaterSurfaceFinder->update(al::getTrans(this), sead::Vector3f::ey, 40.0f);

    if (al::isCollided(this) || mWaterSurfaceFinder->isFoundSurface()) {
        if (mWaterSurfaceFinder->isFoundSurface())
            al::tryAddRippleLarge(this, mWaterSurfaceFinder->getSurfacePosition());

        al::offCollide(this);
        al::setNerve(this, &SignExplosion);
        return;
    }

    if (al::isGreaterEqualStep(this, 600)) {
        al::offCollide(this);
        al::setNerve(this, &SignExplosion);
    }
}

void SmallWanderBossBullet::exeSignExplosion() {
    mWaterSurfaceFinder->update(al::getTrans(this), sead::Vector3f::ey, 40.0f);

    if (al::isFirstStep(this)) {
        al::startAction(this, "SignExplosion");
        mExplosionStepMax = 1200;
    }

    if (mExplosionStepMax == 1200 && al::isNearPlayer(this, 3000.0f)) {
        s32 step = al::getNerveStep(this) + 90;
        if (step >= 1200)
            step = 1200;
        mExplosionStepMax = step;
    }

    al::approachWaterSurfaceSpringDumper(this, mWaterSurfaceFinder, 0.0f, 12.0f, 1.0f, 0.022f,
                                         0.93f);

    if (al::isGreaterEqualStep(this, mExplosionStepMax))
        al::setNerve(this, &Explosion);
}

void SmallWanderBossBullet::exeExplosion() {
    if (al::isFirstStep(this)) {
        if (mWaterSurfaceFinder->isFoundSurface()) {
            al::tryAddRippleWithRange(this, mWaterSurfaceFinder->getSurfacePosition(), 0.25f,
                                      300.0f, 1000.0f, 1000.0f);
        }

        al::startHitReaction(this, "破壊");
        al::hideModelIfShow(this);
        mWaterSurfaceShadow->disappearShadow();
    }

    if (al::isGreaterEqualStep(this, 30))
        kill();
}
