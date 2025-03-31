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

struct ActorInitInfo {
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

    LiveActorGroup* kitDrawingGroup = nullptr;
    const PlacementInfo* placementInfo = nullptr;
    const LayoutInitInfo* layoutInitInfo = nullptr;
    ActorSceneInfo actorSceneInfo = {};
    LiveActorGroup* allActorsGroup = nullptr;
    const ActorFactory* actorFactory = nullptr;
    ActorResourceHolder* actorResourceHolder = nullptr;
    AudioDirector* audioDirector = nullptr;
    EffectSystemInfo* effectSystemInfo = nullptr;
    ExecuteDirector* executeDirector = nullptr;
    HitSensorDirector* hitSensorDirector = nullptr;
    ScreenPointDirector* screenPointDirector = nullptr;
    StageSwitchDirector* stageSwitchDirector = nullptr;
    ViewIdHolder* viewIdHolder = nullptr;
};
}  // namespace al
