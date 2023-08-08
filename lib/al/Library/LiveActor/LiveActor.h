#pragma once

#include "al/Library/Yaml/ByamlIter.h"

#include "al/Library/HostIO/HioNode.h"
#include "al/Library/Area/IUseAreaObj.h"
#include "al/Library/Audio/IUseAudioKeeper.h"
#include "al/Library/Camera/IUseCamera.h"
#include "al/Library/Collision/IUseCollision.h"
#include "al/Library/Effect/IUseEffectKeeper.h"
#include "al/Library/Nerve/Nerve.h"
#include "al/Library/Rail/RailKeeper.h"
#include "al/Library/Rail/RailRider.h"
#include "al/Library/Scene/SceneObjHolder.h"
#include "al/Library/Screen/ScreenPointKeeper.h"
#include "al/Library/HitSensor/HitSensorKeeper.h"
#include "al/Library/Stage/StageSwitchKeeper.h"

namespace al {
class ActorScoreKeeper {
public:
    struct Entry {
        const char* factorName;
        const char* categoryName;
    };

    ActorScoreKeeper();

    void init(const al::ByamlIter& iter);
    void getCategoryName();  // unknown return type
    const char* tryGetCategoryName(const char* a1);

private:
    inline void allocArray();
    inline void putEntry(int index, const al::ByamlIter& iter);

    Entry* array;
    int size;
};

class ActorPoseKeeperBase;
class ActorExecuteInfo;
class ActorActionKeeper;
class ActorItemKeeper;
class ActorScoreKeeper;
class Collider;
class CollisionParts;
class ModelKeeper;
class ShadowKeeper;
class ActorPrePassLightKeeper;
class ActorOcclusionKeeper;
class SubActorKeeper;
class ActorSceneInfo;
class LiveActorFlag;

class ActorInitInfo;
class HitSensor;
class SensorMsg;
class ScreenPointer;
class ScreenPointTarget;

class LiveActor : public al::IUseNerve,
                  public al::IUseEffectKeeper,
                  public al::IUseAudioKeeper,
                  public al::IUseStageSwitch,
                  public al::IUseSceneObjHolder,
                  public al::IUseAreaObj,
                  public al::IUseCamera,
                  public al::IUseCollision,
                  public al::IUseRail,
                  public al::IUseHioNode {
public:
    LiveActor(const char* name);  // TODO requires implementation

    virtual NerveKeeper* getNerveKeeper() const override;
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
    virtual bool receiveMsg(const SensorMsg* message, HitSensor* source,
                            HitSensor* target);  // NOTE: return type unknown
    virtual bool receiveMsgScreenPoint(const SensorMsg*, ScreenPointer*,
                                       ScreenPointTarget*);  // NOTE: return type unknown
    virtual const char* getName() const override;
    virtual void* getBaseMtx() const;  // NOTE: return type unknown
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
    void initHitSensor(int);
    void initScreenPointKeeper(ScreenPointKeeper*);
    void initEffectKeeper(EffectKeeper*);
    void initAudioKeeper(AudioKeeper*);
    void initRailKeeper(const ActorInitInfo&, const char*);
    void initCollider(float, float, unsigned int);
    void initItemKeeper(int);
    void initScoreKeeper();
    void initActorPrePassLightKeeper(ActorPrePassLightKeeper*);
    void initActorOcclusionKeeper(ActorOcclusionKeeper*);
    void initSubActorKeeper(SubActorKeeper*);
    void initSceneInfo(ActorSceneInfo*);

    const char* mActorName;
    al::ActorPoseKeeperBase* mPoseKeeper;
    al::ActorExecuteInfo* mLayoutExecuteInfo;
    al::ActorActionKeeper* mActorActionKeeper;
    al::ActorItemKeeper* mActorItemKeeper;
    al::ActorScoreKeeper* mActorScoreKeeper;
    al::Collider* mCollider;
    al::CollisionParts* mCollisionParts;
    al::ModelKeeper* mModelKeeper;
    al::NerveKeeper* mNerveKeeper;
    al::HitSensorKeeper* mHitSensorKeeper;
    al::ScreenPointKeeper* mScreenPointKeeper;
    al::EffectKeeper* mEffectKeeper;
    al::AudioKeeper* mAudioKeeper;
    void* gap_4;
    al::StageSwitchKeeper* mStageSwitchKeeper;
    al::RailKeeper* mRailKeeper;
    al::ShadowKeeper* mShadowKeeper;
    al::ActorPrePassLightKeeper* mActorPrePassLightKeeper;
    al::ActorOcclusionKeeper* mActorOcclusionKeeper;
    al::SubActorKeeper* mSubActorKeeper;
    void* gap_6;
    al::ActorSceneInfo* mSceneInfo;
    al::LiveActorFlag* mLiveActorFlag;
};
};  // namespace al
