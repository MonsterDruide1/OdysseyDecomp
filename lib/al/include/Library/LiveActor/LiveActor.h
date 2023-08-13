#pragma once

#include <math/seadMatrix.h>
#include "Library/Area/IUseAreaObj.h"
#include "Library/Audio/IUseAudioKeeper.h"
#include "Library/Camera/IUseCamera.h"
#include "Library/Collision/IUseCollision.h"
#include "Library/Effect/IUseEffectKeeper.h"
#include "Library/HostIO/HioNode.h"
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
class ActorInitInfo;
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
    friend class alActorFunction;

public:
    LiveActor(const char* actorName);

    NerveKeeper* getNerveKeeper() const override;
    virtual void init(const ActorInitInfo& info);
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
    virtual void attackSensor(HitSensor* target, HitSensor* source);
    virtual bool receiveMsg(const SensorMsg* message, HitSensor* source, HitSensor* target);
    virtual bool receiveMsgScreenPoint(const SensorMsg*, ScreenPointer*, ScreenPointTarget*);
    virtual const char* getName() const override;
    virtual sead::Matrix34f* getBaseMtx() const;
    virtual EffectKeeper* getEffectKeeper() const override;
    virtual AudioKeeper* getAudioKeeper() const override;
    virtual StageSwitchKeeper* getStageSwitchKeeper() const override;
    virtual RailRider* getRailRider() const override;
    virtual SceneObjHolder* getSceneObjHolder() const override;
    virtual CollisionDirector* getCollisionDirector() const override;
    virtual AreaObjDirector* getAreaObjDirector() const override;
    virtual CameraDirector* getCameraDirector() const override;
    virtual void initStageSwitchKeeper() override;
    virtual void control();
    virtual void updateCollider();

    ActorSceneInfo* getSceneInfo() const;
    void initPoseKeeper(ActorPoseKeeperBase*);
    void initExecuteInfo(ActorExecuteInfo*);
    void initModelKeeper(ModelKeeper*);
    void initActionKeeper(ActorActionKeeper*);
    void initNerveKeeper(NerveKeeper*);
    void initHitSensor(s32);
    void initScreenPointKeeper(ScreenPointKeeper*);
    void initEffectKeeper(EffectKeeper*);
    void initAudioKeeper(AudioKeeper*);
    void initRailKeeper(const ActorInitInfo&, const char*);
    void initCollider(f32, f32, u32);
    void initItemKeeper(s32);
    void initScoreKeeper();
    void initActorPrePassLightKeeper(ActorPrePassLightKeeper*);
    void initActorOcclusionKeeper(ActorOcclusionKeeper*);
    void initSubActorKeeper(SubActorKeeper*);
    void initSceneInfo(ActorSceneInfo*);

    LiveActorFlag* getFlags() const { return mFlags; }
    ModelKeeper* getModelKeeper() const { return mModelKeeper; }
    ActorPoseKeeperBase* getPoseKeeper() const { return mPoseKeeper; }
    ActorExecuteInfo* getExecuteInfo() const { return mExecuteInfo; }
    ActorActionKeeper* getActorActionKeeper() const { return mActorActionKeeper; }
    ActorItemKeeper* getActorItemKeeper() const { return mActorItemKeeper; }
    ActorScoreKeeper* getActorScoreKeeper() const { return mActorScoreKeeper; }
    Collider* getCollider() const { return mCollider; }
    CollisionParts* getCollisionParts() const { return mCollisionParts; }
    HitSensorKeeper* getHitSensorKeeper() const { return mHitSensorKeeper; }
    ScreenPointKeeper* getScreenPointKeeper() const { return mScreenPointKeeper; }
    HitReactionKeeper* getHitReactionKeeper() const { return mHitReactionKeeper; }
    RailKeeper* getRailKeeper() const { return mRailKeeper; }
    ShadowKeeper* getShadowKeeper() const { return mShadowKeeper; }
    ActorPrePassLightKeeper* getActorPrePassLightKeeper() const { return mActorPrePassLightKeeper; }
    ActorOcclusionKeeper* getActorOcclusionKeeper() const { return mActorOcclusionKeeper; }
    SubActorKeeper* getSubActorKeeper() const { return mSubActorKeeper; }
    ActorParamHolder* getActorParamHolder() const { return mActorParamHolder; }

    void setName(const char* newName) { mActorName = newName; }

private:
    const char* mActorName;
    ActorPoseKeeperBase* mPoseKeeper;
    ActorExecuteInfo* mExecuteInfo;
    ActorActionKeeper* mActorActionKeeper;
    ActorItemKeeper* mActorItemKeeper;
    ActorScoreKeeper* mActorScoreKeeper;
    Collider* mCollider;
    CollisionParts* mCollisionParts;
    ModelKeeper* mModelKeeper;
    NerveKeeper* mNerveKeeper;
    HitSensorKeeper* mHitSensorKeeper;
    ScreenPointKeeper* mScreenPointKeeper;
    EffectKeeper* mEffectKeeper;
    AudioKeeper* mAudioKeeper;
    HitReactionKeeper* mHitReactionKeeper;
    StageSwitchKeeper* mStageSwitchKeeper;
    RailKeeper* mRailKeeper;
    ShadowKeeper* mShadowKeeper;
    ActorPrePassLightKeeper* mActorPrePassLightKeeper;
    ActorOcclusionKeeper* mActorOcclusionKeeper;
    SubActorKeeper* mSubActorKeeper;
    ActorParamHolder* mActorParamHolder;
    ActorSceneInfo* mSceneInfo;
    LiveActorFlag* mFlags;
};

struct LiveActorFlag {
    bool isDead = true;
    bool isClipped = false;
    bool isClippingInvalid = true;
    bool isDrawClipped = false;
    bool isCalcAnim = false;
    bool isModelVisible = false;
    bool isCollideOff = true;
    bool field_07 = false;
    bool isMaterialCodeValid = false;
    bool isPuddleMaterialValid = false;
    bool isAreaTargetOn = true;
    bool isUpdateOn = true;

    LiveActorFlag();
};
static_assert(sizeof(LiveActorFlag) == 0xC);

}  // namespace al
