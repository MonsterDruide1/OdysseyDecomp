#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorSceneInfo.h"

namespace al {
class ByamlIter;
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
class ActorFactory;
class ActorResource;
class ActorResourceHolder;
class AudioDirector;
class EffectSystemInfo;
class ExecuteDirector;
class HitSensorDirector;
class StageSwitchDirector;
class ScreenPointDirector;
class ViewIdHolder;
class SceneCameraInfo;
class Nerve;
class PlacementInfo;
class Resource;
class LayoutInitInfo;
class AudioDirector;
class CollisionDirector;
class GraphicsSystemInfo;
class ShadowMaskBase;
class HitSensor;

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

    const LayoutInitInfo& getLayoutInitInfo() const { return *mLayoutInitInfo; }

    const PlacementInfo& getPlacementInfo() const { return *mPlacementInfo; }

    const ActorSceneInfo& getActorSceneInfo() const { return mActorSceneInfo; }

private:
    LiveActorGroup* mKitDrawingGroup;
    const PlacementInfo* mPlacementInfo;
    const LayoutInitInfo* mLayoutInitInfo;
    ActorSceneInfo mActorSceneInfo;
    LiveActorGroup* mAllActorsGroup;
    const ActorFactory* mActorFactory;
    ActorResourceHolder* mActorResourceHolder;
    AudioDirector* mAudioDirector;
    EffectSystemInfo* mEffectSystemInfo;
    ExecuteDirector* mExecuteDirector;
    HitSensorDirector* mHitSensorDirector;
    ScreenPointDirector* mScreenPointDirector;
    StageSwitchDirector* mStageSwitchDirector;
    ViewIdHolder* mViewIdHolder;
};
}  // namespace al
