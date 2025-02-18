#include "Item/CoinCollectEmpty.h"

#include <math/seadVector.h>

#include "Library/Collision/PartsConnector.h"
#include "Library/Controller/PadRumbleFunction.h"
#include "Library/Effect/EffectSystemInfo.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActorUtil.h"
#include "Library/Nature/NatureUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Item/CoinRotateCalculator.h"
#include "Item/CoinStateCountUp.h"
#include "System/GameDataFunction.h"
#include "Util/ExternalForceKeeper.h"
#include "Util/SensorMsgFunction.h"
#include "Util/ShadowUtil.h"

namespace {
NERVE_IMPL(CoinCollectEmpty, Wait);
NERVE_IMPL(CoinCollectEmpty, Got);
NERVE_IMPL(CoinCollectEmpty, CountUp);

NERVES_MAKE_STRUCT(CoinCollectEmpty, Wait, Got, CountUp);
}  // namespace

CoinCollectEmpty::CoinCollectEmpty(const char* name, const char* archiveName)
    : al::LiveActor(name), mArchiveName(archiveName) {}

void CoinCollectEmpty::init(const al::ActorInitInfo& initInfo) {
    al::initActorSceneInfo(this, initInfo);
    al::initActorWithArchiveName(this, initInfo, mArchiveName, nullptr);
    al::initNerve(this, &NrvCoinCollectEmpty.Wait, 1);
    mStateCountUp = new CoinStateCountUp(this);
    al::initNerveState(this, mStateCountUp, &NrvCoinCollectEmpty.Got, "カウントアップ状態");
    mRotateCalculator = new CoinRotateCalculator(this);
    al::tryAddDisplayOffset(this, initInfo);
    mMtxConnector = al::tryCreateMtxConnector(this, initInfo);
    mExternalForceKeeper = new ExternalForceKeeper();
    al::expandClippingRadiusByShadowLength(this, nullptr,
                                           rs::setShadowDropLength(this, initInfo, "本体"));

    if (al::isExistDitherAnimator(this))
        al::stopDitherAnimAutoCtrl(this);

    al::setModelAlphaMask(this, 0.499f);
    al::setEffectNamedMtxPtr(this, "WaterSurface", &mWaterSurfaceMatrix);
    makeActorAlive();
}

void CoinCollectEmpty::initAfterPlacement() {
    if (mMtxConnector != nullptr)
        al::attachMtxConnectorToCollision(mMtxConnector, this, false);
}

bool CoinCollectEmpty::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                  al::HitSensor* self) {
    if (rs::isMsgFishingItemGet(message) && al::isNerve(this, &NrvCoinCollectEmpty.Wait)) {
        al::invalidateClipping(this);
        al::setNerve(this, &NrvCoinCollectEmpty.Got);
        return true;
    }

    if ((rs::isMsgCapAttack(message) || rs::isMsgItemGetAll(message)) &&
        al::isNerve(this, &NrvCoinCollectEmpty.Wait)) {
        al::invalidateClipping(this);
        al::setNerve(this, &NrvCoinCollectEmpty.CountUp);
        return false;
    }

    if (rs::isMsgFishingLineTouch(message)) {
        mRotateCalculator->addFishingLineTouch();
        return true;
    }

    return false;
}

void CoinCollectEmpty::endClipped() {
    mRotateCalculator->reset();
    al::LiveActor::endClipped();
}

void CoinCollectEmpty::appear() {
    mExternalForceKeeper->reset();
    al::LiveActor::appear();
}

void CoinCollectEmpty::control() {
    sead::Vector3f force = sead::Vector3f::zero;
    mExternalForceKeeper->calcForce(&force);
    mExternalForceKeeper->reset();
    mRotateCalculator->update(force, true);
}

void CoinCollectEmpty::rotate() {
    if (mMtxConnector != nullptr)
        al::connectPoseQT(this, mMtxConnector);

    al::setQuat(this, sead::Quatf::unit);
    al::rotateQuatYDirDegree(this, mRotateCalculator->getRotate());
}

void CoinCollectEmpty::exeWait() {
    rotate();
}

void CoinCollectEmpty::exeGot() {
    if (al::isFirstStep(this)) {
        alPadRumbleFunction::startPadRumble(this, "コッ（微弱）", 500.0f, 2000.0f, -1);
        al::startAction(this, "Got");
        GameDataFunction::addCoin(this, 1);
        GameDataFunction::addCoin(this, 1);
        al::hideModelIfShow(this);
    }
    if (al::isGreaterEqualStep(this, 30))
        kill();
}

void CoinCollectEmpty::exeCountUp() {
    if (al::isFirstStep(this)) {
        GameDataFunction::addCoin(this, 1);
        GameDataFunction::addCoin(this, 1);
    }

    if (!mIsNotInWater && !al::isInWater(this)) {
        sead::Vector3f waterSurface = sead::Vector3f::zero;
        al::calcFindWaterSurface(&waterSurface, nullptr, this, al::getTrans(this),
                                 sead::Vector3f::ey, 200.0f);
        mWaterSurfaceMatrix.setBase(3, waterSurface);
        al::startHitReaction(this, "水しぶき");
        mIsNotInWater = true;
    }

    if (al::updateNerveState(this)) {
        al::startHitReaction(this, "釣り取得");
        kill();
    }
}
