#pragma once

#include "Library/LiveActor/ActorSceneInfo.h"

namespace al {
class ActorFactory;
class ActorResourceHolder;
class AreaObjDirector;
class AudioDirector;
class CameraDirector;
class ClippingDirector;
class CollisionDirector;
class DemoDirector;
class EffectSystemInfo;
class ExecuteDirector;
class GameDataHolderBase;
class GamePadSystem;
class GraphicsSystemInfo;
class GravityHolder;
class HitSensorDirector;
class ItemDirectorBase;
class LayoutInitInfo;
class LiveActor;
class LiveActorGroup;
class ModelDrawBufferCounter;
class ModelGroup;
class NatureDirector;
class PadRumbleDirector;
class PlacementInfo;
class PlayerHolder;
class SceneMsgCtrl;
class SceneObjHolder;
class SceneStopCtrl;
class ScreenCoverCtrl;
class ScreenPointDirector;
class ShadowDirector;
class StageSwitchDirector;
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

    const LayoutInitInfo& getLayoutInitInfo() const { return *mLayoutInitInfo; }

    const PlacementInfo& getPlacementInfo() const { return *mPlacementInfo; }

    const ActorSceneInfo& getActorSceneInfo() const { return mActorSceneInfo; }

    ExecuteDirector* getExecuteDirector() const { return mExecuteDirector; }

private:
    LiveActorGroup* mKitDrawingGroup = nullptr;
    const PlacementInfo* mPlacementInfo = nullptr;
    const LayoutInitInfo* mLayoutInitInfo = nullptr;
    ActorSceneInfo mActorSceneInfo = {};
    LiveActorGroup* mAllActorsGroup = nullptr;
    const ActorFactory* mActorFactory = nullptr;
    ActorResourceHolder* mActorResourceHolder = nullptr;
    AudioDirector* mAudioDirector = nullptr;
    EffectSystemInfo* mEffectSystemInfo = nullptr;
    ExecuteDirector* mExecuteDirector = nullptr;
    HitSensorDirector* mHitSensorDirector = nullptr;
    ScreenPointDirector* mScreenPointDirector = nullptr;
    StageSwitchDirector* mStageSwitchDirector = nullptr;
    ViewIdHolder* mViewIdHolder = nullptr;
};
}  // namespace al
