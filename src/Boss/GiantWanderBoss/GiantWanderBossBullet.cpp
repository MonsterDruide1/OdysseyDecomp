#include "Boss/GiantWanderBoss/GiantWanderBossBullet.h"

#include <math/seadQuat.h>

#include "Library/Demo/DemoFunction.h"
#include "Library/Effect/EffectSystemInfo.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Matrix/MatrixUtil.h"
#include "Library/Nature/NatureUtil.h"
#include "Library/Nature/WaterSurfaceFinder.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Player/PlayerUtil.h"

#include "Boss/GiantWanderBoss/GiantWanderBossLifeUpItemHolder.h"
#include "Util/PlayerCollisionUtil.h"
#include "Util/SensorMsgFunction.h"
#include "Util/WaterSurfaceShadow.h"

namespace {
NERVE_IMPL(GiantWanderBossBullet, Fly);
NERVE_IMPL(GiantWanderBossBullet, AppearAttach);

NERVES_MAKE_NOSTRUCT(GiantWanderBossBullet, Fly, AppearAttach);
}  // namespace

GiantWanderBossBullet::GiantWanderBossBullet(const char* name)
    : al::LiveActor(name), mScale(1.0f, 1.0f, 1.0f), mAttachMtx(nullptr), mLocalOffset(nullptr),
      mWaterSurfaceFinder(nullptr), mWaterSurfaceShadow(nullptr), mIsWaterSurfaceEffect(false),
      mLifeUpItemHolder(nullptr), mHitPoint(5), mWaterSurfaceMtx(sead::Matrix34f::ident) {}

void GiantWanderBossBullet::init(const al::ActorInitInfo& initInfo) {
    al::initActorWithArchiveName(this, initInfo, "GiantWanderBossBullet", nullptr);
    al::initNerve(this, &Fly, 0);

    mScale.set(1.0f, 1.0f, 1.0f);
    mWaterSurfaceFinder = new al::WaterSurfaceFinder(this);
    mWaterSurfaceShadow = new WaterSurfaceShadow(initInfo, "WaterSurfaceShadow");
    mWaterSurfaceShadow->setScale(2.0f);
    mWaterSurfaceShadow->setMaxDistanceAndEndFrame(7500.0f, 0.25f);
    al::setEffectNamedMtxPtr(this, "WaterSurface", &mWaterSurfaceMtx);
    makeActorDead();
}

void GiantWanderBossBullet::appear() {
    al::LiveActor::appear();
    al::showModelIfHide(this);
    al::offCollide(this);
    mHitPoint = 5;
}

void GiantWanderBossBullet::kill() {
    if (mWaterSurfaceFinder->isFoundSurface() && mWaterSurfaceFinder->isNearSurface(150.0f))
        al::startHitReaction(this, "水面破壊");
    else
        al::startHitReaction(this, "消滅");

    mWaterSurfaceShadow->disappearShadow();
    al::LiveActor::kill();
    mLifeUpItemHolder->tryAppearItem(this);
}

void GiantWanderBossBullet::control() {
    if (mWaterSurfaceFinder->isFoundSurface()) {
        sead::Quatf quat = sead::Quatf::unit;
        al::calcQuat(&quat, this);
        al::makeMtxQuatPos(&mWaterSurfaceMtx, quat, mWaterSurfaceFinder->getSurfacePosition());
    }

    sead::Vector3f groundNormal;
    groundNormal.x = 0.0f;
    groundNormal.y = 0.0f;
    const al::IUseCollision* collision = this;
    groundNormal.z = 0.0f;
    f32 groundHeight = 7500.0f;

    const sead::Vector3f& trans = al::getTrans(this);
    sead::Vector3f down = -al::getGravity(this);
    rs::calcGroundHeight(&groundHeight, &groundNormal, collision, trans, down, 0.0f, 7500.0f);

    WaterSurfaceShadow* shadow = mWaterSurfaceShadow;
    const sead::Vector3f& shadowTrans = al::getTrans(this);
    down = -al::getGravity(this);
    shadow->update(shadowTrans, down, groundHeight);
}

void GiantWanderBossBullet::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (!al::isSensorEnemyAttack(self) || al::isNerve(this, &AppearAttach))
        return;

    if (al::sendMsgEnemyAttack(other, self) || rs::sendMsgGiantWanderBossBulletAttack(other, self))
        kill();
}

bool GiantWanderBossBullet::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                       al::HitSensor* self) {
    if (rs::isMsgKillByHomeDemo(message)) {
        al::killForceBeforeDemo(this);
        return true;
    }

    if (al::isSensorEnemyBody(self) && !al::isNerve(this, &AppearAttach)) {
        if (rs::isMsgCapReflect(message))
            return true;

        if (rs::isMsgHosuiAttack(message)) {
            rs::requestHitReactionToAttacker(message, self, other);

            mHitPoint--;
            if (mHitPoint <= 0)
                kill();

            return true;
        }
    }

    return false;
}

void GiantWanderBossBullet::appearAttach(const sead::Matrix34f* attachMtx,
                                         const sead::Vector3f* localOffset,
                                         bool isWaterSurfaceEffect) {
    mIsWaterSurfaceEffect = isWaterSurfaceEffect;
    mAttachMtx = attachMtx;
    mLocalOffset = localOffset;

    sead::Vector3f* trans = al::getTransPtr(this);
    const sead::Vector3f* offset = localOffset ? localOffset : &sead::Vector3f::zero;
    trans->setMul(*attachMtx, *offset);
    attachMtx->getBase(*al::getFrontPtr(this), 2);

    appear();
    al::setNerve(this, &AppearAttach);
}

void GiantWanderBossBullet::startLaunch() {
    sead::Vector3f front = al::getFront(this);

    const sead::Vector3f& playerPos = al::findNearestPlayerPos(this);
    sead::Vector3f playerDir = playerPos - al::getTrans(this);
    al::tryNormalizeOrDirZ(&playerDir);

    sead::Vector3f launchDir = front;
    launchDir.y = 0.0f;
    f32 playerDirY = playerDir.y;
    if (al::tryNormalizeOrZero(&launchDir)) {
        f32 turnDegree = 5.0f;
        if (playerDirY < 0.0f)
            turnDegree = 40.0f;

        sead::Vector3f turnDir = sead::Vector3f::zero;
        al::turnVecToVecDegree(&turnDir, launchDir, playerDir, turnDegree);
        al::tryNormalizeOrDirZ(&turnDir);

        f32 angle = al::calcAngleOnPlaneDegree(turnDir, front, sead::Vector3f::ey);
        sead::Vector3f rotatedDir = turnDir;
        al::rotateVectorDegreeY(&rotatedDir, angle);

        al::setVelocity(this, turnDir * 40.0f);
    } else {
        al::setVelocity(this, front * 40.0f);
    }

    al::setNerve(this, &Fly);
}

bool GiantWanderBossBullet::isLaunched() const {
    if (al::isNerve(this, &Fly))
        return true;

    return al::isDead(this);
}

void GiantWanderBossBullet::exeAppearAttach() {
    al::isLessEqualStep(this, 30);

    const sead::Matrix34f* attachMtx = mAttachMtx;
    const sead::Vector3f* localOffset = mLocalOffset;
    sead::Vector3f* trans = al::getTransPtr(this);
    const sead::Vector3f* offset = localOffset ? localOffset : &sead::Vector3f::zero;
    trans->setMul(*attachMtx, *offset);
    attachMtx->getBase(*al::getFrontPtr(this), 2);
}

void GiantWanderBossBullet::exeFly() {
    sead::Vector3f velocity = al::getVelocity(this);
    al::tryNormalizeOrDirZ(&velocity);
    al::turnFrontSpherical(this, al::getFrontPtr(this), velocity, 0.5f);

    mWaterSurfaceFinder->update(al::getTrans(this), sead::Vector3f::ey, 5000.0f);

    if (al::isFirstStep(this)) {
        resetPositionByAnim();
        al::startAction(this, "Fly");
        al::onCollide(this);
    }

    if (mWaterSurfaceFinder->isFoundSurface())
        al::tryAddRippleSmall(this, mWaterSurfaceFinder->getSurfacePosition());
    else
        al::tryAddRippleSmall(this);

    if (!checkCollideAndSendMsg() && !mWaterSurfaceFinder->isNearSurface(150.0f)) {
        if (al::isGreaterEqualStep(this, 600))
            kill();
        return;
    }

    if (mWaterSurfaceFinder->isNearSurface(150.0f))
        al::tryAddRippleLarge(this, mWaterSurfaceFinder->getSurfacePosition());

    kill();
}

void GiantWanderBossBullet::resetPositionByAnim() {
    sead::Vector3f jointPos = sead::Vector3f::zero;
    al::calcJointPos(&jointPos, this, "GiantWanderBossBullet");
    al::resetPosition(this, jointPos);
}

bool GiantWanderBossBullet::checkCollideAndSendMsg() {
    if (!al::isCollided(this))
        return false;

    al::HitSensor* collidedSensor = al::tryGetCollidedSensor(this);
    if (collidedSensor) {
        al::HitSensor* attackSensor = al::getHitSensor(this, "Attack");
        rs::sendMsgGiantWanderBossBulletAttack(collidedSensor, attackSensor);
    }

    return true;
}
