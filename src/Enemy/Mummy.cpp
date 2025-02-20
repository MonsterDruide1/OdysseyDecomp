#include "Enemy/Mummy.h"

#include "Library/Area/AreaObjUtil.h"
#include "Library/Base/StringUtil.h"
#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/Effect/EffectSystemInfo.h"
#include "Library/Item/ItemUtil.h"
#include "Library/Joint/JointSpringControllerHolder.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/LiveActor/LiveActorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Math/MtxUtil.h"
#include "Library/Nature/NatureUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Player/PlayerUtil.h"
#include "Library/Stage/StageSwitchKeeper.h"
#include "Library/Thread/FunctorV0M.h"

#include "Util/ItemUtil.h"
#include "Util/PlayerUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(Mummy, Sleep)
NERVE_IMPL(Mummy, SleepShineStop)
NERVE_IMPL(Mummy, SleepShine)
NERVE_IMPL(Mummy, Walk)
NERVE_IMPL(Mummy, WalkStart)
NERVE_IMPL(Mummy, BlowDown)
NERVE_IMPL(Mummy, HalfReaction)
NERVE_IMPL(Mummy, HeadLost)
NERVE_IMPL(Mummy, Appear)
NERVE_IMPL(Mummy, AppearAir)
NERVE_IMPL(Mummy, HideStart)
NERVE_IMPL(Mummy, Wait)
NERVE_IMPL(Mummy, Hide)

NERVES_MAKE_NOSTRUCT(Mummy, Appear)
NERVES_MAKE_STRUCT(Mummy, Sleep, SleepShineStop, SleepShine, Walk, WalkStart, BlowDown,
                   HalfReaction, HeadLost, AppearAir, HideStart, Wait, Hide)
}  // namespace

const f32 sWalkSpeedHeadLost = 0.43f;
const f32 sWalkSpeedNormal = 0.76f;

Mummy::Mummy(const char* name) : al::LiveActor(name) {}

void Mummy::init(const al::ActorInitInfo& info) {
    using MummyFunctor = al::FunctorV0M<Mummy*, void (Mummy::*)()>;

    al::initActorWithArchiveName(this, info, "Mummy", nullptr);
    al::initNerve(this, &NrvMummy.Sleep, 0);
    mShineActor = rs::tryInitLinkShine(info, "ShineActor", 0);
    if (mShineActor) {
        if (al::listenStageSwitchOnStart(this, MummyFunctor(this, &Mummy::startSleep)))
            al::setNerve(this, &NrvMummy.SleepShineStop);
        else
            al::setNerve(this, &NrvMummy.SleepShine);
    }

    al::initJointControllerKeeper(this, 10);

    mJointSpringControllerHolder = new al::JointSpringControllerHolder();
    mJointSpringControllerHolder->init(this, "JointSpringControllerInfo");

    mTimeLimit = al::getRandom(0, 60) + 390;
    makeActorAlive();
}

inline void Mummy::setupEffectMatrix() {
    sead::Vector3f hitPos;
    sead::Vector3f normal = sead::Vector3f::ey;
    alCollisionUtil::getHitPosAndNormalOnArrow(
        this, &hitPos, &normal, al::getTrans(this) + sead::Vector3f(0.0f, 100.0f, 0.0f),
        sead::Vector3f(0.0f, -200.0f, 0.0f), nullptr, nullptr);

    al::makeMtxUpFrontPos(&mEffectMatrix, normal, al::getFront(this), hitPos);
}

inline void Mummy::checkEffects() {
    if (!mIsHeadLost) {
        if (--mEffectTimer == 0) {
            al::deleteEffect(this, "Core");
            al::startVisAnim(this, "Normal");
        }
    }
}

inline void Mummy::adjustVelocity() {
    bool isOnGround = al::isOnGround(this, 0);
    al::scaleVelocity(this, 0.92f);
    if (isOnGround)
        al::addVelocityToGravityFittedGround(this, 1.05f, 0);
    else
        al::addVelocityToGravity(this, 1.05f);
}

inline bool Mummy::isAsleep() const {
    return al::isNerve(this, &NrvMummy.Sleep) || al::isNerve(this, &NrvMummy.SleepShine) ||
           al::isNerve(this, &NrvMummy.SleepShineStop);
}

void Mummy::startSleep() {
    al::setNerve(this, &NrvMummy.SleepShine);
}

void Mummy::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (isHide())
        return;

    if (al::isNerve(this, &NrvMummy.SleepShineStop))
        return;

    if (al::isSensorEnemyBody(self) && al::isSensorEnemyBody(other))
        al::sendMsgPushAndKillVelocityToTarget(this, self, other);

    if (al::isSensorEnemyAttack(self))
        if (!(al::isNerve(this, &NrvMummy.Walk) || al::isNerve(this, &NrvMummy.WalkStart)) ||
            !al::sendMsgEnemyAttack(other, self))

            rs::sendMsgPushToPlayer(other, self);
}

bool Mummy::isHide() {
    return al::isNerve(this, &NrvMummy.Wait) || al::isNerve(this, &NrvMummy.Sleep) ||
           (al::isNerve(this, &NrvMummy.Hide) && al::isGreaterEqualStep(this, 60)) ||
           al::isNerve(this, &NrvMummy.BlowDown) || al::isDead(this);
}

bool Mummy::receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self) {
    if (!isHide() && rs::isMsgNpcScareByEnemy(message))
        return true;

    if (isAsleep() && al::isMsgPlayerDisregard(message))
        return true;

    if (isHide() && rs::isMsgPlayerDisregardHomingAttack(message))
        return true;

    if (isHide() || al::isNerve(this, &NrvMummy.SleepShineStop))
        return false;

    if (al::tryReceiveMsgPushAndAddVelocity(this, message, other, self, 1.4f))
        return true;

    if (rs::isMsgPressDown(message)) {
        if (mIsNoHitStop)
            al::startHitReactionHitEffect(this, "ヒットマーク", other, self);
        else
            rs::requestHitReactionToAttacker(message, self, other);

        if (mHasCoin)
            rs::setAppearItemFactorAndOffsetByMsg(this, message, other);

        al::tryDeleteEffect(this, "Core");
        al::setNerve(this, &NrvMummy.BlowDown);
        return false;
    }

    if (rs::isMsgSphinxRideAttack(message) || rs::isMsgBullAttack(message) ||
        rs::isMsgGamaneBulletThrough(message) || rs::isMsgBlowDown(message) ||
        al::isMsgExplosion(message) || rs::isMsgBossKnuckleFallAttack(message) ||
        rs::isMsgBossKnuckleIceFallToMummy(message)) {
        if (mIsNoHitStop)
            al::startHitReactionHitEffect(this, "ヒットマーク", other, self);
        else
            rs::requestHitReactionToAttacker(message, self, other);

        if (mHasCoin) {
            if (rs::isMsgCactusNeedleAttack(message))
                mHasCoin = false;
            else
                rs::setAppearItemFactorAndOffsetByMsg(this, message, other);
        }

        al::tryDeleteEffect(this, "Core");
        al::setNerve(this, &NrvMummy.BlowDown);
        return true;
    }

    if (rs::isMsgCapAttack(message) || rs::isMsgSeedAttack(message)) {
        sead::Vector3f dir;
        al::calcDirOnPlane(&dir, al::getTrans(this), rs::getPlayerPos(this), sead::Vector3f::ey);
        sead::Vector3f vel = dir;
        al::faceToDirection(this, dir);

        vel *= -35.0f;

        if (mIsHeadLost) {
            if (mInvulnerableTimer > 0)
                return false;

            if (mIsNoHitStop)
                al::startHitReactionHitEffect(this, "ヒットマーク", other, self);
            else
                rs::requestHitReactionToAttacker(message, self, other);

            al::addVelocity(this, vel);
            al::setNerve(this, &NrvMummy.HalfReaction);
            return false;
        } else {
            if (mIsNoHitStop)
                al::startHitReactionHitEffect(this, "ヒットマーク", other, self);
            else
                rs::requestHitReactionToAttacker(message, self, other);

            al::addVelocity(this, vel);
            al::setNerve(this, &NrvMummy.HeadLost);
            return false;
        }
    }

    return false;
}

void Mummy::control() {
    if (mInvulnerableTimer > 0)
        --mInvulnerableTimer;

    if (!isHide() && al::isCollidedFloorCode(this, "Needle")) {
        al::tryDeleteEffect(this, "Core");
        al::setNerve(this, &NrvMummy.BlowDown);
        mHasCoin = false;
        return;
    }

    if (al::isInDeathArea(this, al::getTrans(this)) || al::isCollidedFloorCode(this, "Poison") ||
        al::isCollidedFloorCode(this, "DamageFire") || al::isInWater(this)) {
        al::tryAddRippleLarge(this);
        al::startHitReaction(this, "死亡");
        kill();
    }

    if (!isAsleep() && al::isOnGround(this, 0)) {
        const char* collidedFloorMaterialCodeName = al::getCollidedFloorMaterialCodeName(this);
        if (collidedFloorMaterialCodeName)
            al::setMaterialCode(this, collidedFloorMaterialCodeName);
    }
}

void Mummy::setKnuckleMode() {
    mTimeLimit = al::getRandom(0, 70) + 195;
}

void Mummy::appearWithTrans(const sead::Vector3f& trans) {
    al::resetPosition(this, trans);
    al::setScale(this, sead::Vector3f::ones);
    al::LiveActor* nearestPlayerActor = al::tryFindNearestPlayerActor(this);
    if (nearestPlayerActor)
        al::faceToTarget(this, nearestPlayerActor);

    mIsHeadLost = false;
    mTimeLimit = 390;
    al::showModelIfHide(this);

    setupEffect();

    mEffectTimer = 0;
    al::tryKillEmitterAndParticleAll(this);
    al::startVisAnim(this, "Normal");
    al::startAction(this, "Appear");
    al::setNerve(this, &Appear);
    appear();
}

void Mummy::setupEffect() {
    setupEffectMatrix();
    al::setEffectFollowMtxPtr(this, "MummyAppear", &mEffectMatrix);
    al::setEffectFollowMtxPtr(this, "MummyHide", &mEffectMatrix);
}

void Mummy::appearByTreasureBox(const sead::Vector3f& trans) {
    al::resetPosition(this, trans);
    al::invalidateClipping(this);

    mIsHeadLost = false;
    al::faceToTarget(this, rs::getPlayerPos(this));
    mTimeLimit = 390;
    mHasCoin = false;
    mIsTreasureBoxSpawned = true;

    al::showModelIfHide(this);

    setupEffect();

    mEffectTimer = 0;
    al::startVisAnim(this, "Normal");
    al::setNerve(this, &NrvMummy.AppearAir);
    appear();
}

void Mummy::startWalkByTreasureBox() {
    if (al::isNerve(this, &NrvMummy.AppearAir))
        al::setNerve(this, &NrvMummy.Walk);
}

void Mummy::sleep() {
    if (isHide()) {
        Shine* shine = mShineActor;
        al::validateClipping(this);
        if (shine)
            al::setNerve(this, &NrvMummy.SleepShine);
        else
            al::setNerve(this, &NrvMummy.Sleep);
    } else if (isOnSpecialGround()) {
        al::validateClipping(this);
        al::setNerve(this, &NrvMummy.HideStart);
    }
}

void Mummy::exeSleepShineStop() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Hide");
        al::setActionFrame(this, al::getActionFrameMax(this, "Hide") - 1.0f);
    }
}

void Mummy::exeSleepShine() {
    al::validateClipping(this);

    if (al::isFirstStep(this)) {
        setupEffect();
        al::startAction(this, "Hide");
        al::startVisAnim(this, "Normal");
        al::setActionFrame(this, al::getActionFrameMax(this, "Hide") - 1.0f);
    }

    if (al::isGreaterEqualStep(this, 200)) {
        if ((rs::getPlayerPos(this) - al::getTrans(this)).length() < 1750.0f) {
            al::invalidateClipping(this);
            mIsHeadLost = false;
            al::faceToTarget(this, rs::getPlayerPos(this));
            mHasCoin = false;
            mTimeLimit = 390;
            al::setNerve(this, &Appear);
        }
    }
}

void Mummy::exeSleep() {
    if (al::isFirstStep(this)) {
        al::validateClipping(this);
        kill();
    }
}

void Mummy::exeWait() {}

void Mummy::exeAppear() {
    if (al::isFirstStep(this)) {
        al::invalidateClipping(this);
        al::tryStartActionIfNotPlaying(this, "Appear");
        if (mShineActor)
            al::emitEffect(this, "ShineGlow", nullptr);
    }

    adjustVelocity();

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvMummy.WalkStart);
}

void Mummy::exeAppearAir() {
    if (al::isFirstStep(this))
        al::startAction(this, "AppearAir");
}

void Mummy::exeWalkStart() {
    if (al::isFirstStep(this))
        al::startAction(this, "WalkStart");

    walk();

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvMummy.Walk);
}

void Mummy::walk() {
    al::LiveActor* nearestPlayerActor = al::tryFindNearestPlayerActor(this);
    if (!nearestPlayerActor || (al::calcDistanceH(this, nearestPlayerActor) > 3500.0f &&
                                !mIsTreasureBoxSpawned && isOnSpecialGround())) {
        al::setNerve(this, &NrvMummy.HideStart);
        return;
    }

    sead::Vector3f dir;
    al::calcDirToActor(&dir, this, nearestPlayerActor);
    al::rotateVectorDegreeY(&dir, mWalkDirectionOffset);
    al::turnToDirection(this, dir, 1.8f);
    if (al::isNerve(this, &NrvMummy.WalkStart)) {
        f32 nerveRate = al::calcNerveRate(this, al::getActionFrameMax(this, "WalkStart"));
        al::addVelocityToFront(this, nerveRate * nerveRate * 0.76f);
    } else {
        al::addVelocityToFront(this, mIsHeadLost ? sWalkSpeedHeadLost : sWalkSpeedNormal);
    }

    adjustVelocity();

    if (--mWalkDirectionChangeTimer <= 0) {
        mWalkDirectionOffset = al::getRandom(-25.0f, 25.0f);
        mWalkDirectionChangeTimer = al::getRandom(-30.0f, 30.0f) + 60.0f;
    }

    al::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
}

void Mummy::exeWalk() {
    if (al::isFirstStep(this))
        al::startAction(this, "Walk");

    walk();

    if (mIsHeadLost) {
        if (--mHeadRegenTimer <= 0) {
            mIsHeadLost = false;
            al::startHitReaction(this, "頭復活");
            mEffectTimer = 24;
        }
    } else
        checkEffects();

    if (--mTimeLimit <= 0 && isHideByTimeLimit())
        al::setNerve(this, &NrvMummy.HideStart);
}

inline bool Mummy::isOnSpecialGround() const {
    if (al::isOnGround(this, 0)) {
        const char* materialName = al::getCollidedFloorMaterialCodeName(this);
        if (!materialName)
            return false;
        if (al::isEqualSubString(materialName, "Sand") ||
            al::isEqualString(materialName, "ExStarCube"))
            return true;
    }

    return false;
}

bool Mummy::isHideByTimeLimit() const {
    if (mTimeLimit > 0 || mIsTreasureBoxSpawned)
        return false;

    return isOnSpecialGround();
}

void Mummy::exeHeadLost() {
    if (al::isFirstStep(this)) {
        mIsHeadLost = true;
        mHeadRegenTimer = 120;
        al::startAction(this, "Half");
        al::startHitReaction(this, "頭飛び");
        al::emitEffect(this, "Core", nullptr);
        mInvulnerableTimer = 25;
    }

    adjustVelocity();

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvMummy.Walk);
}

void Mummy::exeHalfReaction() {
    if (al::isFirstStep(this)) {
        mHeadRegenTimer = 120;
        al::startAction(this, "HalfReaction");
        mInvulnerableTimer = 25;
    }

    checkEffects();
    adjustVelocity();

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvMummy.Walk);
}

void Mummy::exeHideStart() {
    if (al::isFirstStep(this))
        al::startAction(this, "HideStart");

    adjustVelocity();

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvMummy.Hide);
}

void Mummy::exeHide() {
    if (al::isFirstStep(this)) {
        al::setVelocityZero(this);
        setupEffectMatrix();
        al::startAction(this, "Hide");
    }

    checkEffects();
    adjustVelocity();

    if (al::isActionEnd(this)) {
        if (mShineActor) {
            al::deleteEffect(this, "ShineGlow");
            al::setNerve(this, &NrvMummy.SleepShine);
        } else {
            al::setNerve(this, &NrvMummy.Sleep);
        }
    }
}

void Mummy::exeBlowDown() {
    if (al::isFirstStep(this)) {
        al::setVelocityZero(this);
        al::startAction(this, "BlowDown");
        al::hideModelIfShow(this);
    }

    if (al::isStep(this, 25)) {
        if (mShineActor) {
            rs::appearPopupShine(mShineActor, this);
            mShineActor = nullptr;
            al::deleteEffect(this, "ShineGlow");
        }
        if (mHasCoin)
            al::appearItem(this);
    }
    if (al::isActionEnd(this) && al::isGreaterEqualStep(this, 90))
        al::setNerve(this, &NrvMummy.Sleep);
}
