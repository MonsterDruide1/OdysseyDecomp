#include "Library/LiveActor/LiveActor.h"

#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/LiveActorFlag.h"
#include "Library/LiveActor/LiveActorUtil.h"
#include "Library/Rail/RailKeeper.h"
#include "Library/Shadow/ShadowKeeper.h"
#include "Project/Light/ActorPrepassLightKeeper.h"

#include <cstdio>

namespace al {

#define DEREF_NULL *(volatile int*)0;
#define WARN_UNIMPL printf("Function not implemented: %s (%s:%d)\n", __func__, __FILE__, __LINE__)
#define CRASH {WARN_UNIMPL;DEREF_NULL}

LiveActor::LiveActor(const char* actorName) : mActorName(actorName) {
    memset(&mPoseKeeper, 0, 0xB8);
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

void LiveActor::init(const ActorInitInfo&) {}
void LiveActor::attackSensor(HitSensor*, HitSensor*) {}
bool LiveActor::receiveMsg(const SensorMsg*, HitSensor*, HitSensor*) {
    return 0;
}
bool LiveActor::receiveMsgScreenPoint(const SensorMsg*, ScreenPointer*, ScreenPointTarget*) {
    return 0;
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


LiveActor::~LiveActor() {
    delete mFlags;
    delete mShadowKeeper;
}

void LiveActor::makeActorAlive() { CRASH }
void LiveActor::makeActorDead() { CRASH }
void LiveActor::movement() { CRASH }
void LiveActor::calcAnim() { CRASH }
void LiveActor::startClipped() { CRASH }
void LiveActor::endClipped() { CRASH }
sead::Matrix34f* LiveActor::getBaseMtx() const { CRASH }
SceneObjHolder* LiveActor::getSceneObjHolder() const { CRASH }
CollisionDirector* LiveActor::getCollisionDirector() const { CRASH }
AreaObjDirector* LiveActor::getAreaObjDirector() const { CRASH }
CameraDirector* LiveActor::getCameraDirector() const { CRASH }
void LiveActor::initStageSwitchKeeper() { CRASH }
void LiveActor::updateCollider() { CRASH }



}  // namespace al
