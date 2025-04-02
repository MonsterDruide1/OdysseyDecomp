#include "Enemy/Gamane.h"

#include <math/seadVector.h>

#include "Library/Collision/KCollisionServer.h"
#include "Library/Effect/EffectSystemInfo.h"
#include "Library/Item/ItemUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Movement/EnemyStateBlowDown.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Player/PlayerUtil.h"
#include "Library/Shadow/ActorShadowUtil.h"

#include "Enemy/EnemyStateHackStart.h"
#include "Enemy/EnemyStateSwoon.h"
#include "Enemy/GamaneBullet.h"
#include "Enemy/GamaneHackState.h"
#include "Enemy/HackerDepthShadowMapCtrl.h"
#include "Player/HackerJudge.h"
#include "Player/PlayerHackStartShaderCtrl.h"
#include "Util/Hack.h"
#include "Util/ItemUtil.h"
#include "Util/JudgeUtil.h"
#include "Util/SensorMsgFunction.h"
#include "Util/ShadowUtil.h"

namespace {
NERVE_IMPL(Gamane, Wait)
NERVE_IMPL(Gamane, Find)
NERVE_IMPL(Gamane, Runaway)
NERVE_IMPL(Gamane, Fall)
NERVE_IMPL(Gamane, Land)
NERVE_IMPL(Gamane, Swoon)
NERVE_IMPL(Gamane, HackStart)
NERVE_IMPL(Gamane, Hack)
NERVE_IMPL(Gamane, Trampled)
NERVE_IMPL(Gamane, PressDown)
NERVE_IMPL(Gamane, BlowDown)

NERVES_MAKE_NOSTRUCT(Gamane, Land)
NERVES_MAKE_STRUCT(Gamane, Wait, Swoon, Hack, BlowDown, HackStart, Trampled, PressDown, Fall, Find,
                   Runaway)
}  // namespace

static al::EnemyStateBlowDownParam gEnemyStateBlowDownParam =
    al::EnemyStateBlowDownParam("BlowDown", 18.0f, 35.0f, 1.0f, 0.98f, 120, true);

static PlayerHackStartShaderParam gPlayerHackStartShaderParam =
    PlayerHackStartShaderParam(false, 300.0f, 10, 20);

static EnemyStateSwoonInitParam gEnemyStateSwoonInitParam = EnemyStateSwoonInitParam(
    "SwoonStart", "Swoon", "SwoonEnd", nullptr, "SwoonStartFall", "SwoonStartLand");

Gamane::Gamane(const char* name) : al::LiveActor(name) {}

void Gamane::init(const al::ActorInitInfo& initInfo) {
    al::initActorWithArchiveName(this, initInfo, "Gamane", nullptr);
    al::initNerve(this, &NrvGamane.Wait, 5);
    mCapTargetInfo = rs::createCapTargetInfo(this, nullptr);

    mStateSwoon = new EnemyStateSwoon(this, "SwoonStart", "Swoon", "SwoonEnd", false, true);

    gEnemyStateSwoonInitParam.swoonDuration = 180;
    gEnemyStateSwoonInitParam.hitReactionAnimName = "着地";
    mStateSwoon->initParams(gEnemyStateSwoonInitParam);
    mStateSwoon->enableLockOnDelay(true);

    al::initNerveState(this, mStateSwoon, &NrvGamane.Swoon, "気絶");

    mHackState = new GamaneHackState(this);
    al::initNerveState(this, mHackState, &NrvGamane.Hack, "憑依");
    mHackState->initialize(initInfo);

    mStateBlowDown = new al::EnemyStateBlowDown(this, &gEnemyStateBlowDownParam, "吹き飛び状態");
    al::initNerveState(this, mStateBlowDown, &NrvGamane.BlowDown, "吹き飛び");

    mJudgeNormalFall = new HackerJudgeNormalFall(this, 5);

    mCollisionPartsFilter = new al::CollisionPartsFilterSpecialPurpose("MoveLimit");
    al::setColliderFilterCollisionParts(this, mCollisionPartsFilter);

    mStateHackStart = new EnemyStateHackStart(this, nullptr, &gPlayerHackStartShaderParam);
    al::initNerveState(this, mStateHackStart, &NrvGamane.HackStart, "ひょうい開始");

    al::setMaterialProgrammable(this);
    mMaterialIndex = al::getMaterialIndex(this, "TransMT");
    mShadowMaskIntensity = al::getShadowMaskIntensity(this, "シャドウマスク");
    al::hideShadowMask(this);

    al::startVisAnim(this, "Off");
    al::startVisAnim(this, "CapOff");
    al::startMtpAnim(this, "CapOff");
    al::setModelMaterialParameterF32(this, mMaterialIndex, "const_single0", 1.0f);
    makeActorAlive();

    rs::initHackShadow(this);
    mDepthShadowMapCtrl = new HackerDepthShadowMapCtrl(this, "Ground", 50.0f, 0.3f, 0.5f);
}

void Gamane::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isNerve(this, &NrvGamane.PressDown) || al::isNerve(this, &NrvGamane.BlowDown))
        return;

    if (mPlayerHack != nullptr) {
        mHackState->attackSensor(self, other);
        return;
    }

    if (al::isSensorEnemyBody(self) && al::isSensorEnemyBody(other))
        al::sendMsgPushAndKillVelocityToTarget(this, self, other);

    if (al::isSensorEnemyBody(self)) {
        al::sendMsgPush(other, self);
        rs::sendMsgPushToPlayer(other, self);
    }
}

bool Gamane::receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self) {
    if ((!al::isSensorEnemyBody(self) || al::isNerve(this, &NrvGamane.PressDown) ||
         al::isNerve(this, &NrvGamane.BlowDown)) &&
        (al::isMsgPlayerDisregard(message) || rs::isMsgPlayerDisregardHomingAttack(message) ||
         rs::isMsgPlayerDisregardTargetMarker(message)))
        return true;

    if (al::isNerve(this, &NrvGamane.PressDown))
        return false;

    if (al::isNerve(this, &NrvGamane.BlowDown))
        return false;

    if (rs::tryReceiveMsgInitCapTargetAndSetCapTargetInfo(message, mCapTargetInfo))
        return true;

    if (!al::isNerve(this, &NrvGamane.Hack) && !al::isNerve(this, &NrvGamane.HackStart) &&
        !al::isNerve(this, &NrvGamane.Swoon)) {
        if (rs::isMsgCapEnableLockOn(message) || rs::isMsgCapCancelLockOn(message) ||
            mStateSwoon->tryReceiveMsgStartLockOn(message))
            return true;
        if (mStateSwoon->tryReceiveMsgStartHack(message)) {
            startHack(message, other, self);
            al::setNerve(this, &NrvGamane.HackStart);
            return true;
        }
    }

    if (al::isNerve(this, &NrvGamane.Swoon)) {
        if (mStateSwoon->tryReceiveMsgEnableLockOn(message))
            return true;
        if (mStateSwoon->tryReceiveMsgStartHack(message)) {
            startHack(message, other, self);
            al::setNerve(this, &NrvGamane.HackStart);
            return true;
        }
        if (mStateSwoon->tryReceiveMsgEndSwoon(message))
            return true;
    }

    if (mPlayerHack != nullptr) {
        if (rs::isMsgHackMarioCheckpointFlagWarp(message)) {
            rs::endHack(&mPlayerHack);
            rs::endHackShadow(this);
            al::startVisAnim(this, "CapOff");
            al::startMtpAnim(this, "CapOff");
            al::setColliderFilterCollisionParts(this, mCollisionPartsFilter);
            return true;
        }

        bool result = mHackState->receiveMsg(message, other, self);
        if (mHackState->isHackEnd()) {
            mPlayerHack = nullptr;
            rs::endHackShadow(this);
            al::startVisAnim(this, "CapOff");
            al::startMtpAnim(this, "CapOff");
            al::setColliderFilterCollisionParts(this, mCollisionPartsFilter);
            al::setNerve(this, &NrvGamane.Swoon);
        }

        return result;
    }

    if ((al::isSensorEnemyBody(other) || al::isSensorMapObj(other)) &&
        al::tryReceiveMsgPushAndAddVelocity(this, message, other, self, 3.0f))
        return true;

    if (al::isMsgPlayerTrampleReflect(message) || rs::isMsgSenobiTrample(message)) {
        rs::requestHitReactionToAttacker(message, self, other);
        if (mCoinsLeft > 0) {
            rs::tryAppearMultiCoinFromObj(this, al::getHitSensor(this, "Body"), 0, 150.0f);
            mCoinsLeft--;
        }
        endRefract(25);
        if (al::isNerve(this, &NrvGamane.Swoon)) {
            mStateSwoon->requestTrampled();
            return true;
        }
        al::setNerve(this, &NrvGamane.Trampled);
        return true;
    }

    if (rs::isMsgPressDown(message) && !al::isMsgPlayerTrample(message)) {
        rs::requestHitReactionToAttacker(message, self, other);
        al::setNerve(this, &NrvGamane.PressDown);
        return true;
    }

    if (rs::isMsgBlowDown(message)) {
        if (rs::isMsgGamaneBullet(message)) {
            GamaneBullet* bullet = (GamaneBullet*)al::getSensorHost(other);
            if (bullet != nullptr && bullet->getParent() == this)
                return false;
        }

        rs::requestHitReactionToAttacker(message, self, other);
        mStateBlowDown->start(other);
        al::setNerve(this, &NrvGamane.BlowDown);
        return true;
    }

    if (rs::isMsgKillByShineGet(message) || rs::isMsgKillByHomeDemo(message)) {
        al::tryKillEmitterAndParticleAll(this);
        makeActorDead();
        return true;
    }

    if (rs::tryReceiveMsgNpcScareByEnemyIgnoreTargetHack(message, mCapTargetInfo))
        return true;

    if (al::isMsgEnemyAttackFire(message)) {
        rs::requestHitReactionToAttacker(message, self, other);
        return true;
    }

    return false;
}

void Gamane::control() {
    updateRefract();
    if (al::isCollidedGround(this)) {
        al::setMaterialCode(this, al::getCollidedFloorMaterialCodeName(this));
        al::updateMaterialCodePuddle(this);
    }

    if (al::isNerve(this, &NrvGamane.Hack) && mPlayerHack != nullptr)
        mDepthShadowMapCtrl->update(nullptr);

    if (!al::isNerve(this, &NrvGamane.Hack) || al::isHideModel(this))
        al::hideSilhouetteModelIfShow(this);
    else
        al::showSilhouetteModelIfHide(this);

    if (al::isInDeathArea(this) || al::isInWaterArea(this) ||
        al::isCollidedFloorCode(this, "DamageFire") || al::isCollidedFloorCode(this, "Poison")) {
        if (!al::isInDeathArea(this) && mPlayerHack != nullptr)
            rs::endHack(&mPlayerHack);

        al::startHitReaction(this, "消滅");
        kill();
    }
}

void Gamane::endClipped() {
    al::LiveActor::endClipped();
    if (mIsStartRefract)
        al::hideShadowMask(this);
}

void Gamane::updateCollider() {
    if (al::isNerve(this, &NrvGamane.HackStart))
        return;

    if (al::isNerve(this, &NrvGamane.Hack)) {
        sead::Vector3f hackVelocity = mHackState->getVelocity();
        sead::Vector3f velocity = al::getVelocity(this);
        velocity += hackVelocity;
        al::setVelocity(this, velocity);
        mHackState->setVelocity(sead::Vector3f(0.0f, 0.0f, 0.0f));
    }

    al::LiveActor::updateCollider();
}

void Gamane::startHack(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self) {
    al::invalidateClipping(this);
    mPlayerHack = mStateHackStart->tryStart(message, other, self);
    mHackState->setPlayerHackAction(mPlayerHack);
    rs::hideShadowHackCap(mPlayerHack);
    rs::setupHackShadow(this);
    al::setColliderFilterCollisionParts(this, nullptr);
    endRefract(50);
}

void Gamane::updateRefract() {
    if (mRefractTransitionTime == 0) {
        al::setShadowMaskIntensity(this, "シャドウマスク", mShadowMaskIntensity);
        return;
    }

    f32 refractPercentage = mRefractTransitionTime / 50.0f;
    if (mIsStartRefract != false)
        refractPercentage = 1.0f - refractPercentage;
    al::setModelMaterialParameterF32(this, mMaterialIndex, "const_single0", refractPercentage);
    al::setModelMaterialParameterF32(this, mMaterialIndex, "const_single2",
                                     refractPercentage * 0.5);
    mRefractTransitionTime--;

    if (mPlayerHack != nullptr)
        return;

    f32 intensity = al::lerpValue(mShadowMaskIntensity, 1.0, refractPercentage);
    al::setShadowMaskIntensity(this, "シャドウマスク", intensity);
    if (mRefractTransitionTime == 0) {
        if (mIsStartRefract) {
            al::hideShadowMask(this);
            al::validateClipping(this);
        } else {
            al::startVisAnim(this, "On");
            al::invalidateClipping(this);
        }
    }
}

void Gamane::updateMovement() {
    sead::Vector3f gravityDir = sead::Vector3f(0.0f, 0.0f, 0.0f);
    al::calcGravityDir(&gravityDir, this);

    if (al::isOnGround(this, 0))
        al::scaleVelocityDirection(this, gravityDir, 0.0f);

    al::scaleVelocityParallelVertical(this, gravityDir, 0.99f, 0.1f);
    al::addVelocityToGravityNaturalOrFittedGround(this, 1.5f);
}

void Gamane::startRefract(s32 transitionTime) {
    if (mRefractTransitionTime < 1 && !mIsStartRefract) {
        mRefractTransitionTime = transitionTime;
        mIsStartRefract = true;
        al::startVisAnim(this, "Off");
    }
}

void Gamane::endRefract(s32 transitionTime) {
    if (mRefractTransitionTime < 1 && mIsStartRefract) {
        mRefractTransitionTime = transitionTime;
        mIsStartRefract = false;
        al::showShadowMask(this);
    }
}

void Gamane::exeWait() {
    if (al::isFirstStep(this)) {
        al::setVelocityZero(this);
        al::startAction(this, "Wait");
        rs::resetJudge(mJudgeNormalFall);
    }

    updateMovement();

    if (rs::updateJudgeAndResult(mJudgeNormalFall)) {
        al::setNerve(this, &NrvGamane.Fall);
        return;
    }

    if (al::isStep(this, 180))
        startRefract(50);

    if (al::calcDistanceH(this, al::getPlayerActor(this, 0)) < 1000.0f)
        al::setNerve(this, &NrvGamane.Find);
}

void Gamane::exeFind() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Find");
        al::setVelocityZero(this);
        al::faceToTarget(this, al::getPlayerActor(this, 0));
        endRefract(50);
    }

    updateMovement();

    if (al::isActionEnd(this)) {
        al::startVisAnim(this, "On");
        al::setNerve(this, &NrvGamane.Runaway);
    }
}

void Gamane::exeRunaway() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Run");
        rs::resetJudge(mJudgeNormalFall);
        mRunAwayRefractDelay = 0;
    }

    sead::Vector3f dirToActor;
    al::calcDirToActorH(&dirToActor, this, al::getPlayerActor(this, 0));
    dirToActor.x = -dirToActor.x;
    dirToActor.y = -dirToActor.y;
    dirToActor.z = -dirToActor.z;
    al::turnToDirection(this, dirToActor, 4.0f);

    sead::Vector3f quatFront;
    al::calcQuatFront(&quatFront, this);
    al::addVelocityToDirection(this, quatFront, 0.4f);

    sead::Vector3f gravityDir = sead::Vector3f(0.0f, 0.0f, 0.0f);
    al::calcGravityDir(&gravityDir, this);

    if (al::isOnGround(this, 0))
        al::scaleVelocityDirection(this, gravityDir, 0.0f);

    al::scaleVelocityParallelVertical(this, gravityDir, 0.99f, 0.95f);
    al::addVelocityToGravityNaturalOrFittedGround(this, 1.8f);

    if (!mIsStartRefract)
        mRunAwayRefractDelay++;

    if (mRunAwayRefractDelay == 240) {
        startRefract(50);
        mRunAwayRefractDelay = 0;
    }

    if (rs::updateJudgeAndResult(mJudgeNormalFall)) {
        al::setNerve(this, &NrvGamane.Fall);
        return;
    }

    if (al::calcDistanceH(this, al::getPlayerActor(this, 0)) > 1500.0f)
        al::setNerve(this, &NrvGamane.Wait);
}

void Gamane::exeFall() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Fall");
        al::invalidateClipping(this);
    }

    al::addVelocityToGravity(this, 0.7f);
    al::scaleVelocity(this, 0.99f);

    if (al::isOnGround(this, 0)) {
        al::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
        al::setNerve(this, &Land);
        al::startHitReaction(this, "着地");
    }
}

void Gamane::exeLand() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Land");
        al::setVelocityZero(this);
    }

    updateMovement();

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvGamane.Wait);
}

void Gamane::exeSwoon() {
    if (al::isFirstStep(this)) {
        sead::Vector3f velocity = sead::Vector3f(0.0f, 25.0f, 0.0f);
        sead::Vector3f frontDir = sead::Vector3f::ez;
        al::calcFrontDir(&frontDir, this);
        velocity += frontDir * 0.0f;
        al::setVelocity(this, velocity);
        mIsKeepSwoon = false;
    }

    updateMovement();

    if (al::updateNerveState(this) && !mIsKeepSwoon) {
        if (al::calcDistanceH(this, al::getPlayerActor(this, 0)) < 1000.0f)
            al::setNerve(this, &NrvGamane.Runaway);
        else
            al::setNerve(this, &NrvGamane.Wait);
    }
}

void Gamane::exeHackStart() {
    if (mStateHackStart->isHackStart() && mStateHackStart->calcHackStartNerveRate() == 0.0f) {
        al::startVisAnim(this, "CapOn");
        al::startMtpAnim(this, "CapOn");
    }

    updateMovement();

    if (al::updateNerveState(this)) {
        al::startVisAnim(this, "On");
        al::setNerve(this, &NrvGamane.Hack);
    }
}

void Gamane::exeHack() {
    al::updateNerveState(this);
    if (mCoinsLeft > 0 && mHackCoinAppearCounter == 0) {
        rs::tryAppearMultiCoinFromObj(this, al::getHitSensor(this, "Body"), 0, 150.0f);
        mCoinsLeft--;
    }

    mHackCoinAppearCounter = al::modi((mHackCoinAppearCounter++ + 1) + 6, 6);
}

void Gamane::exeTrampled() {
    if (al::isFirstStep(this)) {
        al::setVelocityZero(this);
        al::startAction(this, "Trampled");
    }

    updateMovement();

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvGamane.Runaway);
}

void Gamane::exePressDown() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "PressDown");
        al::invalidateClipping(this);
        endRefract(50);
    }

    updateMovement();

    if (al::isActionEnd(this)) {
        al::startHitReaction(this, "死亡");
        al::appearItemTiming(this, "倒す");
        kill();
    }
}

void Gamane::exeBlowDown() {
    if (al::isFirstStep(this))
        endRefract(50);

    if (al::updateNerveState(this)) {
        al::startHitReaction(this, "死亡");
        al::appearItemTiming(this, "倒す");
        kill();
    }
}
