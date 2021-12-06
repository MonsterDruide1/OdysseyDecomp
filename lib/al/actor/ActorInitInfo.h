#pragma once

#include "al/actor/Placement.h"

namespace al {
class LiveActor;
class LiveActorGroup;
class PlacementInfo;
class LayoutInitInfo;
class AreaObjDirector;
class CameraDirector;
class ClippingDirector;
class CollisionDirector;
class DemoDirector;
class GameDataHolderBase;
class GravityHolder;
class ItemDirectorBase;
class NatureDirector;
class GamePadSystem;
class PadRumbleDirector;
class PlayerHolder;
class SceneObjHolder;
class SceneStopCtrl;
class SceneMsgCtrl;
class ScreenCoverCtrl;
class ShadowDirector;
class ModelGroup;
class GraphicsSystemInfo;
class ModelDrawBufferCounter;
class LiveActorGroup;
class ActorFactory;
class ActorResourceHolder;
class AudioDirector;
class EffectSystemInfo;
class ExecuteDirector;
class HitSensorDirector;
class StageSwitchDirector;
class ScreenPointDirector;
class ViewIdHolder;

class ActorInitInfo {
public:
    ActorInitInfo();
    void initNew(const PlacementInfo*, const LayoutInitInfo*, LiveActorGroup*, const ActorFactory*,
                 ActorResourceHolder*, AreaObjDirector*, AudioDirector*, CameraDirector*,
                 ClippingDirector*, CollisionDirector*, DemoDirector*, EffectSystemInfo*,
                 ExecuteDirector*, GameDataHolderBase*, GravityHolder*, HitSensorDirector*,
                 ItemDirectorBase*, NatureDirector*, const GamePadSystem*, PadRumbleDirector*,
                 PlayerHolder*, SceneObjHolder*, SceneMsgCtrl*, SceneStopCtrl*, ScreenCoverCtrl*,
                 ScreenPointDirector*, ShadowDirector*, StageSwitchDirector*, ModelGroup*,
                 GraphicsSystemInfo*, ModelDrawBufferCounter*, LiveActorGroup*);
    void initViewIdSelf(const PlacementInfo*, const ActorInitInfo&);
    void copyHostInfo(const ActorInitInfo&, const PlacementInfo*);
    void initViewIdHost(const PlacementInfo*, const ActorInitInfo&);
    void initViewIdHostActor(const ActorInitInfo&, const LiveActor*);
    void initNoViewId(const PlacementInfo*, const ActorInitInfo&);

    const al::PlacementInfo& getPlacementInfo() const { return *mPlacementInfo; }

private:
    al::LiveActorGroup* mLiveActorGroup1;
    const al::PlacementInfo* mPlacementInfo;
    const al::LayoutInitInfo* mLayoutInitInfo;
    al::AreaObjDirector* mAreaObjDirector;
    al::CameraDirector* mCameraDirector;
    al::ClippingDirector* mClippingDirector;
    al::CollisionDirector* mCollisionDirector;
    al::DemoDirector* mDemoDirector;
    al::GameDataHolderBase* mGameDataHolderBase;
    al::GravityHolder* mGravityHolder;
    al::ItemDirectorBase* mItemDirectorBase;
    al::NatureDirector* mNatureDirector;
    const al::GamePadSystem* mGamePadSystem;
    al::PadRumbleDirector* mPadRumbleDirector;
    al::PlayerHolder* mPlayerHolder;
    al::SceneObjHolder* mSceneObjHolder;
    al::SceneStopCtrl* mSceneStopCtrl;
    al::SceneMsgCtrl* mSceneMsgCtrl;
    al::ScreenCoverCtrl* mScreenCoverCtrl;
    al::ShadowDirector* mShadowDirector;
    al::ModelGroup* mModelGroup;
    al::GraphicsSystemInfo* mGraphicsSystemInfo;
    al::ModelDrawBufferCounter* mModelDrawBufferCounter;
    al::LiveActorGroup* mLiveActorGroup2;
    const al::ActorFactory* mActorFactory;
    al::ActorResourceHolder* mActorResourceHolder;
    al::AudioDirector* mAudioDirector;
    al::EffectSystemInfo* mEffectSystemInfo;
    al::ExecuteDirector* mExecuteDirector;
    al::HitSensorDirector* mHitSensorDirector;
    al::StageSwitchDirector* mStageSwitchDirector;
    al::ScreenPointDirector* mScreenPointDirector;
    al::ViewIdHolder* mViewIdHolder;
};
}  // namespace al