#include "al/LiveActor/LiveActor.h"

#include <string.h>
#include "al/actor/ActorPrePassLightKeeper.h"
#include "al/LiveActor/ShadowKeeper.h"
#include "al/LiveActor/LiveActorFlag.h"
#include "al/util/LiveActorUtil.h"

namespace al
{

    LiveActor::LiveActor(const char* actorName) : mActorName(actorName) {
        memset(&mPoseKeeper, 0, 0xB8);
        mLiveActorFlag = new LiveActorFlag();
        mShadowKeeper = new ShadowKeeper();
    }

    al::NerveKeeper* LiveActor::getNerveKeeper() const {
        return mNerveKeeper;
    }

    const char* LiveActor::getName() const {
        return mActorName;
    }
    al::EffectKeeper* LiveActor::getEffectKeeper() const {
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
    bool LiveActor::receiveMsg(const SensorMsg*, HitSensor*, HitSensor*) {return 0;}
    bool LiveActor::receiveMsgScreenPoint(const SensorMsg*, ScreenPointer*, ScreenPointTarget*) {return 0;}
    void LiveActor::control() {}
    void LiveActor::draw() const {}

    /*void LiveActor::initAfterPlacement(){
        tryInitFixedModelGpuBuffer(this);
        if(mActorPrePassLightKeeper)
            mActorPrePassLightKeeper->initAfterPlacement();
    }*/

    void LiveActor::appear() {
        makeActorAlive();
    }
    void LiveActor::kill() {
        makeActorDead();
    }
    RailRider* LiveActor::getRailRider() const {
        if(mRailKeeper)
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
        mLayoutExecuteInfo = executeInfo;
    }
    void LiveActor::initModelKeeper(ModelKeeper* modelKeeper){
        mModelKeeper = modelKeeper;
        offUpdateMovementEffectAudioCollisionSensor(this);
    }
    void LiveActor::initActionKeeper(ActorActionKeeper* actionKeeper){
        mActorActionKeeper = actionKeeper;
    }
    void LiveActor::initNerveKeeper(NerveKeeper* nerveKeeper){
        mNerveKeeper = nerveKeeper;
    }
    void LiveActor::initAudioKeeper(AudioKeeper* audioKeeper){
        mAudioKeeper = audioKeeper;
    }
    void LiveActor::initActorPrePassLightKeeper(ActorPrePassLightKeeper* lightKeeper){
        mActorPrePassLightKeeper = lightKeeper;
    }
    void LiveActor::initActorOcclusionKeeper(ActorOcclusionKeeper* occlusionKeeper){
        mActorOcclusionKeeper = occlusionKeeper;
    }
    void LiveActor::initSubActorKeeper(SubActorKeeper* subActorKeeper){
        mSubActorKeeper = subActorKeeper;
    }
    void LiveActor::initSceneInfo(ActorSceneInfo* sceneInfo){
        mSceneInfo = sceneInfo;
    }
    


    ActorScoreKeeper::ActorScoreKeeper() = default;

    void ActorScoreKeeper::init(const al::ByamlIter& iter){
        if(iter.isTypeArray()){
            size = iter.getSize();
            allocArray();
            for(int i=0;i<size;i++){
                al::ByamlIter subIter;
                iter.tryGetIterByIndex(&subIter, i);
                putEntry(i, subIter);
            }
        }
        else {
            size = 1;
            allocArray();
            putEntry(0, iter);
        }
    }

    inline void ActorScoreKeeper::allocArray(){
        Entry* local_array = new Entry[size];
        if(size)
            memset(local_array, 0, sizeof(Entry)*size);
        array = local_array;
    }

    inline void ActorScoreKeeper::putEntry(int index, const al::ByamlIter& iter){
        auto& entry = array[index];
        iter.tryGetStringByKey(&entry.factorName, "FactorName");
        iter.tryGetStringByKey(&entry.categoryName, "CategoryName");
    }

};