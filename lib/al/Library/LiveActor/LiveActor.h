#pragma once

#include <math/seadMatrix.h>

#include "Library/Area/IUseAreaObj.h"
#include "Library/Audio/IUseAudioKeeper.h"
#include "Library/Camera/IUseCamera.h"
#include "Library/Collision/IUseCollision.h"
#include "Library/Effect/IUseEffectKeeper.h"
#include "Library/HostIO/HioNode.h"
#include "Library/Nature/NatureDirector.h"
#include "Library/Nerve/IUseNerve.h"
#include "Library/Rail/IUseRail.h"
#include "Library/Scene/IUseSceneObjHolder.h"
#include "Library/Stage/IUseStageSwitch.h"

namespace al {
class ActorPoseKeeperBase;
class ActorExecuteInfo;
class ActorActionKeeper;
class ActorItemKeeper;
class ActorScoreKeeper;
class Collider;
class CollisionParts;
class ModelKeeper;
class NerveKeeper;
class HitSensorKeeper;
class ScreenPointKeeper;
class EffectKeeper;
class AudioKeeper;
class HitReactionKeeper;
class StageSwitchKeeper;
class RailKeeper;
class ShadowKeeper;
class ActorPrePassLightKeeper;
class ActorOcclusionKeeper;
class SubActorKeeper;
class ActorParamHolder;
struct ActorSceneInfo;
struct LiveActorFlag;
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
class ScreenPointer;
class ScreenPointTarget;

class LiveActor : public IUseNerve,
                  public IUseEffectKeeper,
                  public IUseAudioKeeper,
                  public IUseStageSwitch,
                  public IUseSceneObjHolder,
                  public IUseAreaObj,
                  public IUseCamera,
                  public IUseCollision,
                  public IUseRail,
                  public IUseHioNode {
public:
    LiveActor(const char* actorName);

    NerveKeeper* getNerveKeeper() const override { return mNerveKeeper; }

    virtual void init(const ActorInitInfo& info) {}

    virtual void initAfterPlacement();
    virtual void appear();
    virtual void makeActorAlive();
    virtual void kill();
    virtual void makeActorDead();
    virtual void movement();
    virtual void calcAnim();
    virtual void draw() const;
    virtual void startClipped();
    virtual void endClipped();

    virtual void attackSensor(HitSensor* self, HitSensor* other) {}

    virtual bool receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) {
        return false;
    }

    virtual bool receiveMsgScreenPoint(const SensorMsg* message, ScreenPointer* source,
                                       ScreenPointTarget* target) {
        return false;
    }

    virtual const char* getName() const override { return mName; }

    virtual const sead::Matrix34f* getBaseMtx() const;

    virtual EffectKeeper* getEffectKeeper() const override { return mEffectKeeper; }

    virtual AudioKeeper* getAudioKeeper() const override { return mAudioKeeper; }

    virtual StageSwitchKeeper* getStageSwitchKeeper() const override { return mStageSwitchKeeper; }

    RailRider* getRailRider() const override;
    SceneObjHolder* getSceneObjHolder() const override;
    CollisionDirector* getCollisionDirector() const override;
    AreaObjDirector* getAreaObjDirector() const override;
    CameraDirector* getCameraDirector() const override;
    NatureDirector* getNatureDirector() const;
    void initStageSwitchKeeper() override;

    virtual void control() {}

    virtual void updateCollider();

    ActorSceneInfo* getSceneInfo() const;
    void initPoseKeeper(ActorPoseKeeperBase* poseKeeper);
    void initExecuteInfo(ActorExecuteInfo* executeInfo);
    void initModelKeeper(ModelKeeper* modelKeeper);
    void initActionKeeper(ActorActionKeeper* actionKeeper);
    void initNerveKeeper(NerveKeeper* nerveKeeper);
    void initHitSensor(s32 amount);
    void initScreenPointKeeper(ScreenPointKeeper* screenPointKeeper);
    void initEffectKeeper(EffectKeeper* effectKeeper);
    void initAudioKeeper(AudioKeeper* audioKeeper);
    void initRailKeeper(const ActorInitInfo& info, const char* linkName);
    void initCollider(f32 radius, f32 offsetY, u32 allocatedHitInfo);
    void initItemKeeper(s32 itemAmount);
    void initScoreKeeper();
    void initActorPrePassLightKeeper(ActorPrePassLightKeeper* lightKeeper);
    void initActorOcclusionKeeper(ActorOcclusionKeeper* occlusionKeeper);
    void initSubActorKeeper(SubActorKeeper* subActorKeeper);
    void initSceneInfo(ActorSceneInfo* sceneInfo);

    LiveActorFlag* getFlags() const { return mFlags; }

    ModelKeeper* getModelKeeper() const { return mModelKeeper; }

    ActorPoseKeeperBase* getPoseKeeper() const { return mPoseKeeper; }

    ActorExecuteInfo* getExecuteInfo() const { return mExecuteInfo; }

    ActorActionKeeper* getActorActionKeeper() const { return mActionKeeper; }

    ActorItemKeeper* getActorItemKeeper() const { return mItemKeeper; }

    ActorScoreKeeper* getActorScoreKeeper() const { return mScoreKeeper; }

    Collider* getCollider() const { return mCollider; }

    CollisionParts* getCollisionParts() const { return mCollisionParts; }

    HitSensorKeeper* getHitSensorKeeper() const { return mHitSensorKeeper; }

    ScreenPointKeeper* getScreenPointKeeper() const { return mScreenPointKeeper; }

    HitReactionKeeper* getHitReactionKeeper() const { return mHitReactionKeeper; }

    RailKeeper* getRailKeeper() const { return mRailKeeper; }

    ShadowKeeper* getShadowKeeper() const { return mShadowKeeper; }

    ActorPrePassLightKeeper* getActorPrePassLightKeeper() const { return mPrePassLightKeeper; }

    ActorOcclusionKeeper* getActorOcclusionKeeper() const { return mOcclusionKeeper; }

    SubActorKeeper* getSubActorKeeper() const { return mSubActorKeeper; }

    ActorParamHolder* getActorParamHolder() const { return mParamHolder; }

    void setName(const char* newName) { mName = newName; }

protected:
    friend class alActorFunction;

private:
    const char* mName = nullptr;
    ActorPoseKeeperBase* mPoseKeeper = nullptr;
    ActorExecuteInfo* mExecuteInfo = nullptr;
    ActorActionKeeper* mActionKeeper = nullptr;
    ActorItemKeeper* mItemKeeper = nullptr;
    ActorScoreKeeper* mScoreKeeper = nullptr;
    Collider* mCollider = nullptr;
    CollisionParts* mCollisionParts = nullptr;
    ModelKeeper* mModelKeeper = nullptr;
    NerveKeeper* mNerveKeeper = nullptr;
    HitSensorKeeper* mHitSensorKeeper = nullptr;
    ScreenPointKeeper* mScreenPointKeeper = nullptr;
    EffectKeeper* mEffectKeeper = nullptr;
    AudioKeeper* mAudioKeeper = nullptr;
    HitReactionKeeper* mHitReactionKeeper = nullptr;
    StageSwitchKeeper* mStageSwitchKeeper = nullptr;
    RailKeeper* mRailKeeper = nullptr;
    ShadowKeeper* mShadowKeeper = nullptr;
    ActorPrePassLightKeeper* mPrePassLightKeeper = nullptr;
    ActorOcclusionKeeper* mOcclusionKeeper = nullptr;
    SubActorKeeper* mSubActorKeeper = nullptr;
    ActorParamHolder* mParamHolder = nullptr;
    ActorSceneInfo* mSceneInfo = nullptr;
    LiveActorFlag* mFlags = nullptr;
};
}  // namespace al
