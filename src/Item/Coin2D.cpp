#include "Item/Coin2D.h"

#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/Collision/PartsConnector.h"
#include "Library/Controller/PadRumbleFunction.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

#include "System/GameDataFunction.h"
#include "Util/ActorDimensionKeeper.h"
#include "Util/ActorDimensionUtil.h"
#include "Util/AreaUtil.h"
#include "Util/ItemUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(Coin2D, Appear);
NERVE_IMPL(Coin2D, Wait);
NERVE_IMPL(Coin2D, Got);
NERVE_IMPL_(Coin2D, GotNoCoin, Got);
NERVE_IMPL(Coin2D, CountUp);

NERVES_MAKE_NOSTRUCT(Coin2D, GotNoCoin);
NERVES_MAKE_STRUCT(Coin2D, Wait, Appear, CountUp, Got);
}  // namespace

Coin2D::Coin2D(const char* name) : al::LiveActor(name) {}

void Coin2D::init(const al::ActorInitInfo& initInfo) {
    using Coin2DFunctor = al::FunctorV0M<Coin2D*, void (Coin2D::*)()>;

    al::initActorWithArchiveName(this, initInfo, "CoinDot", nullptr);
    mMtxConnector = al::tryCreateMtxConnector(this, initInfo);
    if (mMtxConnector != nullptr)
        al::tryGetArg(&mIsConnectToCollisionBack, initInfo, "IsConnectToCollisionBack");

    al::initNerve(this, &NrvCoin2D.Wait, 0);
    al::tryAddDisplayOffset(this, initInfo);
    mDimensionKeeper = rs::createDimensionKeeper(this);
    rs::updateDimensionKeeper(mDimensionKeeper);

    mIsPlaced = al::isPlaced(initInfo);
    if (mIsPlaced)
        rs::snap2DParallelizeFront(this, this, 500.0f);

    if (al::listenStageSwitchOnAppear(this, Coin2DFunctor(this, &Coin2D::listenAppear)))
        makeActorDead();
    else
        makeActorAlive();

    al::startAction(this, "Wait");
}

void Coin2D::initAfterPlacement() {
    if (mMtxConnector != nullptr) {
        if (!mIsConnectToCollisionBack) {
            al::attachMtxConnectorToCollision(mMtxConnector, this, 50.0f, 400.0f);
            return;
        }
        sead::Vector3f frontDir = sead::Vector3f::zero;
        al::calcFrontDir(&frontDir, this);
        al::attachMtxConnectorToCollision(
            mMtxConnector, this, al::getTrans(this) + frontDir * 50.0f, frontDir * -400.0f);
    }
}

void Coin2D::appear() {
    al::LiveActor::appear();
    if (mMtxConnector != nullptr)
        al::connectPoseQT(this, mMtxConnector);
}

void Coin2D::control() {
    if (mMtxConnector != nullptr)
        al::connectPoseQT(this, mMtxConnector);
}

void Coin2D::endClipped() {
    rs::syncCoin2DAnimFrame(this, "Wait");
    al::LiveActor::endClipped();
}

bool Coin2D::receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self) {
    if (al::isMsgPlayerDisregard(message))
        return true;

    if (rs::isMsgBlockUpperPunch2D(message)) {
        if (al::isNerve(this, &NrvCoin2D.Wait) || al::isNerve(this, &NrvCoin2D.Appear)) {
            al::invalidateClipping(this);
            sead::Vector3f up = sead::Vector3f::zero;
            al::calcQuatUp(&up, this);
            al::setVelocityToDirection(this, up, 16.0f);
            al::setNerve(this, &NrvCoin2D.CountUp);
            return true;
        }
        return false;
    }

    if (rs::isMsgItemGet2D(message) &&
        (al::isNerve(this, &NrvCoin2D.Wait) || al::isNerve(this, &NrvCoin2D.Appear))) {
        al::startHitReaction(this, "取得");
        al::setNerve(this, &NrvCoin2D.Got);
        return true;
    }

    return false;
}

ActorDimensionKeeper* Coin2D::getActorDimensionKeeper() const {
    return mDimensionKeeper;
}

void Coin2D::listenAppear() {
    appear();
    al::invalidateClipping(this);
    al::setNerve(this, &NrvCoin2D.Appear);
}

void Coin2D::appearCountUp() {
    appear();
    al::invalidateClipping(this);
    sead::Vector3f up = sead::Vector3f::zero;
    al::calcQuatUp(&up, this);
    al::setVelocityToDirection(this, up, 16.0f);

    al::AreaObj* areaObj = rs::tryFind2DAreaObj(this, nullptr, nullptr);
    if (areaObj != nullptr) {
        sead::Vector3f lockDir = sead::Vector3f::zero;
        rs::calc2DAreaLockDir(&lockDir, areaObj, al::getTrans(this));
        sead::Quatf quat = sead::Quatf::unit;
        al::makeQuatFrontUp(&quat, -lockDir, up);
        al::setQuat(this, quat);
    }

    al::setNerve(this, &NrvCoin2D.CountUp);
}

void Coin2D::get() {
    al::startHitReaction(this, "取得");
    al::setNerve(this, &NrvCoin2D.Got);
}

bool Coin2D::isGot() const {
    return al::isNerve(this, &NrvCoin2D.Got);
}

void Coin2D::exeAppear() {
    if (al::isFirstStep(this))
        al::startAction(this, "Appear");

    if (al::isActionEnd(this)) {
        al::startAction(this, "Wait");
        rs::syncCoin2DAnimFrame(this, "Wait");
        al::setNerve(this, &NrvCoin2D.Wait);
    }
}

void Coin2D::exeWait() {}

void Coin2D::exeGot() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Got");
        al::onStageSwitch(this, "SwitchGetOn");
        alPadRumbleFunction::startPadRumble(this, "コッ（微弱）", 1000.0f, 3000.0f);
        if (!al::isNerve(this, &GotNoCoin))
            GameDataFunction::addCoin(this, 1);
    }
    if (al::isActionEnd(this))
        kill();
}

void Coin2D::exeCountUp() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Got");
        GameDataFunction::addCoin(this, 1);
        al::startHitReaction(this, "自動取得[開始]");
    }

    sead::Vector3f gravityDir = {0.0f, 0.0f, 0.0f};
    al::calcQuatGravity(&gravityDir, al::getQuat(this));
    al::addVelocityToDirection(this, gravityDir, 0.6f);
    al::scaleVelocity(this, 0.99f);
    al::getVelocity(this);
    al::getQuat(this);

    if (al::isGreaterEqualStep(this, 32)) {
        al::startHitReaction(this, "自動取得[終了]");
        al::setNerve(this, &GotNoCoin);
    }
}
