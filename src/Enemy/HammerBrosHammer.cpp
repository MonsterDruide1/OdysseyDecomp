#include "Enemy/HammerBrosHammer.h"

#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/Collision/CollisionPartsTriangle.h"
#include "Library/Effect/EffectSystemInfo.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Shadow/ActorShadowUtil.h"

#include "Util/AreaUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(HammerBrosHammer, Attach);
NERVE_IMPL(HammerBrosHammer, Move);
NERVE_IMPL(HammerBrosHammer, Break);

NERVES_MAKE_STRUCT(HammerBrosHammer, Move, Break);
NERVES_MAKE_NOSTRUCT(HammerBrosHammer, Attach);
}  // namespace

HammerBrosHammer::HammerBrosHammer(const char* name, const al::LiveActor* bros,
                                   const char* archiveName, bool isSearch)
    : BrosWeaponBase(name, bros), mArchiveName(archiveName), mIsSearch(isSearch) {}

void HammerBrosHammer::init(const al::ActorInitInfo& initInfo) {
    al::initActorWithArchiveName(this, initInfo, mArchiveName, nullptr);
    al::initNerve(this, &NrvHammerBrosHammer.Move, 0);
    al::setEffectNamedMtxPtr(this, "LavaSurface", &mSurfaceMtx);
    al::invalidateHitSensors(this);
    al::offCollide(this);
    if (!mIsSearch)
        al::invalidateHitSensor(this, "Search");
    makeActorDead();
}

// NON_MATCHING: Register swap https://decomp.me/scratch/VjUUZ
void HammerBrosHammer::shoot(const sead::Vector3f& trans, const sead::Quatf& quat,
                             const sead::Vector3f& force, bool isHack, s32 unknown, bool isFast) {
    al::showModelIfHide(this);

    mIsHack = isHack;
    if (mIsHack)
        al::validateHitSensor(this, "AttackHack");
    else
        al::validateHitSensor(this, "Attack");

    if (mIsHack && mIsSearch) {
        mSearchCooldown = 100;
        al::invalidateHitSensor(this, "Search");
    } else {
        al::validateHitSensor(this, "Search");
    }

    al::setQuat(this, quat);
    al::invalidateClipping(this);
    al::onCollide(this);

    sead::Vector3f frontDir = sead::Vector3f::zero;
    sead::Vector3f upDir = sead::Vector3f::zero;
    al::resetPosition(this, trans);
    al::calcFrontDir(&frontDir, this);
    al::calcUpDir(&upDir, this);

    sead::Vector3f sideDir;
    sideDir.setCross(upDir, frontDir);

    al::Triangle triangle;
    sead::Vector3f hitPos = sead::Vector3f::zero;
    bool isColliding = alCollisionUtil::getFirstPolyOnArrow(
        this, &hitPos, &triangle, al::getTrans(this), frontDir * 100.0f, nullptr, nullptr);

    if (!isHack) {
        sead::Vector3f frontVelocity = frontDir * (rs::isInLowGravityArea(this) ? 20.0f : 25.0f);
        sead::Vector3f upVelocity = upDir * 50.0f;
        al::setVelocity(this, frontVelocity + upVelocity);
        al::showShadow(this);
        appear();
        al::setNerve(this, &NrvHammerBrosHammer.Move);
        return;
    }

    f32 randomVal = al::getRandom(0.0f, 20.0f) - 10.0f;
    bool isInLowGravityArea = rs::isInLowGravityArea(this);
    f32 fastSpeed = isInLowGravityArea ? 60.0f : 40.0f;
    f32 baseSpeed = isInLowGravityArea ? 25.0f : 20.0f;
    f32 speed = randomVal + (isFast ? fastSpeed : baseSpeed);

    if (rs::isInLowGravityArea(this)) {
        f32 sideRandom = al::getRandom(0.0f, 5.0f) - 2.5f;

        sead::Vector3f frontVelocity = frontDir * speed;
        sead::Vector3f upVelocity = upDir * 40.0f;
        sead::Vector3f sideVelocity = sideDir * sideRandom;

        al::setVelocity(this, frontVelocity + upVelocity + sideVelocity +
                                  sead::Vector3f(force.x, 0.0f, force.z));
    } else {
        f32 scale = sead::Mathi::clampMax(unknown, 5) * 8.0f;
        f32 sideRandom = al::getRandom(0.0f, 23.0f) - 11.5f;

        sead::Vector3f frontVelocity = frontDir * speed;
        sead::Vector3f upVelocity = upDir * scale;
        sead::Vector3f sideVelocity = sideDir * sideRandom;

        al::setVelocity(this, frontVelocity + upVelocity + sideVelocity +
                                  sead::Vector3f(force.x, 0.0f, force.z));
    }

    al::faceToVelocity(this);

    sead::Vector3f brosVel = al::getVelocity(getBrosActor());
    brosVel.y = 0.0f;
    al::addVelocity(this, brosVel);

    if (isColliding)
        al::getTransPtr(this)->y += 80.0f;

    al::showShadow(this);
    appear();
    al::setNerve(this, &NrvHammerBrosHammer.Move);
}

void HammerBrosHammer::killCollide(al::HitSensor* sensor, const sead::Vector3f& trans,
                                   bool isHack) {
    al::setTrans(this, trans);
    appear();

    if (isHack)
        rs::sendMsgHammerBrosHammerHackAttack(sensor, al::getHitSensor(this, "Attack"));
    else
        rs::sendMsgHammerBrosHammerEnemyAttack(sensor, al::getHitSensor(this, "Attack"));

    al::startHitReaction(this, "コリジョン衝突");
    HammerBrosHammer::killEnemy();
}

void HammerBrosHammer::killEnemy() {
    al::setNerve(this, &NrvHammerBrosHammer.Break);
    al::hideModelIfShow(this);
}

void HammerBrosHammer::attach(const sead::Matrix34f* poseMtx, const sead::Vector3f& trans,
                              const sead::Vector3f& rotation, const char* actionName) {
    BrosWeaponBase::attach(poseMtx, trans, rotation, actionName);
    al::showModelIfHide(this);
    al::hideShadow(this);
    al::invalidateHitSensor(this, "Attack");
    al::invalidateHitSensor(this, "AttackHack");
    al::setNerve(this, &Attach);
}

bool HammerBrosHammer::isBreak() const {
    return al::isNerve(this, &NrvHammerBrosHammer.Break);
}

u32 HammerBrosHammer::getBreakStep() const {
    if (HammerBrosHammer::isBreak())
        return al::getNerveStep(this);
    return -1;
}

void HammerBrosHammer::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::getSensorHost(other) == getBrosActor() || !al::isNerve(this, &NrvHammerBrosHammer.Move))
        return;

    bool isKillEnemy = false;
    if (!mIsHack) {
        isKillEnemy =
            al::isSensorEnemyAttack(self) && rs::sendMsgHammerBrosHammerEnemyAttack(other, self);
    } else if (al::isSensorPlayerAttack(self)) {
        if (rs::sendMsgWeaponItemGet(other, self))
            return;
        isKillEnemy = rs::sendMsgHammerBrosHammerHackAttack(other, self) ||
                      rs::sendMsgHackAttack(other, self);
    } else {
        isKillEnemy = !mIsHack && al::isSensorEnemyAttack(self) &&
                      rs::sendMsgHammerBrosHammerEnemyAttack(other, self);
    }

    if (isKillEnemy) {
        HammerBrosHammer::killEnemy();
        return;
    }

    if (mIsSearch && al::isSensorEye(self) && rs::sendMsgHammerBrosHammerSearch(other, self)) {
        const sead::Vector3f& selfPos = al::getSensorPos(self);
        const sead::Vector3f& otherPos = al::getSensorPos(other);
        f32 invDistance = 1.0f / sead::Mathf::sqrt(sead::Mathf::pow(selfPos.x - otherPos.x, 2) +
                                                   sead::Mathf::pow(selfPos.z - otherPos.z, 2));
        if (!(otherPos.y > selfPos.y)) {
            f32 velY = invDistance * 500.0f;
            al::addVelocityY(this, velY * -2.0f);
        }
    }
}

bool HammerBrosHammer::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                  al::HitSensor* self) {
    if (rs::isMsgPlayerDisregardTargetMarker(message))
        return true;

    if (HammerBrosHammer::isBreak())
        return false;

    if (rs::isMsgCapAttack(message)) {
        al::startHitReaction(this, "コリジョン衝突");
        HammerBrosHammer::killEnemy();
        return false;
    }

    if ((rs::isMsgHammerBrosHammerHackAttack(message) && !mIsHack) ||
        rs::isMsgHammerBrosHammerEnemyAttack(message)) {
        rs::requestHitReactionToAttacker(message, self, other);
        al::startHitReaction(this, "コリジョン衝突");
        HammerBrosHammer::killEnemy();
        return true;
    }

    return false;
}

void HammerBrosHammer::exeAttach() {
    sead::Matrix34f attachMtx;
    BrosWeaponBase::calcAttachMtx(&attachMtx, getPoseMtx(), getTrans(), getRotation());
    al::updatePoseMtx(this, &attachMtx);
}

void HammerBrosHammer::exeMove() {
    if (al::isFirstStep(this))
        al::startAction(this, "Spin");

    al::HitSensor* hitSensor = al::getHitSensor(this, "Attack");
    if (al::isCollidedGround(this)) {
        al::HitSensor* groundSensor = al::tryGetCollidedGroundSensor(this);
        if (groundSensor != nullptr)
            al::sendMsgPlayerTouch(groundSensor, hitSensor);
        if (al::isCollidedGroundFloorCode(this, "DamageFire")) {
            mSurfaceMtx.setTranslation(al::getCollidedGroundPos(this));
            al::startHitReaction(this, "ダメージ床(火)");
        }
    }

    al::HitSensor* collidedSensor = al::tryGetCollidedSensor(this);
    if (collidedSensor == nullptr) {
        if (!al::isInDeathArea(this)) {
            if (al::isGreaterEqualStep(this, 300)) {
                kill();
                return;
            }
            if (mSearchCooldown > 0)
                mSearchCooldown--;

            sead::Quatf quat = al::getQuat(this);
            al::rotateQuatXDirDegree(&quat, quat, 20.0f);
            al::setQuat(this, quat);

            al::scaleVelocity(this, rs::isInLowGravityArea(this) ? 0.985f : 0.98f);
            al::addVelocityToGravity(this, rs::isInLowGravityArea(this) ? 1.0f : 2.0f);
            return;
        }
    } else {
        bool isKillEnemy;
        if (mIsHack)
            isKillEnemy = rs::sendMsgHammerBrosHammerHackAttack(collidedSensor, hitSensor);
        else
            isKillEnemy = rs::sendMsgHammerBrosHammerEnemyAttack(collidedSensor, hitSensor);

        if (isKillEnemy) {
            HammerBrosHammer::killEnemy();
            return;
        }
    }
    al::startHitReaction(this, "コリジョン衝突");
    HammerBrosHammer::killEnemy();
}

void HammerBrosHammer::exeBreak() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Wait");
        al::setVelocityZero(this);
        al::invalidateHitSensors(this);
        al::hideModelIfShow(this);
    }

    if (al::isGreaterEqualStep(this, 50))
        kill();
}
