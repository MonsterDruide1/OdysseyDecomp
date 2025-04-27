#include "Library/LiveActor/LiveActor.h"

#include "Library/Action/ActorActionKeeper.h"
#include "Library/Audio/System/AudioKeeper.h"
#include "Library/Collision/Collider.h"
#include "Library/Effect/EffectKeeper.h"
#include "Library/Execute/ActorExecuteInfo.h"
#include "Library/Execute/ExecuteUtil.h"
#include "Library/HitSensor/HitSensorKeeper.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSceneInfo.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/LiveActorFlag.h"
#include "Library/LiveActor/LiveActorFunction.h"
#include "Library/Matrix/MatrixUtil.h"
#include "Library/Model/ModelKeeper.h"
#include "Library/Nature/NatureDirector.h"
#include "Library/Nerve/NerveKeeper.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Rail/RailKeeper.h"
#include "Library/Screen/ScreenPointKeeper.h"
#include "Library/Shader/ActorOcclusionKeeper.h"
#include "Library/Shadow/ActorShadowUtil.h"
#include "Library/Shadow/ShadowKeeper.h"
#include "Library/Stage/StageSwitchKeeper.h"
#include "Project/Action/ActionPadAndCameraCtrl.h"
#include "Project/Action/ActionSeCtrl.h"
#include "Project/Clipping/ClippingFunction.h"
#include "Project/Item/ActorItemKeeper.h"
#include "Project/Item/ActorScoreKeeper.h"
#include "Project/Light/ActorPrepassLightKeeper.h"

namespace al {

LiveActor::LiveActor(const char* actorName) : mName(actorName) {
    mFlags = new LiveActorFlag();
    mShadowKeeper = new ShadowKeeper();
}

CollisionDirector* LiveActor::getCollisionDirector() const {
    return mSceneInfo->collisionDirector;
}

AreaObjDirector* LiveActor::getAreaObjDirector() const {
    return mSceneInfo->areaObjDirector;
}

CameraDirector* LiveActor::getCameraDirector() const {
    return mSceneInfo->cameraDirector;
}

NatureDirector* LiveActor::getNatureDirector() const {
    return mSceneInfo->natureDirector;
}

SceneObjHolder* LiveActor::getSceneObjHolder() const {
    return mSceneInfo->sceneObjHolder;
}

void LiveActor::draw() const {}

void LiveActor::initAfterPlacement() {
    tryInitFixedModelGpuBuffer(this);
    if (mPrePassLightKeeper)
        mPrePassLightKeeper->initAfterPlacement();
}

void LiveActor::appear() {
    makeActorAlive();
}

void LiveActor::makeActorAlive() {
    if (mHitSensorKeeper)
        mHitSensorKeeper->validateBySystem();
    if (mScreenPointKeeper)
        mScreenPointKeeper->validateBySystem();
    mFlags->isDead = false;
    if (isClipped(this))
        endClipped();
    if (!isHideModel(this) && mModelKeeper)
        mModelKeeper->show();
    resetPosition(this);
    if (mCollisionParts)
        validateCollisionPartsBySystem(this);
    if (mHitSensorKeeper)
        mHitSensorKeeper->update();
    alClippingFunction::addToClippingTarget(this);
    if (mExecuteInfo) {
        alActorSystemFunction::addToExecutorMovement(this);
        if (!isHideModel(this) && mExecuteInfo->getDrawerCount() >= 1)
            alActorSystemFunction::addToExecutorDraw(this);
    }
    if (getAudioKeeper())
        getAudioKeeper()->appear();
    if (mPrePassLightKeeper)
        mPrePassLightKeeper->appear(isHideModel(this));
    if (mOcclusionKeeper)
        mOcclusionKeeper->appear(isHideModel(this));
    if (mShadowKeeper && !isHideModel(this))
        mShadowKeeper->show();
    if (mSubActorKeeper)
        alSubActorFunction::trySyncAlive(mSubActorKeeper);
}

void LiveActor::kill() {
    makeActorDead();
}

void LiveActor::makeActorDead() {
    mFlags->isDead = true;
    if (mModelKeeper)
        mModelKeeper->hide();
    if (mHitSensorKeeper)
        mHitSensorKeeper->invalidateBySystem();
    if (mPoseKeeper)
        setVelocityZero(this);
    if (mScreenPointKeeper)
        mScreenPointKeeper->invalidateBySystem();
    alClippingFunction::removeFromClippingTarget(this);
    if (mCollider)
        mCollider->onInvalidate();
    if (mCollisionParts)
        invalidateCollisionPartsBySystem(this);
    if (getEffectKeeper())
        getEffectKeeper()->deleteAndClearEffectAll();
    if (getAudioKeeper())
        getAudioKeeper()->kill();
    if (mShadowKeeper)
        hideShadow(this);
    if (mActionKeeper) {
        if (mActionKeeper->getPadAndCameraCtrl())
            mActionKeeper->getPadAndCameraCtrl()->notifyActorDead();
        if (mActionKeeper && mActionKeeper->getSeCtrl())
            mActionKeeper->getSeCtrl()->resetAction(false);
    }
    if (mPrePassLightKeeper)
        mPrePassLightKeeper->requestKill();
    if (mOcclusionKeeper)
        mOcclusionKeeper->requestKill();
    if (mExecuteInfo) {
        alActorSystemFunction::removeFromExecutorMovement(this);
        if (mExecuteInfo->getDrawerCount() >= 1)
            alActorSystemFunction::removeFromExecutorDraw(this);
    }
    if (mSubActorKeeper)
        alSubActorFunction::trySyncDead(mSubActorKeeper);
}

void LiveActor::movement() {
    if (mFlags->isDead || (mFlags->isClipped && !mFlags->isDrawClipped))
        return;
    if (mActionKeeper)
        mActionKeeper->updatePrev();
    if (mModelKeeper)
        mModelKeeper->update();
    if (mFlags->isMaterialCodeValid && isCollidedGround(this)) {
        setMaterialCode(this, getCollidedFloorMaterialCodeName(this));
        if (mFlags->isPuddleMaterialValid)
            updateMaterialCodePuddle(this);
    }
    if (mHitSensorKeeper) {
        mHitSensorKeeper->attackSensor();
        if (mFlags->isDead)
            return;
    }
    if (mNerveKeeper) {
        mNerveKeeper->update();
        if (mFlags->isDead)
            return;
    }
    control();
    if (mFlags->isDead)
        return;
    updateCollider();
    if (isUpdateMovementEffectAudioCollisionSensor(this)) {
        if (mEffectKeeper)
            mEffectKeeper->update();
        if (mCollisionParts) {
            if (!mModelKeeper || !mModelKeeper->isFixedModel()) {
                sead::Matrix34f baseMtx;
                mPoseKeeper->calcBaseMtx(&baseMtx);
                preScaleMtx(&baseMtx, mPoseKeeper->getScale());
                syncCollisionMtx(this, &baseMtx);
            }
        }
        if (mHitSensorKeeper)
            mHitSensorKeeper->update();
    }
    if (mActionKeeper)
        mActionKeeper->updatePost();
    if (mShadowKeeper)
        mShadowKeeper->update();
    if (mScreenPointKeeper)
        mScreenPointKeeper->update();
    if (mModelKeeper)
        mModelKeeper->updateLast();
}

void LiveActor::calcAnim() {
    sead::Matrix34f baseMtx;
    alActorPoseFunction::calcBaseMtx(&baseMtx, this);
    if (mModelKeeper && mModelKeeper->isNeedSetBaseMtxAndCalcAnim())
        setBaseMtxAndCalcAnim(this, baseMtx, getScale(this));
    if (mCollisionParts) {
        preScaleMtx(&baseMtx, getScale(this));
        syncCollisionMtx(this, &baseMtx);
    }
    if (getEffectKeeper())
        getEffectKeeper()->update();
    if (mHitSensorKeeper)
        mHitSensorKeeper->update();
    if (mOcclusionKeeper)
        mOcclusionKeeper->updateAndRequest();
}

void LiveActor::startClipped() {
    mFlags->isClipped = true;
    if (mModelKeeper)
        mModelKeeper->hide();
    if (!mFlags->isDrawClipped) {
        if (mHitSensorKeeper)
            mHitSensorKeeper->invalidateBySystem();
        if (getAudioKeeper())
            getAudioKeeper()->startClipped();
        if (mActionKeeper && mActionKeeper->getPadAndCameraCtrl())
            mActionKeeper->getPadAndCameraCtrl()->notifyActorStartClipped();
    }
    if (getEffectKeeper())
        getEffectKeeper()->offCalcAndDraw();
    if (mShadowKeeper)
        mShadowKeeper->hide();
    if (mScreenPointKeeper)
        mScreenPointKeeper->invalidateBySystem();
    if (mPrePassLightKeeper)
        mPrePassLightKeeper->hideModel();
    if (mOcclusionKeeper)
        mOcclusionKeeper->hideModel();
    if (mExecuteInfo) {
        if (!mFlags->isDrawClipped)
            alActorSystemFunction::removeFromExecutorMovement(this);
        if (mExecuteInfo->getDrawerCount() >= 1)
            alActorSystemFunction::removeFromExecutorDraw(this);
        if (mFlags->isDrawClipped && mModelKeeper && mModelKeeper->isIgnoreUpdateDrawClipping())
            setNeedSetBaseMtxAndCalcAnimFlag(this, false);
    }
    if (mSubActorKeeper)
        alSubActorFunction::trySyncClippingStart(mSubActorKeeper);
}

void LiveActor::endClipped() {
    mFlags->isClipped = false;
    if (!mFlags->isDrawClipped) {
        if (mHitSensorKeeper) {
            mHitSensorKeeper->validateBySystem();
            updateHitSensorsAll(this);
        }
        if (getAudioKeeper())
            getAudioKeeper()->endClipped();
        if (mActionKeeper && mActionKeeper->getPadAndCameraCtrl())
            mActionKeeper->getPadAndCameraCtrl()->notifyActorEndClipped();
    }
    if (getEffectKeeper())
        getEffectKeeper()->onCalcAndDraw();
    if (mScreenPointKeeper)
        mScreenPointKeeper->validateBySystem();
    if (mExecuteInfo) {
        if (!mFlags->isDrawClipped)
            alActorSystemFunction::addToExecutorMovement(this);
        if (mExecuteInfo->getDrawerCount() >= 1 && !isHideModel(this))
            alActorSystemFunction::addToExecutorDraw(this);
        if (mModelKeeper && mModelKeeper->isIgnoreUpdateDrawClipping())
            setNeedSetBaseMtxAndCalcAnimFlag(this, true);
    }
    if (!isHideModel(this) && mModelKeeper)
        mModelKeeper->show();
    if (mShadowKeeper && !isHideModel(this))
        mShadowKeeper->show();
    if (mPrePassLightKeeper)
        mPrePassLightKeeper->appear(isHideModel(this));
    if (mOcclusionKeeper)
        mOcclusionKeeper->appear(isHideModel(this));
    if (mSubActorKeeper)
        alSubActorFunction::trySyncClippingEnd(mSubActorKeeper);
}

const sead::Matrix34f* LiveActor::getBaseMtx() const {
    if (mModelKeeper)
        return mModelKeeper->getBaseMtx();
    if (mPoseKeeper)
        return mPoseKeeper->getMtxPtr();
    return nullptr;
}

RailRider* LiveActor::getRailRider() const {
    if (mRailKeeper)
        return mRailKeeper->getRailRider();
    return nullptr;
}

ActorSceneInfo* LiveActor::getSceneInfo() const {
    return mSceneInfo;
}

void LiveActor::initHitSensor(s32 amount) {
    mHitSensorKeeper = new HitSensorKeeper(amount);
}

void LiveActor::initCollider(f32 radius, f32 offsetY, u32 allocatedHitInfo) {
    mCollider = new Collider(getCollisionDirector(), getBaseMtx(), &getTrans(this),
                             &getGravity(this), radius, offsetY, allocatedHitInfo);
    mFlags->isCollideOff = false;
}

void LiveActor::initPoseKeeper(ActorPoseKeeperBase* poseKeeper) {
    mPoseKeeper = poseKeeper;
}

void LiveActor::initExecuteInfo(ActorExecuteInfo* executeInfo) {
    mExecuteInfo = executeInfo;
}

void LiveActor::initRailKeeper(const ActorInitInfo& info, const char* linkName) {
    mRailKeeper = tryCreateRailKeeper(*info.placementInfo, linkName);
}

void LiveActor::initModelKeeper(ModelKeeper* modelKeeper) {
    mModelKeeper = modelKeeper;
    offUpdateMovementEffectAudioCollisionSensor(this);
}

void LiveActor::initScoreKeeper() {
    mScoreKeeper = new ActorScoreKeeper();
}

void LiveActor::initStageSwitchKeeper() {
    mStageSwitchKeeper = new StageSwitchKeeper();
}

void LiveActor::initItemKeeper(s32 itemAmount) {
    mItemKeeper = new ActorItemKeeper(this, itemAmount);
}

void LiveActor::initActionKeeper(ActorActionKeeper* actionKeeper) {
    mActionKeeper = actionKeeper;
}

void LiveActor::initScreenPointKeeper(ScreenPointKeeper* screenPointKeeper) {
    mScreenPointKeeper = screenPointKeeper;
}

void LiveActor::initEffectKeeper(EffectKeeper* effectKeeper) {
    mEffectKeeper = effectKeeper;
    if (mModelKeeper)
        alEffectKeeperInitFunction::setupModelToEffectKeeper(mEffectKeeper, mModelKeeper);
    alEffectKeeperInitFunction::setupCameraToEffectKeeper(mEffectKeeper, this);
}

void LiveActor::initNerveKeeper(NerveKeeper* nerveKeeper) {
    mNerveKeeper = nerveKeeper;
}

void LiveActor::initAudioKeeper(AudioKeeper* audioKeeper) {
    mAudioKeeper = audioKeeper;
}

void LiveActor::initActorPrePassLightKeeper(ActorPrePassLightKeeper* lightKeeper) {
    mPrePassLightKeeper = lightKeeper;
}

void LiveActor::initActorOcclusionKeeper(ActorOcclusionKeeper* occlusionKeeper) {
    mOcclusionKeeper = occlusionKeeper;
}

void LiveActor::initSubActorKeeper(SubActorKeeper* subActorKeeper) {
    mSubActorKeeper = subActorKeeper;
}

void LiveActor::initSceneInfo(ActorSceneInfo* sceneInfo) {
    mSceneInfo = sceneInfo;
}

void LiveActor::updateCollider() {
    if (!mPoseKeeper)
        return;
    const sead::Vector3f& velocity = getVelocity(this);
    if (mCollider) {
        if (mFlags->isCollideOff) {
            *getTransPtr(this) += velocity;
            mCollider->onInvalidate();
        } else {
            *getTransPtr(this) += mCollider->collide(velocity);
        }
        return;
    }
    *getTransPtr(this) += velocity;
}

}  // namespace al
