#include "Enemy/Pecho.h"

#include <math/seadVectorCalcCommon.h>

#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/Item/ItemUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/LiveActor/LiveActorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Movement/AnimScaleController.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Player/PlayerUtil.h"

#include "Util/ItemUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(Pecho, Appear);
NERVE_IMPL(Pecho, AttackSuccess);
NERVE_IMPL(Pecho, Wait);
NERVE_IMPL(Pecho, Find);
NERVE_IMPL(Pecho, Move);
NERVE_IMPL(Pecho, LiquidSign);
NERVE_IMPL(Pecho, LiquidStart);
NERVE_IMPL(Pecho, Liquid);
NERVE_IMPL(Pecho, LiquidEnd);
NERVE_IMPL(Pecho, Reset);

NERVES_MAKE_NOSTRUCT(Pecho, LiquidEnd, Reset);
NERVES_MAKE_STRUCT(Pecho, Wait, LiquidStart, Liquid, AttackSuccess, LiquidSign, Move, Appear, Find);
}  // namespace

const al::AnimScaleParam gAnimScaleParam = al::AnimScaleParam();

Pecho::Pecho(const char* name) : al::LiveActor(name) {}

void Pecho::init(const al::ActorInitInfo& initInfo) {
    al::initActor(this, initInfo);
    mStartingQuat = al::getQuat(this);
    mStartingTrans = al::getTrans(this);
    mBodyOrientation = al::getQuat(this);

    al::initJointControllerKeeper(this, 3);
    al::initJointGlobalQuatController(this, &mBodyOrientation, "BodyRotate");
    al::createAndSetColliderSpecialPurpose(this, "MoveLimit");
    al::setAppearItemOffset(this, sead::Vector3f::ey * 150.0f);

    mAnimScaleController = new al::AnimScaleController(&gAnimScaleParam);

    al::invalidateCollisionParts(this);
    al::initNerve(this, &NrvPecho.Wait, 0);
    al::trySyncStageSwitchAppear(this);
}

bool Pecho::receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self) {
    if (al::isMsgPlayerDisregard(message))
        return !isEnableCap();

    if (rs::isMsgBubbleAttackToPecho(message) && isEnablePush()) {
        rs::setAppearItemFactorAndOffsetByMsg(this, message, other);
        startLiquidFast();
        return true;
    }

    if (al::isMsgEnemyAttackNeedle(message) && isEnablePush()) {
        rs::setAppearItemFactorAndOffsetByMsg(this, message, other);
        startLiquid();
        return true;
    }

    if (rs::isMsgNpcScareByEnemy(message))
        return true;

    if (isEnablePush() && al::isSensorEnemyBody(self) &&
        al::tryReceiveMsgPushAndAddVelocity(this, message, other, self, 3.0f)) {
        mAnimScaleController->startAndSetScaleVelocityY(0.13f);
        al::startHitReactionHitEffect(this, "接触反射", other, self);
        mVelocity.set(al::getVelocity(this));
        mVelocity.y = 0;
        al::limitLength(&mVelocity, mVelocity, 1.0f);
        return true;
    }

    if ((rs::isMsgCapAttack(message) || rs::isMsgBlowDown(message)) &&
        al::isSensorEnemyBody(self) && isEnablePush()) {
        al::startHitReaction(this, "帽子ヒット");
        rs::setAppearItemFactorAndOffsetByMsg(this, message, other);
        rs::requestHitReactionToAttacker(message, self, other);
        startLiquidCap();
        return true;
    }

    if (rs::isMsgPlayerAndCapObjHipDropReflectAll(message) && isEnablePush()) {
        rs::setAppearItemFactorAndOffsetByMsg(this, message, other);
        startLiquid();
        return true;
    }

    if (rs::isMsgPressDown(message) && al::isSensorEnemyBody(self) && isEnablePush()) {
        al::startHitReaction(this, "踏まれ");
        rs::setAppearItemFactorAndOffsetByMsg(this, message, other);
        rs::requestHitReactionToAttacker(message, self, other);
        startLiquid();
        return true;
    }

    if (rs::isMsgBubbleGroundTouchTrigger(message) && al::isNerve(this, &NrvPecho.Liquid)) {
        mIsBubbleReaction = true;
        al::startAction(this, "LiquidBubbleReaction");
        return true;
    }

    return false;
}

void Pecho::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (isEnablePush() && al::isSensorEnemyBody(self) && al::isSensorEnemyBody(other))
        al::sendMsgPushAndKillVelocityToTarget(this, self, other);

    if (isEnableAttack() && al::isSensorEnemyAttack(self)) {
        if (isEnableSendPechoSpot()) {
            sead::Vector3f upDir;
            al::calcUpDir(&upDir, this);
            if (al::calcDistanceV(upDir, self, other) < al::getSensorRadius(other) + 20.0f)
                al::sendMsgEnemyAttackFire(other, self, "LavaRed");
            return;
        }

        if (!al::sendMsgEnemyAttack(other, self)) {
            rs::sendMsgPushToPlayer(other, self);
            return;
        }

        if (al::pushAndAddVelocity(this, other, self, 15.0f)) {
            mAnimScaleController->startAndSetScaleVelocityY(0.13f);
            al::startHitReactionHitEffect(this, "攻撃ヒット", other, self);
            al::setNerve(this, &NrvPecho.AttackSuccess);
            return;
        }
    }
}

void Pecho::control() {
    if (al::isInDeathArea(this) || al::isCollidedFloorCode(this, "DamageFire") ||
        al::isCollidedFloorCode(this, "Needle") || al::isCollidedFloorCode(this, "Poison")) {
        al::invalidateClipping(this);
        al::setVelocityZero(this);
        al::setNerve(this, &Reset);
        return;
    }

    mAnimScaleController->update();
    al::setScale(this, mAnimScaleController->getScale());
}

void Pecho::startLiquidFast() {
    al::appearItem(this);
    al::invalidateClipping(this);
    al::setVelocityZero(this);
    al::setNerve(this, &NrvPecho.LiquidSign);
    mIsStartLiquidFast = true;
    mLiquidStartAction = "LiquidStartFast";
    al::setSensorRadius(this, "BodyLiquid", 290.0);
    al::setSensorRadius(this, "AttackLiquid", 250.0);
}

void Pecho::startLiquid() {
    al::appearItem(this);
    al::invalidateClipping(this);
    al::setVelocityZero(this);
    al::setNerve(this, &NrvPecho.LiquidSign);
    mIsStartLiquidFast = false;
    mLiquidStartAction = "LiquidStart";
}

void Pecho::startLiquidCap() {
    al::appearItem(this);
    al::invalidateClipping(this);
    al::setVelocityZero(this);
    al::setNerve(this, &NrvPecho.LiquidSign);
    mIsStartLiquidFast = false;
    mLiquidStartAction = "LiquidStartCap";
}

void Pecho::updateVelocity() {
    if (al::reboundVelocityFromCollision(this, 0.75f, 5.0f, 1.0f)) {
        f32 speedV = sead::Mathf::abs(al::calcSpeedV(this));
        f32 lerpValue = al::lerpValue(speedV, 5.0f, 30.0f, 0.87f, 0.75f);
        al::startHitReaction(this, "コリジョン反射");
        mAnimScaleController->startAndSetScaleY(lerpValue);
    }

    if (al::isOnGroundNoVelocity(this, 0))
        al::addVelocityToGravityFittedGround(this, 1.0f, 0);
    else
        al::addVelocityToGravity(this, 1.0f);

    f32 scale = al::isOnGroundNoVelocity(this, 4) ? 0.9f : 0.98f;
    al::scaleVelocityHV(this, scale, 0.98f);
}

bool Pecho::tryStartFind() {
    if (!al::isNearPlayer(this, 800.0f))
        return false;

    al::invalidateClipping(this);
    sead::Vector3f dirH;
    if (al::calcDirH(&dirH, al::getTrans(this), al::getPlayerPos(this, 0)))
        mNextBodyOrientation = mStartingQuat;
    else
        al::makeQuatUpFront(&mNextBodyOrientation, sead::Vector3f::ey, dirH);

    al::setNerve(this, &NrvPecho.Find);
    return true;
}

// NON_MATCHING: regswap (https://decomp.me/scratch/D3wWz)
void Pecho::updateVelocityEscapeWallAndFall(f32 force, f32 velocity) {
    sead::Vector3f direction = sead::Vector3f::zero;
    s32 hitCount = 0;

    for (s32 i = 0; i < 8; i++) {
        f32 rad = sead::Mathf::deg2rad(i * 45.0f);
        sead::Vector3f arrowDirection(sead::Mathf::sin(rad), 0.0f, sead::Mathf::cos(rad));

        arrowDirection.rotate(al::getQuat(this));

        if (alCollisionUtil::checkStrikeArrow(this, al::getTrans(this) + 20.0f * sead::Vector3f::ey,
                                              arrowDirection * force, nullptr, nullptr) != 0) {
            direction += arrowDirection;
            hitCount++;
        } else if (alCollisionUtil::checkStrikeArrow(
                       this,
                       al::getTrans(this) + 20.0f * sead::Vector3f::ey + arrowDirection * force,
                       -70.0f * sead::Vector3f::ey, nullptr, nullptr) == 0) {
            direction += arrowDirection;
            hitCount++;
        }
    }

    if (hitCount > 0)
        al::addVelocity(this, direction * -velocity * (1.0f / hitCount));

    if (al::isOnGroundNoVelocity(this, 0))
        al::addVelocityToGravityFittedGround(this, 1.0f, 0);
    else
        al::addVelocityToGravity(this, 1.0f);

    al::scaleVelocity(this, 0.89f);
}

bool Pecho::isEnablePush() const {
    return al::isNerve(this, &NrvPecho.Wait) || al::isNerve(this, &NrvPecho.Find) ||
           al::isNerve(this, &NrvPecho.AttackSuccess) || al::isNerve(this, &NrvPecho.Move);
}

bool Pecho::isEnableAttack() const {
    return al::isNerve(this, &NrvPecho.Wait) || al::isNerve(this, &NrvPecho.Find) ||
           al::isNerve(this, &NrvPecho.Move) || al::isNerve(this, &NrvPecho.AttackSuccess) ||
           al::isNerve(this, &NrvPecho.LiquidStart) || al::isNerve(this, &NrvPecho.Liquid);
}

bool Pecho::isEnableCap() const {
    return al::isNerve(this, &NrvPecho.Appear) || al::isNerve(this, &NrvPecho.AttackSuccess) ||
           al::isNerve(this, &NrvPecho.Wait) || al::isNerve(this, &NrvPecho.Find) ||
           al::isNerve(this, &NrvPecho.Move);
}

bool Pecho::isEnableSendPechoSpot() const {
    return al::isNerve(this, &NrvPecho.LiquidStart) || al::isNerve(this, &NrvPecho.Liquid);
}

void Pecho::exeAppear() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Appear");
        al::showModelIfHide(this);
        al::validateClipping(this);
        al::onCollide(this);
    }

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvPecho.Wait);
}

void Pecho::exeAttackSuccess() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");

    updateVelocity();
    al::calcMomentRollBall(&mMoment, al::getVelocity(this), -al::getGravity(this), 100.0f);
    al::rotateQuatMoment(&mBodyOrientation, mBodyOrientation, mMoment);

    if (al::isGreaterStep(this, 60))
        al::setNerve(this, &NrvPecho.Wait);
}

void Pecho::exeWait() {
    if (al::isFirstStep(this)) {
        al::tryStartActionIfNotPlaying(this, "Wait");
        al::getRandomDirH(&mVelocity, sead::Vector3f::ey);
        mWaitTrans.set(al::getTrans(this));
        mIsWaitTiltClockwise = al::isHalfProbability();
    }

    if (al::isLessEqualStep(this, 120)) {
        sead::Vector3f vel = sead::Vector3f::zero;
        f32 degree = al::calcNerveValue(this, 120, 0.0, mIsWaitTiltClockwise ? 720.0f : -720.0f);
        al::rotateVectorDegree(&vel, mVelocity, sead::Vector3f::ey, degree);
        al::addVelocity(this, vel * 0.25f);
    } else {
        al::addVelocityDampToTarget(this, mWaitTrans, 0.03f, 0.1f);
    }

    updateVelocity();
    al::calcMomentRollBall(&mMoment, al::getVelocity(this), -al::getGravity(this), 100.0f);
    al::rotateQuatMoment(&mBodyOrientation, mBodyOrientation, mMoment);

    if (al::isGreaterEqualStep(this, 80))
        al::turnQuatYDirRadian(&mBodyOrientation, mBodyOrientation, sead::Vector3f::ey,
                               sead::Mathf::deg2rad(0.3f));

    if (!tryStartFind() && al::isGreaterEqualStep(this, 140))
        al::setNerve(this, &NrvPecho.Wait);
}

void Pecho::exeFind() {
    if (al::isFirstStep(this))
        al::startAction(this, "Find");

    al::slerpQuat(&mBodyOrientation, mBodyOrientation, mNextBodyOrientation, 0.2f);
    updateVelocity();

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvPecho.Move);
}

void Pecho::exeMove() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Move");
        mVelocity.set(sead::Vector3f::zero);
    }

    if (al::isOnGroundNoVelocity(this, 4)) {
        sead::Vector3f dirToActor;
        al::calcDirToActorH(&dirToActor, this, al::getPlayerActor(this, 0));
        mVelocity += dirToActor * 0.04f;
        al::limitLength(&mVelocity, mVelocity, 1.0f);
        al::addVelocity(this, mVelocity);
    }

    updateVelocity();
    al::calcMomentRollBall(&mMoment, al::getVelocity(this), -al::getGravity(this), 100.0f);
    al::rotateQuatMoment(&mBodyOrientation, mBodyOrientation, mMoment);
    if (!al::isNearPlayer(this, 1500.0f))
        al::setNerve(this, &NrvPecho.Wait);
}

void Pecho::exeLiquidSign() {
    if (al::isFirstStep(this)) {
        mAnimScaleController->stopAndReset();
        al::setVelocityZeroH(this);
    }
    al::addVelocityToGravity(this, 1.0);

    if (al::isOnGround(this, 0)) {
        mAnimScaleController->stopAndReset();
        sead::Quatf* quat = al::getQuatPtr(this);
        al::turnQuatYDirRate(quat, *quat, al::getOnGroundNormal(this, 0), 1.0f);
        if (!mIsStartLiquidFast) {
            al::setSensorRadius(this, "BodyLiquid", 0.0f);
            al::setSensorRadius(this, "AttackLiquid", 0.0f);
        }
        al::setVelocityZero(this);
        al::setNerve(this, &NrvPecho.LiquidStart);
    }
}

void Pecho::exeLiquidStart() {
    if (al::isFirstStep(this)) {
        al::startAction(this, mLiquidStartAction);
        al::rotateQuatYDirRandomDegree(this);
        al::validateCollisionParts(this);
    }

    sead::Vector3f jointScale;
    al::calcJointScale(&jointScale, this, "JointRoot");
    if (!mIsStartLiquidFast) {
        al::setSensorRadius(this, "BodyLiquid", jointScale.x * 290.0f);
        f32 attackLiquidRadius = mIsStartLiquidFast ? 250.0f : jointScale.x * 250.0f;
        al::setSensorRadius(this, "AttackLiquid", attackLiquidRadius);
    }

    updateVelocityEscapeWallAndFall(jointScale.x * 150.0f,
                                    al::calcNerveValue(this, 40, 80, 1.0f, 0.25f));

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvPecho.Liquid);
}

void Pecho::exeLiquid() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Liquid");
        mIsBubbleReaction = false;
    }

    if (al::isLessStep(this, 30))
        updateVelocityEscapeWallAndFall(150.0f, al::calcNerveValue(this, 30, 0.25f, 0.0f));
    else
        al::setVelocityZero(this);

    if (mIsBubbleReaction && al::isActionEnd(this)) {
        al::startAction(this, "Liquid");
        mIsBubbleReaction = false;
    }

    if (al::isGreaterEqualStep(this, 600) && !al::isNearPlayer(this, 3000.0f))
        al::setNerve(this, &LiquidEnd);
}

void Pecho::exeLiquidEnd() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "LiquidEnd");
        al::invalidateCollisionParts(this);
    }

    sead::Vector3f jointScale;
    al::calcJointScale(&jointScale, this, "JointRoot");
    al::setSensorRadius(this, "BodyLiquid", jointScale.x * 290.0f);
    al::setSensorRadius(this, "AttackLiquid", jointScale.x * 250.0f);

    if (al::isActionEnd(this))
        al::setNerve(this, &Reset);
}

void Pecho::exeReset() {
    if (al::isFirstStep(this)) {
        al::startHitReaction(this, "消滅");
        al::startAction(this, "Hide");
        al::setVelocityZero(this);
        al::offCollide(this);
        al::hideModelIfShow(this);
        al::resetQuatPosition(this, mStartingQuat, mStartingTrans);
        mBodyOrientation = mStartingQuat;
    }

    if (al::isGreaterEqualStep(this, 120))
        al::setNerve(this, &NrvPecho.Appear);
}
