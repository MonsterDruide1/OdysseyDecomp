#include "Item/CoinCollect.h"

#include <math/seadQuat.h>

#include "Library/Collision/PartsConnector.h"
#include "Library/Controller/PadRumbleFunction.h"
#include "Library/Effect/EffectSystemInfo.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/Nature/NatureUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Scene/SceneObjUtil.h"
#include "Library/Se/SeFunction.h"
#include "Library/Stage/StageSwitchKeeper.h"
#include "Library/Thread/FunctorV0M.h"

#include "Item/CoinCollectEmpty.h"
#include "Item/CoinCollectHintState.h"
#include "Item/CoinCollectHolder.h"
#include "Item/CoinCollectWatcher.h"
#include "Item/CoinRotateCalculator.h"
#include "Item/CoinStateCountUp.h"
#include "MapObj/CapMessageShowInfo.h"
#include "Scene/SceneObjFactory.h"
#include "System/GameDataFunction.h"
#include "Util/ExternalForceKeeper.h"
#include "Util/ItemUtil.h"
#include "Util/PlayerUtil.h"
#include "Util/SensorMsgFunction.h"
#include "Util/ShadowUtil.h"
#include "Util/WaterSurfaceShadow.h"

namespace {
NERVE_IMPL(CoinCollect, Wait);
NERVE_IMPL(CoinCollect, WaitAmiibo);
NERVE_IMPL(CoinCollect, Got);
NERVE_IMPL(CoinCollect, CountUp);
NERVE_IMPL(CoinCollect, Blow);

NERVES_MAKE_STRUCT(CoinCollect, Wait, CountUp, WaitAmiibo, Got, Blow);
}  // namespace

CoinCollect::CoinCollect(const char* name) : al::LiveActor(name) {}

void CoinCollect::init(const al::ActorInitInfo& initInfo) {
    using CoinCollectFunctor = al::FunctorV0M<CoinCollect*, void (CoinCollect::*)()>;

    al::initActorSceneInfo(this, initInfo);
    rs::createCoinCollectWatcher(this);
    rs::createCoinCollectHolder(this);

    al::getSceneObj<CoinCollectHolder>(this, SceneObjID_CoinCollectHolder)
        ->registerCoinCollect(this);

    if (GameDataFunction::isGotCoinCollect(this, initInfo)) {
        const char* archiveName = rs::getStageCoinCollectEmptyArchiveName(this);
        al::getSceneObj<CoinCollectWatcher>(this, SceneObjID_CoinCollectWatcher)
            ->registerCoin(true);
        CoinCollectEmpty* coinCollectEmpty = new CoinCollectEmpty("コレクトコイン空", archiveName);
        al::initCreateActorWithPlacementInfo(coinCollectEmpty, initInfo);
        makeActorDead();
        mCoinCollectEmpty = coinCollectEmpty;
        _198 = false;
        return;
    }

    al::getSceneObj<CoinCollectWatcher>(this, SceneObjID_CoinCollectWatcher)->registerCoin(false);
    const char* archiveName = rs::getStageCoinCollectArchiveName(this);
    al::initActorWithArchiveName(this, initInfo, archiveName, nullptr);
    al::initNerve(this, &NrvCoinCollect.Wait, 2);

    mStateCountUp = new CoinStateCountUp(this);
    al::initNerveState(this, mStateCountUp, &NrvCoinCollect.CountUp, "カウントアップ状態");

    mHintState = new CoinCollectHintState(this);
    al::initNerveState(this, mHintState, &NrvCoinCollect.WaitAmiibo, "ヒント");

    al::tryAddDisplayOffset(this, initInfo);
    mMtxConnector = al::tryCreateMtxConnector(this, initInfo);

    mQuat.set(al::getQuat(this));

    mPlacementId = al::createPlacementId(initInfo);
    mExternalForceKeeper = new ExternalForceKeeper();

    mRotateCalculator = new CoinRotateCalculator(this);

    al::listenStageSwitchOnKill(this, CoinCollectFunctor(this, &CoinCollect::listenKill));
    if (al::listenStageSwitchOnAppear(this, CoinCollectFunctor(this, &CoinCollect::listenAppear)))
        makeActorDead();
    else
        makeActorAlive();

    al::offCollide(this);
    mShadowLength = rs::setShadowDropLength(this, initInfo, "本体");
    al::expandClippingRadiusByShadowLength(this, nullptr, mShadowLength);
    mWaterSurfaceShadow = rs::tryCreateWaterSurfaceCoinShadow(initInfo);
    al::setEffectNamedMtxPtr(this, "WaterSurface", &mSurfaceMatrix);
}

void CoinCollect::initAfterPlacement() {
    if (mMtxConnector != nullptr)
        al::attachMtxConnectorToCollision(mMtxConnector, this, false);
}

void CoinCollect::control() {
    sead::Vector3f force = sead::Vector3f::zero;
    mExternalForceKeeper->calcForce(&force);
    mExternalForceKeeper->reset();

    bool isNotWait = !al::isNerve(this, &NrvCoinCollect.Wait);
    mRotateCalculator->update(force, isNotWait);

    if (!al::isNerve(this, &NrvCoinCollect.Got) && !al::isNerve(this, &NrvCoinCollect.Wait))
        rs::tryUpdateWaterSurfaceCoinShadow(mWaterSurfaceShadow, this, mShadowLength);
}

bool CoinCollect::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                             al::HitSensor* self) {
    if (mExternalForceKeeper->receiveMsg(message, other, self))
        return true;

    if (rs::isMsgItemAmiiboKoopa(message)) {
        sead::Vector3f direction = sead::Vector3f::zero;
        al::calcDirBetweenSensorsH(&direction, other, self);
        sead::Vector3f velocity = 20.0f * direction;
        sead::Vector3f verticalVelocity = 40.0f * sead::Vector3f::ey;
        verticalVelocity += velocity;
        al::setVelocity(this, verticalVelocity);
        al::setNerve(this, &NrvCoinCollect.Blow);
        return true;
    }
    if (!al::isNerve(this, &NrvCoinCollect.Wait) && !al::isNerve(this, &NrvCoinCollect.WaitAmiibo))
        return false;

    if (rs::isMsgCapAttack(message) && !al::isNerve(this, &NrvCoinCollect.Got) &&
        !al::isNerve(this, &NrvCoinCollect.CountUp)) {
        al::invalidateClipping(this);
        al::setNerve(this, &NrvCoinCollect.Got);
        return false;
    }
    if (rs::isMsgFishingItemGet(message) && !al::isNerve(this, &NrvCoinCollect.Got) &&
        !al::isNerve(this, &NrvCoinCollect.CountUp)) {
        al::invalidateClipping(this);
        al::setNerve(this, &NrvCoinCollect.CountUp);
        return true;
    }
    if (rs::isMsgFishingLineTouch(message)) {
        mRotateCalculator->addFishingLineTouch();
        return true;
    }
    if (rs::isMsgItemGetAll(message) && !al::isNerve(this, &NrvCoinCollect.Got) &&
        !al::isNerve(this, &NrvCoinCollect.CountUp)) {
        al::invalidateClipping(this);
        al::setNerve(this, &NrvCoinCollect.Got);
        return true;
    }
    return false;
}

void CoinCollect::endClipped() {
    mRotateCalculator->reset();
    al::LiveActor::endClipped();
}

void CoinCollect::appear() {
    mRotateCalculator->reset();
    al::LiveActor::appear();
}

void CoinCollect::makeActorAlive() {
    if (mCoinCollectEmpty == nullptr && !al::isNerve(this, &NrvCoinCollect.Got))
        al::LiveActor::makeActorAlive();
}

void CoinCollect::listenKill() {
    kill();
}

void CoinCollect::listenAppear() {
    appear();
}

const sead::Vector3f& CoinCollect::getTransForHint() const {
    if (mCoinCollectEmpty != nullptr)
        return al::getTrans(mCoinCollectEmpty);

    return al::getTrans(this);
}

void CoinCollect::appearHelpAmiiboEffect() {
    if (al::isDead(this))
        makeActorAlive();

    al::invalidateClipping(this);
    al::startHitReaction(this, "発光");
    al::setNerve(this, &NrvCoinCollect.WaitAmiibo);
}

void CoinCollect::deleteHelpAmiiboEffect() {
    mHintState->deleteHintEffect();
}

void CoinCollect::reappearHelpAmiiboEffect() {
    mHintState->appearHintEffect();
}

bool CoinCollect::isEnableHint() const {
    if (al::isDead(this))
        return true;

    return al::isNerve(this, &NrvCoinCollect.Wait);
}

void CoinCollect::rotate() {
    if (mMtxConnector != nullptr)
        al::connectPoseQT(this, mMtxConnector);

    al::setQuat(this, sead::Quatf::unit);
    al::rotateQuatYDirDegree(this, mRotateCalculator->getRotate());
}

void CoinCollect::exeWait() {
    if (al::isFirstStep(this)) {
        al::validateClipping(this);
        rs::tryUpdateWaterSurfaceCoinShadow(mWaterSurfaceShadow, this, mShadowLength);
    }

    if (mMtxConnector != nullptr)
        al::connectPoseQT(this, mMtxConnector);

    rotate();

    if (rs::isNearPlayerH(this, 2000.0f))
        al::holdSe(this, "Shining");
}

void CoinCollect::exeWaitAmiibo() {
    if (mMtxConnector != nullptr)
        al::connectPoseQT(this, mMtxConnector);

    rotate();

    if (rs::isNearPlayerH(this, 2000.0f))
        al::holdSe(this, "Shining");

    if (al::updateNerveState(this))
        al::setNerve(this, &NrvCoinCollect.Wait);
}

void CoinCollect::exeGot() {
    if (al::isFirstStep(this)) {
        rs::tryShowCapMsgCollectCoinGetFirst(this);
        al::startAction(this, "Got");
        alPadRumbleFunction::startPadRumble(this, "コッ（微弱）", 1000.0f, 5000.0f, -1);
        if (mWaterSurfaceShadow != nullptr)
            mWaterSurfaceShadow->disappearShadow();
        _198 = false;
    }

    if (mMtxConnector != nullptr)
        al::connectPoseQT(this, mMtxConnector);

    if (al::isActionEnd(this)) {
        CoinCollectWatcher* watcher =
            al::getSceneObj<CoinCollectWatcher>(this, SceneObjID_CoinCollectWatcher);
        watcher->countup(this);
        GameDataFunction::addCoinCollect(this, mPlacementId);
        kill();
    }
}

void CoinCollect::exeCountUp() {
    if (al::isFirstStep(this))
        alPadRumbleFunction::startPadRumble(this, "コッ（弱）", 500.0f, 2000.0f, -1);

    if (!mIsSurfaceUpdated && !al::isInWater(this)) {
        sead::Vector3f surface = sead::Vector3f::zero;
        al::calcFindWaterSurface(&surface, nullptr, this, al::getTrans(this), sead::Vector3f::ey,
                                 200.0f);
        mSurfaceMatrix.setBase(3, surface);
        al::startHitReaction(this, "水しぶき");
        mIsSurfaceUpdated = true;
    }
    if (al::updateNerveState(this)) {
        CoinCollectWatcher* watcher =
            al::getSceneObj<CoinCollectWatcher>(this, SceneObjID_CoinCollectWatcher);
        watcher->countup(this);
        GameDataFunction::addCoinCollect(this, mPlacementId);

        rs::tryShowCapMsgCollectCoinGetFirst(this);
        if (mWaterSurfaceShadow != nullptr)
            mWaterSurfaceShadow->disappearShadow();

        al::startHitReaction(this, "釣り取得");
        kill();
    }
}

void CoinCollect::exeBlow() {
    if (al::isFirstStep(this)) {
        al::invalidateClipping(this);
        al::onCollide(this);
    }

    if (mMtxConnector != nullptr)
        al::connectPoseQT(this, mMtxConnector);

    al::setQuat(this, sead::Quatf::unit);
    al::rotateQuatYDirDegree(this, mRotateCalculator->getRotate());
    al::addVelocityToGravity(this, 1.5f);
    al::scaleVelocity(this, 0.99f);

    if (al::getVelocity(this).y < 0.0f && al::isCollidedGround(this)) {
        if (al::calcSpeed(this) < 15.0f) {
            if (mMtxConnector != nullptr)
                al::attachMtxConnectorToCollision(mMtxConnector, this, false);
            al::setVelocityZero(this);
            al::setNerve(this, &NrvCoinCollect.Wait);
            al::offCollide(this);
            return;
        }
        al::getVelocityPtr(this)->y *= -0.75f;
    }
}
