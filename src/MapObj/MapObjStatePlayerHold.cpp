#include "MapObj/MapObjStatePlayerHold.h"

#include <math/seadMathCalcCommon.h>
#include <math/seadMatrix.h>

#include "Library/Collision/Collider.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathUtil.h"
#include "Library/Matrix/MatrixUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Shadow/ActorShadowUtil.h"

#include "Util/DemoUtil.h"
#include "Util/PlayerUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(MapObjStatePlayerHold, Hold)

NERVES_MAKE_STRUCT(MapObjStatePlayerHold, Hold)
}  // namespace

MapObjStatePlayerHold::MapObjStatePlayerHold(al::LiveActor* actor,
                                             const sead::Vector3f& localOffset,
                                             const sead::Vector3f& localRotate)
    : al::ActorStateBase("プレイヤーに持たれるステート", actor), mLocalOffset(localOffset),
      mLocalRotate(localRotate) {
    initNerve(&NrvMapObjStatePlayerHold.Hold, 0);
}

void MapObjStatePlayerHold::initUseColliderPush(f32 pushForce) {
    mIsUseColliderPush = true;
    mColliderRadius = al::getColliderRadius(mActor);
    mColliderPushForce = pushForce;
}

void MapObjStatePlayerHold::initShadowMaskDropLengthCtrl(const char* shadowMaskName) {
    mShadowMaskName = shadowMaskName;
    mShadowMaskDropLength = al::getShadowMaskDropLength(mActor, shadowMaskName);
}

void MapObjStatePlayerHold::appear() {
    setDead(false);
    mTotalPush = {0.0f, 0.0f, 0.0f};
    al::invalidateClipping(mActor);

    if (mIsUseColliderPush) {
        mColliderUpOffset = 0;
        al::setColliderRadius(mActor,
                              sead::Mathf::clampMax(mColliderRadius * mColliderPushForce, 40.0f));
        al::getActorCollider(mActor)->onInvalidate();
        al::onCollide(mActor);
        if (mIsCarryUp)
            mColliderUpOffset = 1000;
    } else {
        al::offCollide(mActor);
    }

    al::stopDitherAnimAutoCtrl(mActor);
    al::invalidateOcclusionQuery(mActor);
    mIsResetPositionNeeded = false;
    al::setNerve(this, &NrvMapObjStatePlayerHold.Hold);
}

void MapObjStatePlayerHold::kill() {
    mPlayerHitSensor = nullptr;
    al::validateClipping(mActor);

    if (mIsUseColliderPush)
        al::setColliderRadius(mActor, mColliderRadius);
    else
        al::onCollide(mActor);

    if (mShadowMaskName)
        al::setShadowMaskDropLength(mActor, mShadowMaskDropLength, mShadowMaskName);

    al::restartDitherAnimAutoCtrl(mActor);
    al::setModelAlphaMask(mActor, 1.0f);
    al::validateOcclusionQuery(mActor);
    mTotalPush = {0.0f, 0.0f, 0.0f};
    setDead(true);
}

void MapObjStatePlayerHold::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    al::sendMsgPlayerItemGet(other, self);
}

bool MapObjStatePlayerHold::receiveMsgNoRelease(const al::SensorMsg* message, al::HitSensor* other,
                                                al::HitSensor* self) {
    if (al::isMsgPlayerDisregard(message))
        return al::isNerve(this, &NrvMapObjStatePlayerHold.Hold);

    sead::Vector3f pushPos = {0.0f, 0.0f, 0.0f};
    if (al::tryReceiveMsgPushAndCalcPushTrans(&pushPos, message, mActor, other, self, 15.0f)) {
        al::sendMsgCollidePush(mPlayerHitSensor, self, pushPos);
        return true;
    }

    if (al::isMsgPlayerCarryKeepDemo(message)) {
        rs::addDemoActor(mActor, false);
        return true;
    }

    if (rs::isMsgPlayerCarryShineGetStart(message)) {
        al::hideModelIfShow(mActor);
        rs::addDemoActor(mActor, false);
        return true;
    }

    if (rs::isMsgPlayerCarryShineGetEnd(message)) {
        al::showModelIfHide(mActor);
        return true;
    }

    if (rs::isMsgPlayerCarryCameraSubjectiveStart(message)) {
        al::hideModelIfShow(mActor);
        return true;
    }

    if (rs::isMsgPlayerCarryCameraSubjectiveEnd(message)) {
        al::showModelIfHide(mActor);
        return true;
    }

    if (al::isMsgPlayerCarryWarp(message)) {
        mIsResetPositionNeeded = true;
        return true;
    }
    return false;
}

bool MapObjStatePlayerHold::receiveMsgRelease(const al::SensorMsg* message, al::HitSensor* other,
                                              al::HitSensor* self) {
    if (al::isMsgHoldReleaseAll(message)) {
        al::LiveActor* actor = mActor;
        sead::Vector3f frontDir = {0.0f, 0.0f, 0.0f};
        rs::calcPlayerFrontDir(&frontDir, actor);
        al::makeQuatFrontUp(al::getQuatPtr(actor), frontDir, sead::Vector3f::ey);

        if (mIsUseColliderPush)
            prepareThrowCollide();
        kill();
        return true;
    }
    return false;
}

void MapObjStatePlayerHold::prepareThrowCollide() {
    al::LiveActor* actor = mActor;
    sead::Vector3f direction = {0.0f, 0.0f, 0.0f};
    al::Collider* collider = actor->getCollider();

    collider->setRadius(sead::Mathf::clampMax(mColliderRadius * mColliderPushForce, 40.0f));
    collider->onInvalidate();
    collider->setRadius(mColliderRadius);

    al::setTrans(actor, collider->collide(direction) + al::getTrans(actor));
}

bool MapObjStatePlayerHold::receiveMsgCancel(const al::SensorMsg* message, al::HitSensor* other,
                                             al::HitSensor* self) {
    if (al::isMsgHoldCancel(message)) {
        if (mIsUseColliderPush)
            prepareThrowCollide();
        kill();
        return true;
    }
    return false;
}

bool MapObjStatePlayerHold::tryStartCarryFront(const al::SensorMsg* message,
                                               al::HitSensor* sensor) {
    if (al::isMsgPlayerCarryFront(message)) {
        mIsCarryUp = false;
        mPlayerHitSensor = sensor;
        return true;
    }
    return false;
}

bool MapObjStatePlayerHold::tryStartCarryFrontWallKeep(const al::SensorMsg* message,
                                                       al::HitSensor* sensor) {
    if (al::isMsgPlayerCarryFrontWallKeep(message)) {
        mIsCarryUp = false;
        mPlayerHitSensor = sensor;
        return true;
    }
    return false;
}

bool MapObjStatePlayerHold::tryStartCarryUp(const al::SensorMsg* message, al::HitSensor* sensor) {
    if (al::isMsgPlayerCarryUp(message)) {
        mIsCarryUp = true;
        mPlayerHitSensor = sensor;
        return true;
    }
    return false;
}

void MapObjStatePlayerHold::tryCancelHold(al::HitSensor* sensor) {
    al::sendMsgHoldCancel(mPlayerHitSensor, sensor);
}

void MapObjStatePlayerHold::updateCollider(al::HitSensor* sensor) {
    al::LiveActor* actor = mActor;
    al::Collider* collider = actor->getCollider();

    if (al::isNoCollide(actor)) {
        collider->onInvalidate();
        return;
    }

    s32 nextOffset = sead::Mathi::clampMin(mColliderUpOffset - 3, 0);
    f32 maxColliderRadius = mColliderRadius * mColliderPushForce;
    f32 colliderRadius = nextOffset * 10.0f + 40.0f;

    collider->setRadius(sead::Mathf::min(colliderRadius, maxColliderRadius));
    sead::Vector3f pushPos = collider->collide(sead::Vector3f::zero);

    if (al::isOnGround(actor, 0)) {
        if (rs::isPlayerOnGround(actor)) {
            al::verticalizeVec(&pushPos, al::getGravity(actor), pushPos);
        } else {
            sead::Vector3f groundPos = al::getCollidedGroundPos(actor);
            sead::Vector3f normal = al::getActorTrans(mPlayerHitSensor) - groundPos;
            al::verticalizeVec(&normal, al::getGravity(actor), normal);
            if (!al::tryNormalizeOrZero(&normal)) {
                rs::calcPlayerFrontDir(&normal, actor);
                normal.negate();
            }
            pushPos += mColliderRadius * normal * mColliderPushForce * 0.5f;
        }
    } else if (mIsCarryUp && al::isLessStep(this, 3) && al::isCollidedCeiling(actor)) {
        sead::Vector3f ceilingPos = al::getCollidedCeilingPos(actor);
        sead::Vector3f normal = al::getActorTrans(mPlayerHitSensor) - ceilingPos;
        al::verticalizeVec(&normal, al::getGravity(actor), normal);
        if (!al::tryNormalizeOrZero(&normal)) {
            rs::calcPlayerFrontDir(&normal, actor);
            normal.negate();
        }
        pushPos += mColliderRadius * normal * mColliderPushForce;
    }
    mTotalPush = pushPos;
    al::sendMsgCollidePush(mPlayerHitSensor, sensor, pushPos);
    mColliderUpOffset = al::converge(mColliderUpOffset, 1000, 1);
}

void MapObjStatePlayerHold::exeHold() {
    rs::syncPlayerModelAlpha(mActor);
    sead::Matrix34f poseMtx = sead::Matrix34f::ident;
    rs::calcPlayerHoldMtx(&poseMtx, mPlayerHitSensor);

    sead::Vector3f position = {0.0f, 0.0f, 0.0f};
    al::calcTransLocalOffsetByMtx(&position, poseMtx, mLocalOffset);

    sead::Matrix34f rotationMatrix;
    sead::Vector3f rotate(sead::Mathf::deg2rad(mLocalRotate.x),
                          sead::Mathf::deg2rad(mLocalRotate.y),
                          sead::Mathf::deg2rad(mLocalRotate.z));
    rotationMatrix.makeR(rotate);

    poseMtx = poseMtx * rotationMatrix;
    poseMtx.setBase(3, position);
    al::updatePoseMtx(mActor, &poseMtx);

    if (mIsResetPositionNeeded) {
        al::resetPosition(mActor);
        mIsResetPositionNeeded = false;
    }

    if (mShadowMaskName) {
        al::setShadowMaskDropLength(
            mActor, mShadowMaskDropLength + rs::getPlayerShadowDropLength(mActor), mShadowMaskName);
    }
}
