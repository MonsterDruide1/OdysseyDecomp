#include "Item/LifeUpItem.h"

#include <cmath>
#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/Collision/CollisionParts.h"
#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/Collision/PartsConnectorUtil.h"
#include "Library/Demo/DemoFunction.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nature/FireSurfaceFinder.h"
#include "Library/Nature/NatureUtil.h"
#include "Library/Nature/WaterSurfaceFinder.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Shadow/ActorShadowUtil.h"

#include "Item/LifeUpItemFunction.h"
#include "Item/LifeUpItemStateAutoGet.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolderAccessor.h"
#include "Util/ItemUtil.h"
#include "Util/PlayerUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(LifeUpItem, Wait);
NERVE_IMPL(LifeUpItem, AutoGetDemo);
NERVE_IMPL(LifeUpItem, Land);
NERVE_IMPL(LifeUpItem, Get);
NERVE_IMPL(LifeUpItem, TakeWater);
NERVE_IMPL(LifeUpItem, PopUp);
NERVE_IMPL(LifeUpItem, LandWater);
NERVE_IMPL(LifeUpItem, FloatWater);

NERVES_MAKE_NOSTRUCT(LifeUpItem, Get);
NERVES_MAKE_NOSTRUCT(LifeUpItem, LandWater);
NERVES_MAKE_NOSTRUCT(LifeUpItem, FloatWater);
NERVES_MAKE_NOSTRUCT(LifeUpItem, Wait, AutoGetDemo, Land, TakeWater, PopUp);

inline void updateShadowMaskDropLength(LifeUpItem* actor, f32 dropLength) {
    sead::Vector3f trans = al::getTrans(actor);
    sead::Vector3f hitPos;
    sead::Vector3f upDir;
    al::calcUpDir(&upDir, actor);
    if (alCollisionUtil::getFirstPolyOnArrow(actor, &hitPos, nullptr, trans + upDir * 50.0f,
                                             -(upDir * (dropLength + 50.0f)), nullptr, nullptr)) {
        al::setShadowMaskDropLength(
            actor, sead::Mathf::min((trans - hitPos).length() + 100.0f, dropLength));
    }
}

void appearPopUpSub(LifeUpItem* actor, const char* actionName, al::MtxConnector* mtxConnector,
                    f32 frontSpeed, f32 gravitySpeed) {
    al::disconnectMtxConnector(mtxConnector);
    al::onCollide(actor);
    sead::Vector3f frontDir = {0.0f, 0.0f, 0.0f};
    al::calcFrontDir(&frontDir, actor);
    al::setVelocity(actor, frontDir * frontSpeed - al::getGravity(actor) * gravitySpeed);
    al::setNerve(actor, &PopUp);
    al::showModelIfHide(actor);
    al::validateHitSensors(actor);
    al::invalidateClipping(actor);
    al::startAction(actor, actionName);
    actor->appear();
}
}  // namespace

LifeUpItem::LifeUpItem(const char* name) : al::LiveActor(name) {}

void LifeUpItem::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "LifeUpItem", nullptr);
    al::initNerve(this, &Wait, 1);
    al::offCollide(this);
    if (mIsCreateMtxConnector) {
        if (al::isPlaced(info))
            mMtxConnector = al::tryCreateMtxConnector(this, info);
        else
            mMtxConnector = al::createMtxConnector(this);
    }

    mShadowMaskDropLength = al::getShadowMaskDropLengthMax(this);
    mWaterSurfaceFinder = new al::WaterSurfaceFinder(this);
    mFireSurfaceFinder = new al::FireSurfaceFinder(this);
    mStateAutoGet = new LifeUpItemStateAutoGet(this, false);
    al::initNerveState(this, mStateAutoGet, &AutoGetDemo, "自動取得");
    al::startAction(this, "Wait");
    al::setActionFrameRate(this, 0.0f);
    makeActorAlive();
}

void LifeUpItem::initAfterPlacement() {
    if (al::isDead(this))
        return;

    if (mMtxConnector) {
        const al::CollisionParts* collisionParts =
            al::attachMtxConnectorToCollision(mMtxConnector, this, false);
        if (collisionParts)
            mConnectedHost = collisionParts->getConnectedHost();
    }

    updateShadowMaskDropLength(this, mShadowMaskDropLength);
}

void LifeUpItem::control() {
    if (mMtxConnector && (al::isNerve(this, &Wait) || al::isNerve(this, &Land)))
        al::connectPoseQT(this, mMtxConnector);

    if (al::tryKillByDeathArea(this))
        al::startHitReaction(this, "消滅");
}

void LifeUpItem::appearPopUp() {
    mConnectedHost = nullptr;
    mIsValidateClipping = true;
    appearPopUpSub(this, "AppearPopUp", mMtxConnector, 3.0f, 11.0f);
    al::startHitReaction(this, "飛出し出現");
}

void LifeUpItem::appearPopUpBack() {
    mConnectedHost = nullptr;
    mIsValidateClipping = true;
    appearPopUpSub(this, "AppearPopUp", mMtxConnector, 3.0f, 11.0f);

    sead::Vector3f upDir;
    al::calcUpDir(&upDir, this);
    sead::Vector3f backDir;
    al::calcBackDir(&backDir, this);
    sead::Quatf quat;
    al::makeQuatUpFront(&quat, upDir, backDir);
    al::updatePoseQuat(this, quat);
    al::startHitReaction(this, "飛出し出現");
}

void LifeUpItem::appearPopUpAbove() {
    mConnectedHost = nullptr;
    mIsValidateClipping = true;
    appearPopUpSub(this, "AppearAbove", mMtxConnector, 0.0f, 14.0f);
    al::startHitReaction(this, "飛出し出現");
}

void LifeUpItem::appearSlot() {
    mConnectedHost = nullptr;
    mIsValidateClipping = true;
    appearPopUpSub(this, "AppearPopUp", mMtxConnector, 3.0f, 11.0f);
    al::startHitReaction(this, "スロット出現");
}

void LifeUpItem::appearInvalidClipping() {
    mConnectedHost = nullptr;
    mIsValidateClipping = false;
    appearPopUpSub(this, "AppearPopUp", mMtxConnector, 3.0f, 11.0f);
    al::startHitReaction(this, "飛出し出現");
}

void LifeUpItem::appearAmiiboTouch() {
    mConnectedHost = nullptr;
    mIsValidateClipping = true;
    appearPopUpSub(this, "AppearAbove", mMtxConnector, 0.0f, 14.0f);
    al::startHitReaction(this, "飛出し出現");
    al::setNerve(this, &AutoGetDemo);
}

void LifeUpItem::get() {
    if (al::isNerve(this, &Get))
        return;

    rs::recoveryPlayerOxygen(this);
    al::setVelocityZero(this);
    al::startHitReaction(this, "取得");
    if (!GameDataFunction::isPlayerHitPointMax(this)) {
        GameDataFunction::recoveryPlayer(this);
        kill();
        return;
    }

    if (_145) {
        kill();
        return;
    }

    if (!mHitSensor)
        mHitSensor = al::getHitSensor(this, "Body");

    al::hideModel(this);
    al::invalidateHitSensors(this);
    al::invalidateClipping(this);
    al::startAction(this, "Hide");
    al::setNerve(this, &Get);
}

void LifeUpItem::exePopUp() {
    al::addVelocityToGravity(this, 0.4f);
    al::limitVelocitySeparateHV(this, al::getGravity(this), 10.0f, 20.0f);
    updateShadowMaskDropLength(this, mShadowMaskDropLength);
    if (LifeUpItemFunction::updateSurfaceFinder(mWaterSurfaceFinder, this, 0.4f) ||
        LifeUpItemFunction::updateSurfaceFinder(mFireSurfaceFinder, this, 0.4f)) {
        al::offCollide(this);
        al::setNerve(this, &TakeWater);
        return;
    }

    if (al::isOnGround(this, 0)) {
        if (al::isActionPlaying(this, "Wait")) {
            sead::Vector3f upDir;
            al::calcUpDir(&upDir, this);
            sead::Vector3f frontDir;
            al::calcJointFrontDir(&frontDir, this, "LifeUpItem");
            sead::Quatf quat;
            al::makeQuatUpFront(&quat, upDir, frontDir);
            al::updatePoseQuat(this, quat);
        }

        const al::CollisionParts* collisionParts =
            al::attachMtxConnectorToCollision(mMtxConnector, this, false);
        if (collisionParts)
            mConnectedHost = collisionParts->getConnectedHost();
        al::setConnectorBaseQuatTrans(this, mMtxConnector);
        al::offCollide(this);
        al::setVelocityZero(this);
        if (mIsValidateClipping)
            al::validateClipping(this);
        al::startAction(this, "Land");
        al::setNerve(this, &Land);
    }
}

void LifeUpItem::exeLand() {
    if (al::isActionEnd(this))
        al::setNerve(this, &Wait);
}

void LifeUpItem::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");

    if (mConnectedHost && al::isDead(mConnectedHost)) {
        mConnectedHost = nullptr;
        al::disconnectMtxConnector(mMtxConnector);
        al::onCollide(this);
        al::invalidateClipping(this);
        al::stopAction(this);
        al::setNerve(this, &PopUp);
    }
}

void LifeUpItem::exeTakeWater() {
    LifeUpItemFunction::updateVelocityInWaterAndFire(this, mWaterSurfaceFinder, mFireSurfaceFinder,
                                                     25.0f, 0.4f);
    if (al::isInWater(this)) {
        al::startHitReaction(this, "水に入る");
        al::setNerve(this, &LandWater);
        return;
    }

    if (al::isInFire(this)) {
        al::startHitReaction(this, "着溶岩");
        al::setNerve(this, &LandWater);
    }
}

void LifeUpItem::exeLandWater() {
    if (al::isFirstStep(this))
        al::startAction(this, "LandFloat");

    LifeUpItemFunction::updateVelocityInWaterAndFire(this, mWaterSurfaceFinder, mFireSurfaceFinder,
                                                     25.0f, 0.4f);
    if (al::isActionEnd(this))
        al::setNerve(this, &FloatWater);
}

void LifeUpItem::exeFloatWater() {
    if (al::isFirstStep(this))
        al::startAction(this, "Float");

    LifeUpItemFunction::updateVelocityInWaterAndFire(this, mWaterSurfaceFinder, mFireSurfaceFinder,
                                                     25.0f, 0.4f);
}

void LifeUpItem::exeGet() {
    if (al::isFirstStep(this))
        mCoinAppearCount = 0;

    if (rs::tryAppearMultiCoinFromObj(this, mHitSensor, al::getNerveStep(this), 150.0f))
        mCoinAppearCount++;

    if (mCoinAppearCount >= 5)
        kill();
}

void LifeUpItem::exeAutoGetDemo() {
    if (al::updateNerveState(this))
        get();
}

bool LifeUpItem::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                            al::HitSensor* self) {
    if (rs::isMsgItemGetAll(message) &&
        ((!al::isNerve(this, &PopUp) && !al::isNerve(this, &AutoGetDemo)) ||
         !al::isLessEqualStep(this, 40))) {
        mHitSensor = other;
        rs::sendMsgAckLifeUp(other, self);
        get();
        return true;
    }

    if (rs::isMsgKillByHomeDemo(message) || rs::isMsgKillByShineGet(message) ||
        rs::isMsgKillByMoonRockDemo(message) || rs::isMsgKillByBossBattleDemo(message)) {
        al::killForceBeforeDemo(this);
        return true;
    }

    return false;
}
