#pragma once

#include "al/actor/ActorInitInfo.h"
#include "al/area/AreaObjDirector.h"
#include "al/audio/AudioKeeper.h"
#include "al/camera/CameraDirector.h"
#include "al/collision/CollisionDirector.h"
#include "al/effect/EffectKeeper.h"
#include "al/hio/HioNode.h"
#include "al/nerve/Nerve.h"
#include "al/pose/ActorPoseKeeper.h"
#include "al/rail/RailKeeper.h"
#include "al/rail/RailRider.h"
#include "al/scene/SceneObjHolder.h"
#include "al/screen/ScreenPointKeeper.h"
#include "al/sensor/HitSensorKeeper.h"
#include "al/switch/StageSwitchKeeper.h"

namespace al
{
    class LiveActor : public al::IUseNerve, public al::IUseEffectKeeper, public al::IUseAudioKeeper, public al::IUseStageSwitch, public al::IUseSceneObjHolder, public al::IUseAreaObj, public al::IUseCamera, public al::IUseCollision, public al::IUseRail, public al::IUseHioNode
    {
    public:
        LiveActor(const char *);

        virtual al::NerveKeeper* getNerveKeeper() const;

        const char* mActorName; // _48
        al::ActorPoseKeeperBase* mPoseKeeper; // _50
        char _58[0x90-0x58];
        al::NerveKeeper* mNerveKeeper; // _90
        al::HitSensorKeeper* mHitSensorKeeper; // _98
        al::ScreenPointKeeper* mScreenPointKeeper; // _A0
        al::EffectKeeper* mEffectKeeper; // _A8
        al::AudioKeeper* mAudioKeeper; // _B0

        al::StageSwitchKeeper* mStageSwitchKeeper; // _C0
        al::RailKeeper* mRailKeeper; // _C8
    };
};