#include "Library/LiveActor/LiveActor.h"

#include "Library/Collision/Collider.h"
#include "Library/HitSensor/HitSensorKeeper.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSceneInfo.h"
#include "Library/LiveActor/LiveActorFlag.h"
#include "Library/LiveActor/LiveActorUtil.h"
#include "Library/Rail/RailKeeper.h"
#include "Library/Shadow/ShadowKeeper.h"
#include "Library/Stage/StageSwitchKeeper.h"
#include "Project/Item/ActorItemKeeper.h"
#include "Project/Item/ActorScoreKeeper.h"
#include "Project/Light/ActorPrepassLightKeeper.h"

namespace al {

LiveActor::LiveActor(const char* actorName) : mActorName(actorName) {
    mFlags = new LiveActorFlag();
    mShadowKeeper = new ShadowKeeper();
}

NerveKeeper* LiveActor::getNerveKeeper() const {
    return mNerveKeeper;
}

const char* LiveActor::getName() const {
    return mActorName;
}

CollisionDirector* LiveActor::getCollisionDirector() const {
    return mSceneInfo->mCollisionDirector;
}

AreaObjDirector* LiveActor::getAreaObjDirector() const {
    return mSceneInfo->mAreaObjDirector;
}

CameraDirector* LiveActor::getCameraDirector() const {
    return mSceneInfo->mCameraDirector;
}

SceneObjHolder* LiveActor::getSceneObjHolder() const {
    return mSceneInfo->mSceneObjHolder;
}

EffectKeeper* LiveActor::getEffectKeeper() const {
    return mEffectKeeper;
}

AudioKeeper* LiveActor::getAudioKeeper() const {
    return mAudioKeeper;
}

StageSwitchKeeper* LiveActor::getStageSwitchKeeper() const {
    return mStageSwitchKeeper;
}

void LiveActor::init(const ActorInitInfo& info) {}

void LiveActor::attackSensor(HitSensor* self, HitSensor* other) {}

bool LiveActor::receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) {
    return false;
}

bool LiveActor::receiveMsgScreenPoint(const SensorMsg* message, ScreenPointer* source,
                                      ScreenPointTarget* target) {
    return false;
}

void LiveActor::control() {}

void LiveActor::draw() const {}

void LiveActor::initAfterPlacement() {
    tryInitFixedModelGpuBuffer(this);
    if (mActorPrePassLightKeeper)
        mActorPrePassLightKeeper->initAfterPlacement();
}

void LiveActor::appear() {
    makeActorAlive();
}

void LiveActor::kill() {
    makeActorDead();
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
    mRailKeeper = tryCreateRailKeeper(info.getPlacementInfo(), linkName);
}

void LiveActor::initModelKeeper(ModelKeeper* modelKeeper) {
    mModelKeeper = modelKeeper;
    offUpdateMovementEffectAudioCollisionSensor(this);
}

void LiveActor::initScoreKeeper() {
    mActorScoreKeeper = new ActorScoreKeeper();
}

void LiveActor::initStageSwitchKeeper() {
    mStageSwitchKeeper = new StageSwitchKeeper();
}

void LiveActor::initItemKeeper(s32 itemAmount) {
    mActorItemKeeper = new ActorItemKeeper(this, itemAmount);
}

void LiveActor::initActionKeeper(ActorActionKeeper* actionKeeper) {
    mActorActionKeeper = actionKeeper;
}

void LiveActor::initScreenPointKeeper(ScreenPointKeeper* screenPointKeeper) {
    mScreenPointKeeper = screenPointKeeper;
}

void LiveActor::initNerveKeeper(NerveKeeper* nerveKeeper) {
    mNerveKeeper = nerveKeeper;
}

void LiveActor::initAudioKeeper(AudioKeeper* audioKeeper) {
    mAudioKeeper = audioKeeper;
}

void LiveActor::initActorPrePassLightKeeper(ActorPrePassLightKeeper* lightKeeper) {
    mActorPrePassLightKeeper = lightKeeper;
}

void LiveActor::initActorOcclusionKeeper(ActorOcclusionKeeper* occlusionKeeper) {
    mActorOcclusionKeeper = occlusionKeeper;
}

void LiveActor::initSubActorKeeper(SubActorKeeper* subActorKeeper) {
    mSubActorKeeper = subActorKeeper;
}

void LiveActor::initSceneInfo(ActorSceneInfo* sceneInfo) {
    mSceneInfo = sceneInfo;
}

}  // namespace al
