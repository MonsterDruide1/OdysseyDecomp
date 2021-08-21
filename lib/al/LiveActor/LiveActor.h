#pragma once

#include "al/byaml/ByamlIter.h"

#include "al/area/AreaObjDirector.h"
#include "al/audio/AudioKeeper.h"
#include "al/camera/CameraDirector.h"
#include "al/collision/CollisionDirector.h"
#include "al/effect/EffectKeeper.h"
#include "al/hio/HioNode.h"
#include "al/nerve/Nerve.h"
#include "al/rail/RailKeeper.h"
#include "al/rail/RailRider.h"
#include "al/scene/SceneObjHolder.h"
#include "al/screen/ScreenPointKeeper.h"
#include "al/sensor/HitSensorKeeper.h"
#include "al/switch/StageSwitchKeeper.h"

namespace al
{
    class ActorScoreKeeper {
    public:
        struct Entry {
            const char* factorName;
            const char* categoryName;
        };

        ActorScoreKeeper();

        void init(const al::ByamlIter& iter);
        void getCategoryName(); //unknown return type
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

    class LiveActor : public al::IUseNerve, public al::IUseEffectKeeper, public al::IUseAudioKeeper, public al::IUseStageSwitch, public al::IUseSceneObjHolder, public al::IUseAreaObj, public al::IUseCamera, public al::IUseCollision, public al::IUseRail, public al::IUseHioNode
    {
    public:
        LiveActor(const char*); // TODO requires implementation

        NerveKeeper* getNerveKeeper() const override;
        const char* getName() const override;
        EffectKeeper* getEffectKeeper() const override;
        AudioKeeper* getAudioKeeper() const override;
        StageSwitchKeeper* getStageSwitchKeeper() const override;

        virtual void init(const ActorInitInfo&);
        virtual void attackSensor(HitSensor*, HitSensor*);
        virtual bool receiveMsg(const SensorMsg*, HitSensor*, HitSensor*); // NOTE: return type unknown
        virtual bool receiveMsgScreenPoint(const SensorMsg*, ScreenPointer*, ScreenPointTarget*); // NOTE: return type unknown
        virtual void control();
        void initAfterPlacement();

        const char* mActorName;
        al::ActorPoseKeeperBase *mPoseKeeper;
        al::ActorExecuteInfo *mLayoutExecuteInfo;
        al::ActorActionKeeper *mActorActionKeeper;
        al::ActorItemKeeper *mActorItemKeeper;
        al::ActorScoreKeeper *mActorScoreKeeper;
        al::Collider *mCollider;
        al::CollisionParts *mCollisionParts;
        al::ModelKeeper *mModelKeeper;
        al::NerveKeeper *mNerveKeeper;
        al::HitSensorKeeper *mHitSensorKeeper;
        al::ScreenPointKeeper *mScreenPointKeeper;
        al::EffectKeeper *mEffectKeeper;
        al::AudioKeeper *mAudioKeeper;
        //void *gap_4;
        al::StageSwitchKeeper *mStageSwitchKeeper;
        al::RailKeeper *mRailKeeper;
        al::ShadowKeeper *mShadowKeeper;
        al::ActorPrePassLightKeeper *mActorPrePassLightKeeper;
        al::ActorOcclusionKeeper *mActorOcclusionKeeper;
        al::SubActorKeeper *mSubActorKeeper;
        void *gap_6;
        al::ActorSceneInfo *mSceneInfo;
        al::LiveActorFlag *mLiveActorFlag;
    };
};