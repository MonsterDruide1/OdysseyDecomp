#include "Library/LiveActor/LiveActor.h"

#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/LiveActorFlag.h"
#include "Library/LiveActor/LiveActorUtil.h"
#include "Library/Rail/RailKeeper.h"
#include "Library/Shadow/ShadowKeeper.h"
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

void LiveActor::attackSensor(HitSensor* target, HitSensor* source) {}

bool LiveActor::receiveMsg(const SensorMsg* message, HitSensor* source, HitSensor* target) {
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

void LiveActor::initPoseKeeper(ActorPoseKeeperBase* poseKeeper) {
    mPoseKeeper = poseKeeper;
}

void LiveActor::initExecuteInfo(ActorExecuteInfo* executeInfo) {
    mExecuteInfo = executeInfo;
}

void LiveActor::initModelKeeper(ModelKeeper* modelKeeper) {
    mModelKeeper = modelKeeper;
    offUpdateMovementEffectAudioCollisionSensor(this);
}

void LiveActor::initActionKeeper(ActorActionKeeper* actionKeeper) {
    mActorActionKeeper = actionKeeper;
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
