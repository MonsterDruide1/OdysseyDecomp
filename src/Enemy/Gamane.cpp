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
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/Math/MathUtil.h"
#include "Library/Movement/EnemyStateBlowDown.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Player/PlayerUtil.h"
#include "Library/Shadow/DepthShadowDrawer.h"

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
NERVE_IMPL(Gamane, Wait);
NERVE_IMPL(Gamane, Find);
NERVE_IMPL(Gamane, Runaway);
NERVE_IMPL(Gamane, Fall);
NERVE_IMPL(Gamane, Land);
NERVE_IMPL(Gamane, Swoon);
NERVE_IMPL(Gamane, HackStart);
NERVE_IMPL(Gamane, Hack);
NERVE_IMPL(Gamane, Trampled);
NERVE_IMPL(Gamane, PressDown);
NERVE_IMPL(Gamane, BlowDown);

NERVES_MAKE_NOSTRUCT(Gamane, Land);
NERVES_MAKE_STRUCT(Gamane, Wait, Swoon, Hack, BlowDown, HackStart, Trampled, PressDown, Fall, Find,
                   Runaway);
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

    mEnemyStateSwoon = new EnemyStateSwoon(this, "SwoonStart", "Swoon", "SwoonEnd", false, true);

    gEnemyStateSwoonInitParam._74 = 180;
    gEnemyStateSwoonInitParam._60 = "着地";
    mEnemyStateSwoon->initParams(gEnemyStateSwoonInitParam);
    mEnemyStateSwoon->set9b(true);

    al::initNerveState(this, mEnemyStateSwoon, &NrvGamane.Swoon, "気絶");

    mGamaneHackState = new GamaneHackState(this);
    al::initNerveState(this, mGamaneHackState, &NrvGamane.Hack, "憑依");
    mGamaneHackState->initialize(initInfo);

    mEnemyStateBlowDown =
        new al::EnemyStateBlowDown(this, &gEnemyStateBlowDownParam, "吹き飛び状態");
    al::initNerveState(this, mEnemyStateBlowDown, &NrvGamane.BlowDown, "吹き飛び");

    mHackerJudgeNormalFall = new HackerJudgeNormalFall(this, 5);

    mCollisionPartsFilter = new al::CollisionPartsFilterSpecialPurpose("MoveLimit");
    al::setColliderFilterCollisionParts(this, mCollisionPartsFilter);

    mEnemyStateHackStart = new EnemyStateHackStart(this, nullptr, &gPlayerHackStartShaderParam);
    al::initNerveState(this, mEnemyStateHackStart, &NrvGamane.HackStart, "ひょうい開始");

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
    mHackerDepthShadowMapCtrl = new HackerDepthShadowMapCtrl(this, "Ground", 50.0f, 0.3f, 0.5f);
}

void Gamane::attackSensor(al::HitSensor* target, al::HitSensor* source) {
    if (al::isNerve(this, &NrvGamane.PressDown) || al::isNerve(this, &NrvGamane.BlowDown))
        return;
    if (somePlayerHack != nullptr) {
        mGamaneHackState->attackSensor(target, source);
        return;
    }
    if (al::isSensorEnemyBody(target) && al::isSensorEnemyBody(source))
        al::sendMsgPushAndKillVelocityToTarget(this, target, source);
    if (al::isSensorEnemyBody(target)) {
        al::sendMsgPush(source, target);
        rs::sendMsgPushToPlayer(source, target);
    }
}

bool Gamane::receiveMsg(const al::SensorMsg* message, al::HitSensor* source,
                        al::HitSensor* target) {
    if ((!al::isSensorEnemyBody(target) || al::isNerve(this, &NrvGamane.PressDown) ||
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
            mEnemyStateSwoon->tryReceiveMsgStartLockOn(message))
            return true;
        if (mEnemyStateSwoon->tryReceiveMsgStartHack(message)) {
            startHack(message, source, target);
            al::setNerve(this, &NrvGamane.HackStart);
            return true;
        }
    }

    if (al::isNerve(this, &NrvGamane.Swoon)) {
        if (mEnemyStateSwoon->tryReceiveMsgEnableLockOn(message))
            return true;
        if (mEnemyStateSwoon->tryReceiveMsgStartHack(message)) {
            startHack(message, source, target);
            al::setNerve(this, &NrvGamane.HackStart);
            return true;
        }
        if (mEnemyStateSwoon->tryReceiveMsgEndSwoon(message))
            return true;
    }

    if (somePlayerHack != nullptr) {
        if (rs::isMsgHackMarioCheckpointFlagWarp(message)) {
            rs::endHack(&somePlayerHack);
            rs::endHackShadow(this);
            al::startVisAnim(this, "CapOff");
            al::startMtpAnim(this, "CapOff");
            al::setColliderFilterCollisionParts(this, mCollisionPartsFilter);
            return true;
        }

        bool result = mGamaneHackState->receiveMsg(message, source, target);
        if (mGamaneHackState->isHackEnd()) {
            somePlayerHack = nullptr;
            rs::endHackShadow(this);
            al::startVisAnim(this, "CapOff");
            al::startMtpAnim(this, "CapOff");
            al::setColliderFilterCollisionParts(this, mCollisionPartsFilter);
            al::setNerve(this, &NrvGamane.Swoon);
        }

        return result;
    }

    if ((al::isSensorEnemyBody(source) || al::isSensorMapObj(source)) &&
        al::tryReceiveMsgPushAndAddVelocity(this, message, source, target, 3.0f))
        return true;

    if (al::isMsgPlayerTrampleReflect(message) || rs::isMsgSenobiTrample(message)) {
        rs::requestHitReactionToAttacker(message, target, source);
        if (0 < mCoinDelay) {
            rs::tryAppearMultiCoinFromObj(this, al::getHitSensor(this, "Body"), 0, 150.0f);
            mCoinDelay--;
        }
        endRefract(25);
        if (al::isNerve(this, &NrvGamane.Swoon)) {
            mEnemyStateSwoon->requestTrampled();
            return true;
        }
        al::setNerve(this, &NrvGamane.Trampled);
        return true;
    }

    if (rs::isMsgPressDown(message) && !al::isMsgPlayerTrample(message)) {
        rs::requestHitReactionToAttacker(message, target, source);
        al::setNerve(this, &NrvGamane.PressDown);
        return true;
    }

    if (rs::isMsgBlowDown(message)) {
        if (rs::isMsgGamaneBullet(message)) {
            GamaneBullet* bullet = (GamaneBullet*)al::getSensorHost(source);
            if (bullet != nullptr && bullet->getParent() == this)
                return false;
        }

        rs::requestHitReactionToAttacker(message, target, source);
        mEnemyStateBlowDown->start(source);
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
        rs::requestHitReactionToAttacker(message, target, source);
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
    if (al::isNerve(this, &NrvGamane.Hack) && somePlayerHack != nullptr)
        mHackerDepthShadowMapCtrl->update(nullptr);
    if (!al::isNerve(this, &NrvGamane.Hack) || al::isHideModel(this))
        al::hideSilhouetteModelIfShow(this);

    else
        al::showSilhouetteModelIfHide(this);
    if (!al::isInDeathArea(this) && !al::isInWaterArea(this) &&
        !al::isCollidedFloorCode(this, "DamageFire") && !al::isCollidedFloorCode(this, "Poison"))
        return;
    if (!al::isInDeathArea(this) && somePlayerHack != nullptr)
        rs::endHack(&somePlayerHack);
    al::startHitReaction(this, "消滅");
    kill();
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
        sead::Vector3f hackVelocity = mGamaneHackState->getVelocity();
        sead::Vector3f velocity = al::getVelocity(this);
        velocity += hackVelocity;
        al::setVelocity(this, velocity);
        mGamaneHackState->setVelocity(sead::Vector3f(0.0f, 0.0f, 0.0f));
    }
    al::LiveActor::updateCollider();
}

void Gamane::startHack(const al::SensorMsg* message, al::HitSensor* source, al::HitSensor* target) {
    al::invalidateClipping(this);
    somePlayerHack = mEnemyStateHackStart->tryStart(message, source, target);
    mGamaneHackState->setPlayerHackAction(somePlayerHack);
    rs::hideShadowHackCap(somePlayerHack);
    rs::setupHackShadow(this);
    al::setColliderFilterCollisionParts(this, nullptr);
    endRefract(50);
}

void Gamane::updateRefract() {
    if (mRefractDelayTransisionTime == 0) {
        al::setShadowMaskIntensity(this, "シャドウマスク", mShadowMaskIntensity);
        return;
    }
    f32 refractPercentage = mRefractDelayTransisionTime / 50.0f;
    if (mIsStartRefract != false)
        refractPercentage = 1.0f - refractPercentage;
    al::setModelMaterialParameterF32(this, mMaterialIndex, "const_single0", refractPercentage);
    al::setModelMaterialParameterF32(this, mMaterialIndex, "const_single2",
                                     refractPercentage * 0.5);
    mRefractDelayTransisionTime--;
    if (somePlayerHack == nullptr) {
        f32 intensity = al::lerpValue(mShadowMaskIntensity, 1.0, refractPercentage);
        al::setShadowMaskIntensity(this, "シャドウマスク", intensity);
        if (mRefractDelayTransisionTime == 0) {
            if (mIsStartRefract != false) {
                al::hideShadowMask(this);
                al::validateClipping(this);
                return;
            }
            al::startVisAnim(this, "On");
            al::invalidateClipping(this);
            return;
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

void Gamane::startRefract(int delay) {
    if (mRefractDelayTransisionTime < 1 && !mIsStartRefract) {
        mRefractDelayTransisionTime = delay;
        mIsStartRefract = true;
        al::startVisAnim(this, "Off");
    }
}

void Gamane::endRefract(s32 delay) {
    if (mRefractDelayTransisionTime < 1 && mIsStartRefract) {
        mRefractDelayTransisionTime = delay;
        mIsStartRefract = false;
        al::showShadowMask(this);
    }
}

void Gamane::exeWait() {
    if (al::isFirstStep(this)) {
        al::setVelocityZero(this);
        al::startAction(this, "Wait");
        rs::resetJudge(mHackerJudgeNormalFall);
    }

    updateMovement();

    if (rs::updateJudgeAndResult(mHackerJudgeNormalFall)) {
        al::setNerve(this, &NrvGamane.Fall);
        return;
    }

    if (al::isStep(this, 0xb4))
        startRefract(50);

    if (1000.0 > al::calcDistanceH(this, al::getPlayerActor(this, 0)))
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
        rs::resetJudge(mHackerJudgeNormalFall);
        mRefractDelay = 0;
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
        mRefractDelay++;

    if (mRefractDelay == 240) {
        startRefract(50);
        mRefractDelay = 0;
    }

    if (rs::updateJudgeAndResult(mHackerJudgeNormalFall)) {
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
        sead::Vector3f frontDir = sead::Vector3f::ez;
        al::calcFrontDir(&frontDir, this);

        al::setVelocity(this, frontDir * 0.0f + sead::Vector3f(0.0f, 25.0f, 0.0f));
        mIsInLove = false;
    }

    updateMovement();

    if (al::updateNerveState(this) && !mIsInLove) {
        if (1000.0 > al::calcDistanceH(this, al::getPlayerActor(this, 0)))
            al::setNerve(this, &NrvGamane.Runaway);
        else
            al::setNerve(this, &NrvGamane.Wait);
    }
}

void Gamane::exeHackStart() {
    if (mEnemyStateHackStart->isHackStart() &&
        mEnemyStateHackStart->calcHackStartNerveRate() == 0.0f) {
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
    if (0 < mCoinDelay && mHackDelay == 0) {
        rs::tryAppearMultiCoinFromObj(this, al::getHitSensor(this, "Body"), 0, 150.0f);
        mCoinDelay--;
    }

    mHackDelay = al::modi((mHackDelay++ + 1) + 6, 6);
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
