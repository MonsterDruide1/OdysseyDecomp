#include "Item/Coin.h"

#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/Collision/CollisionPartsTriangle.h"
#include "Library/Collision/PartsConnector.h"
#include "Library/Controller/PadRumbleFunction.h"
#include "Library/Demo/DemoFunction.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nature/NatureUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Se/SeFunction.h"
#include "Library/Shadow/ActorShadowUtil.h"
#include "Library/Stage/StageSwitchUtil.h"

#include "Item/CoinRotateCalculator.h"
#include "Item/CoinStateAppearRotate.h"
#include "System/GameDataFunction.h"
#include "System/GameDataUtil.h"
#include "Util/AreaUtil.h"
#include "Util/DemoUtil.h"
#include "Util/ExternalForceKeeper.h"
#include "Util/SensorMsgFunction.h"
#include "Util/ShadowUtil.h"
#include "Util/WaterSurfaceShadow.h"

namespace {
NERVE_IMPL_(Coin, WaitConnectMtx, Wait)
NERVE_IMPL(Coin, Wait)
NERVE_IMPL(Coin, Appear)
NERVE_IMPL(Coin, AppearCoinLead)
NERVE_IMPL_(Coin, WaitPlayerDead, Wait)
NERVE_IMPL(Coin, Got)
NERVE_IMPL_(Coin, WaitCircle, Wait)
NERVE_IMPL_(Coin, PopUpNormal, PopUp)
NERVE_IMPL(Coin, CountUp)
NERVE_IMPL_(Coin, CountUpFive, CountUp)
NERVE_IMPL(Coin, CountUpDelay)
NERVE_IMPL_(Coin, WaitCoinRail, Wait)
NERVE_IMPL_(Coin, PopUpPlayerDeadTimeBalloon, PopUp)
NERVE_IMPL_(Coin, PopUpPlayerDead, PopUp)
NERVE_IMPL_(Coin, AppearOnDemo, Appear)
NERVE_IMPL_(Coin, WaitOnDemo, Wait)
NERVE_IMPL_(Coin, GotNoCoin, Got)
NERVE_IMPL(Coin, WaitOnDemoEnd)
NERVE_IMPL(Coin, BlowUpDelay)

NERVES_MAKE_NOSTRUCT(Coin, BlowUpDelay)
NERVES_MAKE_STRUCT(Coin, WaitConnectMtx, Wait, Appear, AppearCoinLead, WaitPlayerDead, Got,
                   WaitCircle, PopUpNormal, CountUp, CountUpFive, CountUpDelay, WaitCoinRail,
                   PopUpPlayerDeadTimeBalloon, PopUpPlayerDead, AppearOnDemo, WaitOnDemo, GotNoCoin,
                   WaitOnDemoEnd)
}  // namespace

const sead::Vector3f sAppearAboveVelocity(0.0f, 25.0f, 0.0f);

Coin::Coin(const char* name, bool isDemo) : al::LiveActor(name), mIsDemo(isDemo) {}

void Coin::init(const al::ActorInitInfo& initInfo) {
    const char* modelName = "Coin";
    if (al::isPlaced(initInfo)) {
        mIsPlaced = true;
        alPlacementFunction::tryGetModelName(&modelName, initInfo);
    } else {
        mIsPlaced = false;
    }

    const char* suffix = mIsDemo ? "MoveDemo" : nullptr;
    al::initActorWithArchiveName(this, initInfo, modelName, suffix);

    bool isConnectToCollision = false;
    al::tryGetArg(&isConnectToCollision, initInfo, "IsConnectToCollision");
    if (!isConnectToCollision)
        al::initNerve(this, &NrvCoin.Wait, 1);

    else
        al::initNerve(this, &NrvCoin.WaitConnectMtx, 1);

    al::offCollide(this);
    mExternalForceKeeper = new ExternalForceKeeper();
    mRotateCalculator = new CoinRotateCalculator(this);

    mStartingQuat.set(al::getQuat(this));
    al::tryAddDisplayOffset(this, initInfo);
    al::tryGetDisplayOffset(&mDisplayOffset, initInfo);
    mMtxConnector = al::createMtxConnector(this);

    mShadowSize = rs::setShadowDropLength(this, initInfo, "本体");
    al::expandClippingRadiusByShadowLength(this, nullptr, mShadowSize);
    mWaterSurfaceShadow = rs::tryCreateWaterSurfaceCoinShadow(initInfo);

    if (mMtxConnector != nullptr)
        al::tryGetArg(&mIsConnectToCollisionBack, initInfo, "IsConnectToCollisionBack");

    if (al::trySyncStageSwitchAppear(this))
        al::setNerve(this, &NrvCoin.Appear);

    bool isCoinSave = false;
    al::tryGetArg(&isCoinSave, initInfo, "IsCoinSave");
    if (isCoinSave) {
        mSaveObjInfo = rs::createSaveObjInfoNoWriteSaveDataInSameWorldResetMiniGame(initInfo);
        if (rs::isOnSaveObjInfo(mSaveObjInfo)) {
            kill();
            return;
        }
    }

    mStateAppearRotate = new CoinStateAppearRotate(this, nullptr, mDisplayOffset, "誘導コイン出現");
    al::initNerveState(this, mStateAppearRotate, &NrvCoin.AppearCoinLead, "誘導コイン出現");
    mPoseTrans = al::getTrans(this);
    mPoseQuat = al::getQuat(this);
    al::registActorToDemoInfo(this, initInfo);
}

void Coin::initAfterPlacement() {
    if (mMtxConnector != nullptr) {
        if (!mIsConnectToCollisionBack) {
            al::attachMtxConnectorToCollision(mMtxConnector, this, 50.0f, 400.0f);
        } else {
            sead::Vector3f frontDir = sead::Vector3f(0.0f, 0.0f, 0.0f);
            al::calcFrontDir(&frontDir, this);
            al::attachMtxConnectorToCollision(
                mMtxConnector, this, al::getTrans(this) + frontDir * 50.0f, frontDir * -400.0f);
        }
    }

    if (al::isNerve(this, &NrvCoin.WaitPlayerDead)) {
        sead::Vector3f hitPos = sead::Vector3f::zero;
        al::Triangle triangle;

        if (alCollisionUtil::getFirstPolyOnArrow(
                this, &hitPos, &triangle, al::getTrans(this) + sead::Vector3f(0.0f, 20.0f, 0.0f),
                sead::Vector3f::ey * -200.0f, nullptr, nullptr)) {
            al::resetPosition(this, hitPos + 80.0f * sead::Vector3f::ey);
            return;
        }

        kill();
    }
}

void Coin::appear() {
    mExternalForceKeeper->reset();
    mRotateCalculator->reset();
    if (al::isNerve(this, &NrvCoin.Got) && mIsPlaced)
        return;
    al::LiveActor::appear();
    al::showModelIfHide(this);
}

void Coin::makeActorAlive() {
    al::LiveActor::makeActorAlive();
    al::showModelIfHide(this);
}

// https://decomp.me/scratch/5pT1P
// NON_MATCHING: Different stack pointer order
void Coin::control() {
    sead::Vector3f force = sead::Vector3f::zero;
    mExternalForceKeeper->calcForce(&force);
    mExternalForceKeeper->reset();
    bool checkWater;
    if (!al::isNerve(this, &NrvCoin.Wait) && !al::isNerve(this, &NrvCoin.WaitPlayerDead))
        checkWater = true;
    else
        checkWater = false;

    mRotateCalculator->update(force, checkWater);

    if (!rs::isActiveDemo(this) && mTimeLimit > 0) {
        mTimeLimit--;
        if (mTimeLimit <= 0) {
            kill();
            return;
        }
        if (!(mTimeLimit >= 201) && al::blinkModel(this, mTimeLimit, 6, 0))
            al::startSe(this, "PgBlink");
    }

    if (!al::isNerve(this, &NrvCoin.Got))
        rs::tryUpdateWaterSurfaceCoinShadow(mWaterSurfaceShadow, this, mShadowSize);
}

void Coin::endClipped() {
    mRotateCalculator->reset();
    al::LiveActor::endClipped();
}

bool Coin::receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self) {
    if (rs::isMsgPlayerDisregardTargetMarker(message) ||
        mExternalForceKeeper->receiveMsg(message, other, self))
        return true;

    if (al::isMsgRestart(message)) {
        if (al::isNerve(this, &NrvCoin.Got))
            return false;
        al::setNerve(this, &NrvCoin.AppearOnDemo);
        appear();
        return true;
    }

    if (rs::isMsgTimerAthleticDemoStart(message)) {
        rs::addDemoActor(this, false);
        return true;
    }

    if (rs::isMsgConductLightning(message) && isAnyWaitOrAppear() && isWait())
        return true;

    if (rs::isMsgItemGetAll(message) && isAnyWaitOrAppear()) {
        al::invalidateClipping(this);
        al::startHitReaction(this, "取得");
        al::setNerve(this, &NrvCoin.Got);
        return true;
    }

    if (rs::isMsgFishingItemGet(message) && isAnyWaitOrAppear()) {
        appearCountUp();
        return true;
    }

    if (rs::isMsgItemAmiiboKoopa(message) && isAnyWaitOrAppear()) {
        sead::Vector3f direction = sead::Vector3f::zero;
        al::calcDirBetweenSensorsH(&direction, other, self);
        al::setVelocity(this, direction * 20.0f + 40.0f * sead::Vector3f::ey);
        al::setNerve(this, &NrvCoin.PopUpNormal);
        return true;
    }

    return false;
}

void Coin::tryCreateMtxConnector() {
    if (mMtxConnector == nullptr)
        mMtxConnector = al::createMtxConnector(this);
}

void Coin::appearCirclePlacement() {
    appear();
    al::setNerve(this, &NrvCoin.WaitCircle);
}

void Coin::appearPopUpCommon(bool startHitReaction) {
    al::validateHitSensors(this);
    al::setNerve(this, &NrvCoin.PopUpNormal);
    appear();
    al::invalidateClipping(this);
    if (startHitReaction)
        al::startHitReaction(this, "飛出し出現");

    sead::Vector3f frontDir = sead::Vector3f(0.0f, 0.0f, 0.0f);
    sead::Vector3f upDir = sead::Vector3f(0.0f, 0.0f, 0.0f);
    al::calcFrontDir(&frontDir, this);
    al::calcUpDir(&upDir, this);
    al::setVelocity(this, frontDir * 7.5f + upDir * 20.0f);
}

void Coin::appearPopUp() {
    if (!al::isNerve(this, &NrvCoin.Got) || !mIsPlaced) {
        appearPopUpCommon(true);
        mTimeLimit = 600;
    }
}

void Coin::appearPopUpWithoutHitReaction() {
    if (!al::isNerve(this, &NrvCoin.Got) || !mIsPlaced) {
        appearPopUpCommon(false);
        mTimeLimit = 600;
    }
}

void Coin::appearPopUpVelocity() {
    appearPopUp();

    sead::Vector3f frontDir = sead::Vector3f(0.0f, 0.0f, 0.0f);
    sead::Vector3f upDir = sead::Vector3f(0.0f, 0.0f, 0.0f);
    al::calcFrontDir(&frontDir, this);
    al::calcUpDir(&upDir, this);
    f32 frontOffset = al::getRandom(10.0f, 15.0f);
    f32 upOffset = al::getRandom(20.0f, 30.0f);
    al::setVelocity(this, frontOffset * frontDir + upOffset * upDir);
}

void Coin::appearAbove() {
    appear();
    al::invalidateClipping(this);
    al::setVelocity(this, sAppearAboveVelocity);
    al::setNerve(this, &NrvCoin.PopUpNormal);
    al::startHitReaction(this, "真上に出現");
    mTimeLimit = -1;
}

inline void appearCountUpReset(al::LiveActor* actor, ExternalForceKeeper* forceKeeper) {
    al::offCollide(actor);
    al::invalidateClipping(actor);
    al::invalidateHitSensors(actor);
    sead::Vector3f direction = sead::Vector3f(0.0f, 0.0f, 0.0f);
    al::calcQuatUp(&direction, actor);
    al::setVelocityToDirection(actor, direction, 16.0f);
    forceKeeper->reset();
    actor->appear();
}

void Coin::appearCountUpCommon(sead::Vector3f* velocity, s32 delay) {
    al::setVelocityZero(this);
    sead::Vector3f quatUp = sead::Vector3f::zero;
    al::calcQuatUp(&quatUp, this);
    velocity->set(quatUp * 16.0f);

    if (delay != 0) {
        al::setNerve(this, &NrvCoin.CountUpDelay);
        al::hideModelIfShow(this);
        return;
    }

    al::setNerve(this, &NrvCoin.CountUp);
    al::setVelocity(this, *velocity);
}

void Coin::appearCountUp() {
    al::setNerve(this, &NrvCoin.CountUp);
    al::setQuat(this, sead::Quatf::unit);
    appearCountUpReset(this, mExternalForceKeeper);
    mTimeLimit = -1;
}

void Coin::appearCountUpFiveCount() {
    al::setNerve(this, &NrvCoin.CountUpFive);
    al::setQuat(this, sead::Quatf::unit);
    appearCountUpReset(this, mExternalForceKeeper);
    mTimeLimit = -1;
}

void Coin::appearCountUp3(s32 coinIndex) {
    sead::Vector3f direction = sead::Vector3f::zero;

    switch (coinIndex) {
    case 0:
        mCountUpDelay = 0;
        direction = sead::Vector3f(0.0f, 0.0f, 50.0f);
        break;
    case 1:
        mCountUpDelay = 5;
        direction = sead::Vector3f(40.0f, 0.0f, -30.0f);
        break;
    case 2:
        mCountUpDelay = 10;
        direction = sead::Vector3f(-40.0f, 0.0f, -30.0f);
        break;
    default:
        break;
    }

    appearCountUpReset(this, mExternalForceKeeper);
    al::rotateVectorQuat(&direction, al::getQuat(this));
    al::getTransPtr(this)->add(direction);
    mTimeLimit = -1;
    appearCountUpCommon(&mBlowVelocity, mCountUpDelay);
}

void Coin::appearCountUp5(s32 coinIndex) {
    sead::Vector3f direction = sead::Vector3f::zero;

    switch (coinIndex) {
    case 0:
        mCountUpDelay = 0;
        direction = sead::Vector3f(0.0f, 0.0f, 220.0f);
        break;
    case 1:
        mCountUpDelay = 5;
        direction = sead::Vector3f(-120.0f, 0.0f, -200.0f);
        break;
    case 2:
        mCountUpDelay = 10;
        direction = sead::Vector3f(240.0f, 0.0f, 50.0f);
        break;
    case 3:
        mCountUpDelay = 15;
        direction = sead::Vector3f(-240.0f, 0.0f, 50.0f);
        break;
    case 4:
        mCountUpDelay = 20;
        direction = sead::Vector3f(120.0f, 0.0f, -200.0f);
        break;
    default:
        break;
    }

    appearCountUpReset(this, mExternalForceKeeper);
    al::rotateVectorQuat(&direction, al::getQuat(this));
    al::setTrans(this, al::getTrans(this) + direction);
    mTimeLimit = -1;
    appearCountUpCommon(&mBlowVelocity, mCountUpDelay);
}

void Coin::appearCountUp10(s32 coinIndex) {
    sead::Vector3f direction = sead::Vector3f(200.0f, 200.0f, 200.0f);
    al::rotateVectorDegreeY(&direction, coinIndex * 36.0f);

    // Weird optimization here. It's supposed to be a switch case.
    if ((coinIndex == 5) || (coinIndex == 0))
        mCountUpDelay = coinIndex == 0 ? 0 : 20;

    else if ((coinIndex == 6) || (coinIndex == 1))
        mCountUpDelay = coinIndex == 1 ? 5 : 25;

    else if ((coinIndex == 7) || (coinIndex == 2))
        mCountUpDelay = coinIndex == 2 ? 10 : 30;

    else if ((coinIndex == 8) || (coinIndex == 3))
        mCountUpDelay = coinIndex == 3 ? 15 : 35;

    else if ((coinIndex == 9) || (coinIndex == 4))
        mCountUpDelay = coinIndex == 4 ? 20 : 40;

    appearCountUpReset(this, mExternalForceKeeper);
    al::rotateVectorQuat(&direction, al::getQuat(this));
    al::getTransPtr(this)->add(direction);
    mTimeLimit = -1;
    appearCountUpCommon(&mBlowVelocity, mCountUpDelay);
}

void Coin::appearCountUpFixPos10(s32 coinIndex) {
    al::setNerve(this, &NrvCoin.CountUp);
    appearCountUpReset(this, mExternalForceKeeper);
    mTimeLimit = -1;
    mCountUpDelay = coinIndex * 8;
    appearCountUpCommon(&mBlowVelocity, mCountUpDelay);
}

void Coin::appearCoinRail() {
    al::setNerve(this, &NrvCoin.WaitCoinRail);
    appear();
}

void Coin::appearCoinChameleon(const sead::Vector3f& trans, const sead::Quatf& quat,
                               const sead::Vector3f& offset) {
    al::setTrans(this, trans);
    al::setQuat(this, quat);
    mChameleonOffset = offset;
    appear();
    al::setNerve(this, &NrvCoin.Wait);
}

void Coin::appearLimitTime(s32 timeLimit) {
    al::setNerve(this, &NrvCoin.AppearCoinLead);
    al::invalidateClipping(this);
    appear();
    mTimeLimit = timeLimit;
}

void Coin::appearBlow(const sead::Vector3f& velocity, s32 timeLimit) {
    appear();
    al::invalidateClipping(this);
    al::startHitReaction(this, "飛出し出現");
    al::setVelocity(this, velocity);
    mTimeLimit = timeLimit;
    al::validateHitSensors(this);
    al::showModelIfHide(this);

    if (mMtxConnector != nullptr)
        al::disconnectMtxConnector(mMtxConnector);

    rotate();
    al::setNerve(this, &NrvCoin.PopUpNormal);
}

void Coin::rotate() {
    if (al::isNerve(this, &NrvCoin.CountUp) || al::isNerve(this, &NrvCoin.CountUpFive)) {
        al::rotateQuatYDirDegree(this, 9.5f);
        return;
    }

    if (mMtxConnector != nullptr && al::isMtxConnectorConnecting(mMtxConnector) &&
        (al::isNerve(this, &NrvCoin.Appear) || al::isNerve(this, &NrvCoin.WaitConnectMtx))) {
        al::connectPoseQT(this, mMtxConnector, mPoseQuat, mPoseTrans);
        al::getTransPtr(this)->add(mChameleonOffset);
    } else {
        al::setQuat(this, mStartingQuat);
    }

    al::rotateQuatYDirDegree(this, mRotateCalculator->getRotate());
}

void Coin::appearBlowUpCommon(s32 delayTime, f32 horizontalForce, f32 verticalForce, s32 coinCount,
                              s32 coinIndex) {
    appear();
    al::invalidateClipping(this);
    mCountUpDelay = delayTime;
    sead::Vector3f horizontalDirection = horizontalForce * sead::Vector3f::ez;
    al::rotateVectorDegreeY(&horizontalDirection, (360.0 / coinCount) * coinIndex);
    mBlowVelocity = verticalForce * sead::Vector3f::ey + horizontalDirection;
    al::setVelocityZero(this);
    if (mCountUpDelay == 0) {
        appearBlow(mBlowVelocity, 480);
    } else {
        al::hideModelIfShow(this);
        al::setNerve(this, &BlowUpDelay);
    }
}

void Coin::appearBlowUp(s32 coinIndex, s32 coinCount) {
    appearBlowUpCommon(coinIndex * 5, 6.0f, 40.0f, coinCount, coinIndex);
}

void Coin::appearBlowUpLittle(s32 coinIndex, s32 coinCount) {
    appearBlowUpCommon(coinIndex * 5, 3.0f, 35.0f, coinCount, coinIndex);
}

void Coin::appearFall(const sead::Vector3f& velocity, s32 timeLimit) {
    appear();
    al::invalidateClipping(this);
    al::setVelocity(this, velocity);
    mTimeLimit = timeLimit;
    al::setNerve(this, &NrvCoin.PopUpNormal);
}

void Coin::appearPlayerDead(const sead::Vector3f& position, const sead::Vector3f& direction,
                            bool isInWater, bool isTimeBalloon) {
    al::resetPosition(this, position);

    if (isInWater)
        al::setVelocity(this, 30.0f * sead::Vector3f::ey + direction * 15.0f);
    else if (rs::isInLowGravityArea(this))
        al::setVelocity(this, 30.0f * sead::Vector3f::ey + direction * 5.0f);
    else
        al::setVelocity(this, 40.0f * sead::Vector3f::ey + direction * 5.0f);

    al::onCollide(this);
    appear();
    al::invalidateClipping(this);
    al::startHitReaction(this, "飛出し出現[死亡]");
    mTimeLimit = -1;

    if (isTimeBalloon)
        al::setNerve(this, &NrvCoin.PopUpPlayerDeadTimeBalloon);
    else
        al::setNerve(this, &NrvCoin.PopUpPlayerDead);
}

void Coin::appearPlayerDeadReplace(const sead::Vector3f& position) {
    al::setNerve(this, &NrvCoin.WaitPlayerDead);
    al::resetPosition(this, position);
    al::LiveActor::appear();
}

void Coin::setShadowDropLength(f32 shadowLength) {
    al::setShadowMaskDropLength(this, shadowLength, "本体");
    al::expandClippingRadiusByShadowLength(this, nullptr, shadowLength);
}

void Coin::get() {
    al::startHitReaction(this, "取得");
    al::setNerve(this, &NrvCoin.Got);
}

bool Coin::isGot() const {
    return al::isNerve(this, &NrvCoin.Got);
}

bool Coin::isGotOrRotate() const {
    if (al::isNerve(this, &NrvCoin.Got))
        return true;
    if (al::isNerve(this, &NrvCoin.AppearCoinLead))
        return true;
    return false;
}

bool Coin::isWait() const {
    if (al::isNerve(this, &NrvCoin.Wait))
        return true;
    if (al::isNerve(this, &NrvCoin.WaitCircle))
        return true;
    return false;
}

bool Coin::isAnyWaitOrAppear() const {
    if (al::isNerve(this, &NrvCoin.Wait))
        return true;
    if (al::isNerve(this, &NrvCoin.WaitCircle))
        return true;
    if (al::isNerve(this, &NrvCoin.WaitCoinRail))
        return true;
    if (al::isNerve(this, &NrvCoin.WaitConnectMtx))
        return true;
    if (al::isNerve(this, &NrvCoin.WaitOnDemo))
        return true;
    if (al::isNerve(this, &NrvCoin.WaitOnDemoEnd))
        return true;
    if (al::isNerve(this, &NrvCoin.WaitPlayerDead))
        return true;
    if (al::isNerve(this, &NrvCoin.PopUpNormal) && al::isGreaterEqualStep(this, 60))
        return true;
    if (al::isNerve(this, &NrvCoin.AppearCoinLead))
        return true;
    if (al::isNerve(this, &NrvCoin.Appear))
        return true;
    return false;
}

void Coin::exeAppear() {
    f32 rotateOffset = al::calcNerveEaseOutValue(this, 54, -720.0f, 0.0f);
    mRotateCalculator->setRotateOffset(rotateOffset);
    rotate();

    if (al::isNerve(this, &NrvCoin.Appear)) {
        if (al::isFirstStep(this))
            al::startHitReaction(this, "通常出現");
        al::setNerveAtGreaterEqualStep(this, &NrvCoin.Wait, 54);
        return;
    }

    if (al::isNerve(this, &NrvCoin.AppearOnDemo))
        al::setNerveAtGreaterEqualStep(this, &NrvCoin.WaitOnDemo, 54);
}

void Coin::exeAppearCoinLead() {
    if (al::updateNerveState(this))
        al::setNerve(this, &NrvCoin.Wait);
}

void Coin::exePopUp() {
    if (al::isFirstStep(this)) {
        al::invalidateClipping(this);
        al::onCollide(this);
        al::validateHitSensors(this);
    }

    rotate();

    if (al::isInWater(this) || al::isInWaterArea(this)) {
        al::addVelocityToGravity(this, 0.6f);
        al::scaleVelocity(this, 0.95f);
    } else {
        if (rs::isInLowGravityArea(this))
            al::addVelocityToGravity(this, 1.0f);
        else
            al::addVelocityToGravity(this, 1.5f);
        al::scaleVelocity(this, 0.99f);
    }

    if ((al::isCollidedFloorCode(this, "Poison") || al::isCollidedFloorCode(this, "DamageFire") ||
         al::isInDeathArea(this)) &&
        al::getVelocity(this).y < 0.0f) {
        al::startHitReaction(this, "消滅");
        kill();
        return;
    }

    if ((al::isNerve(this, &NrvCoin.PopUpPlayerDead) ||
         al::isNerve(this, &NrvCoin.PopUpPlayerDeadTimeBalloon)) &&
        al::isGreaterEqualStep(this, 5)) {
        sead::Vector3f surfacePos = sead::Vector3f::zero;
        sead::Vector3f surfaceNormal = sead::Vector3f::zero;
        if (al::calcFindFireSurface(&surfacePos, &surfaceNormal, this,
                                    al::getTrans(this) + 50.0f * sead::Vector3f::ey,
                                    sead::Vector3f::ey, 50.0f)) {
            al::startHitReaction(this, "消滅");
            kill();
            return;
        }
    }

    if (al::isNerve(this, &NrvCoin.PopUpPlayerDeadTimeBalloon) &&
        al::isGreaterEqualStep(this, 60)) {
        al::startHitReaction(this, "消滅");
        kill();
        return;
    }

    if (al::getVelocity(this).y < 0.0f && al::isCollidedGround(this)) {
        if (al::calcSpeed(this) < 15.0f) {
            if (mMtxConnector != nullptr) {
                al::disconnectMtxConnector(mMtxConnector);
                mPoseTrans = al::getTrans(this);
                al::attachMtxConnectorToCollision(mMtxConnector, this, false);
            }
            al::offCollide(this);
            al::setVelocityZero(this);
            al::setNerve(this, &NrvCoin.WaitConnectMtx);
            return;
        }
        al::getVelocityPtr(this)->y *= -0.75f;
        al::startSeWithParam(this, "PgBound", al::getVelocityPtr(this)->y, "");
    }
}

void Coin::exeCountUpDelay() {
    if (al::isGreaterEqualStep(this, mCountUpDelay)) {
        al::setVelocity(this, mBlowVelocity);
        al::setNerve(this, &NrvCoin.CountUp);
    }
}

void Coin::exeCountUp() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Got");
        if (al::isNerve(this, &NrvCoin.CountUpFive))
            al::startHitReaction(this, "自動取得5枚[開始]");
        else
            al::startHitReaction(this, "自動取得[開始]");

        if (al::isNerve(this, &NrvCoin.CountUpFive))
            for (u32 i = 0; i < 5; i++)
                GameDataFunction::addCoin(this, 1);
        else
            GameDataFunction::addCoin(this, 1);
        al::showModelIfHide(this);
    }

    rotate();

    sead::Vector3f direction = sead::Vector3f(0.0f, 0.0f, 0.0f);
    al::calcQuatGravity(&direction, al::getQuat(this));
    al::addVelocityToDirection(this, direction, 0.6f);
    al::scaleVelocity(this, 0.99f);
    al::getVelocity(this);
    al::getQuat(this);

    if (al::isGreaterEqualStep(this, 32)) {
        al::startHitReaction(this, "自動取得[終了]");
        al::setNerve(this, &NrvCoin.GotNoCoin);
    }
}

void Coin::exeWait() {
    if (al::isFirstStep(this)) {
        if (mTimeLimit == -1 && !al::isNerve(this, &NrvCoin.WaitCoinRail) &&
            !al::isNerve(this, &NrvCoin.WaitCircle))
            al::validateClipping(this);
        al::startAction(this, "Wait");
    }

    if (al::isNerve(this, &NrvCoin.WaitOnDemo)) {
        mRotateCalculator->increaseObjCountOffset();
        rotate();
        if (!rs::isActiveDemo(this))
            al::setNerve(this, &NrvCoin.WaitOnDemoEnd);
        return;
    }

    rotate();
}

void Coin::exeWaitOnDemoEnd() {
    rotate();
}

void Coin::exeGot() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Got");
        if (!al::isNerve(this, &NrvCoin.GotNoCoin))
            GameDataFunction::addCoin(this, 1);

        al::onStageSwitch(this, "SwitchGetOn");
        alPadRumbleFunction::startPadRumble(this, "コッ（最小）", 1000.0f, 5000.0f);
        if (mWaterSurfaceShadow != nullptr)
            mWaterSurfaceShadow->disappearShadow();
    }

    if (al::isActionEnd(this)) {
        if (mSaveObjInfo != nullptr)
            rs::onSaveObjInfo(mSaveObjInfo);
        kill();
    }
}

void Coin::exeBlowUpDelay() {
    if (al::isGreaterEqualStep(this, mCountUpDelay)) {
        al::showModelIfHide(this);
        appearBlow(mBlowVelocity, 480);
    }
}

const sead::Vector3f& CoinFunction::getAppearAboveVelocity() {
    return sAppearAboveVelocity;
}

void CoinFunction::appearCoinBlowVeryLittle(Coin* coin, const sead::Vector3f& direction) {
    bool isInWater = al::isInWater(coin) || al::isInWaterArea(coin);

    sead::Vector3f velocity = (isInWater ? 12.0f : 3.5f) * direction;
    sead::Vector3f verticalVelocity = (isInWater ? 35.0f : 30.0f) * sead::Vector3f::ey;
    velocity += verticalVelocity;

    coin->appearBlow(velocity, 600);
}

void CoinFunction::appearCoinBlowLittle(Coin* coin, const sead::Vector3f& direction) {
    bool isInWater = al::isInWater(coin) || al::isInWaterArea(coin);

    sead::Vector3f velocity = (isInWater ? 15.0f : 4.5f) * direction;
    sead::Vector3f verticalVelocity = (isInWater ? 40.0f : 30.0f) * sead::Vector3f::ey;
    velocity += verticalVelocity;

    coin->appearBlow(velocity, 600);
}

void CoinFunction::appearCoinBlowMiddle(Coin* coin, const sead::Vector3f& direction) {
    bool isInWater = al::isInWater(coin) || al::isInWaterArea(coin);

    sead::Vector3f velocity = (isInWater ? 18.0f : 6.0f) * direction;
    sead::Vector3f verticalVelocity = (isInWater ? 45.0f : 35.0f) * sead::Vector3f::ey;
    velocity += verticalVelocity;

    coin->appearBlow(velocity, 600);
}

void CoinFunction::appearCoinBlowLarge(Coin* coin, const sead::Vector3f& direction) {
    bool isInWater = al::isInWater(coin) || al::isInWaterArea(coin);

    sead::Vector3f velocity = (isInWater ? 23.0f : 7.5f) * direction;
    sead::Vector3f verticalVelocity = (isInWater ? 50.0f : 40.0f) * sead::Vector3f::ey;
    velocity += verticalVelocity;

    coin->appearBlow(velocity, 600);
}

void CoinFunction::appearCoinBlowLargeEnemy(Coin* coin, const sead::Vector3f& direction) {
    sead::Vector3f velocity = 3.5f * direction;
    sead::Vector3f verticalVelocity = 30.0f * sead::Vector3f::ey;
    velocity += verticalVelocity;

    coin->appearBlow(velocity, 1200);
}

void CoinFunction::appearCoinBlowBoss(Coin* coin, const sead::Vector3f& direction) {
    sead::Vector3f velocity = 8.0f * direction;
    sead::Vector3f verticalVelocity = 40.0f * sead::Vector3f::ey;
    velocity += verticalVelocity;

    coin->appearBlow(velocity, 1200);
}

void CoinFunction::appearCoinBlowGk(Coin* coin, const sead::Vector3f& direction) {
    sead::Vector3f velocity = 7.0f * direction;
    sead::Vector3f verticalVelocity = 40.0f * sead::Vector3f::ey;
    velocity += verticalVelocity;

    coin->appearBlow(velocity, 1200);
}
