#pragma once

#include <math/seadBoundBox.h>
#include <prim/seadSafeString.h>

namespace sead {
class DrawContext;
}

namespace alNerveFunction {
class NerveActionCollector;
}

namespace al {
class ActorInitInfo;
class AudioDirector;
class CollisionDirector;
class GraphicsSystemInfo;
class LayoutInitInfo;
class LiveActor;
class LiveActorGroup;
class Nerve;
class PartsEffectGroup;
class PlacementInfo;
class SceneCameraInfo;

void initActor(LiveActor* actor, const ActorInitInfo& initInfo);
// sub_71008DC12C
void initActorSuffix(LiveActor* actor, const ActorInitInfo& initInfo, const char* suffix);
void initActorChangeModel(LiveActor* actor, const ActorInitInfo& initInfo);
void initActorChangeModelSuffix(LiveActor* actor, const ActorInitInfo& initInfo,
                                const char* suffix);
void initActorWithArchiveName(LiveActor* actor, const ActorInitInfo& initInfo,
                              const sead::SafeString& archiveName, const char* suffix);
void initChildActorWithArchiveNameWithPlacementInfo(LiveActor* actor, const ActorInitInfo& initInfo,
                                                    const sead::SafeString& archiveName,
                                                    const char* suffix);
void initChildActorWithArchiveNameNoPlacementInfo(LiveActor* actor, const ActorInitInfo& initInfo,
                                                  const sead::SafeString& archiveName,
                                                  const char* suffix);
LiveActor* createChildLinkSimpleActor(const char* actorName, const char* archiveName,
                                      const ActorInitInfo& initInfo, bool alive);
LiveActor* createChildLinkMapPartsActor(const char* actorName, const char* archiveName,
                                        const ActorInitInfo& initInfo, s32 linkIndex, bool alive);
void initMapPartsActor(LiveActor* actor, const ActorInitInfo& initInfo, const char* suffix);
void initLinksActor(LiveActor* actor, const ActorInitInfo& initInfo, const char* suffix,
                    s32 linkIndex);
ActorInitInfo* createLinksPlayerActorInfo(LiveActor* actor, const ActorInitInfo& initInfo);
const char* getLinksActorClassName(const ActorInitInfo&, const char*, s32);
const char* getLinksActorDisplayName(const ActorInitInfo&, const char*, s32);
const char* getLinksActorObjectName(const ActorInitInfo&, const char*, s32);
void initCreateActorWithPlacementInfo(LiveActor*, const ActorInitInfo&);
void initCreateActorWithPlacementInfo(LiveActor*, const ActorInitInfo&, const PlacementInfo&);
void initCreateActorNoPlacementInfo(LiveActor*, const ActorInitInfo&);
void initCreateActorNoPlacementInfoNoViewId(LiveActor*, const ActorInitInfo&);
LiveActor* createPlacementActorFromFactory(const ActorInitInfo&, const PlacementInfo*);
LiveActor* createLinksActorFromFactory(const ActorInitInfo& info, const char* linkName,
                                       s32 linkNum);
LiveActorGroup* createLinksActorGroupFromFactory(const ActorInitInfo&, const char*, const char*);
LiveActorGroup* tryCreateLinksActorGroupFromFactory(const ActorInitInfo&, const char*, const char*);
LiveActorGroup* tryCreateLinksActorFromFactorySingle(const ActorInitInfo&, const char*);
void createAndRegisterLinksActorFromFactory(LiveActorGroup*, const ActorInitInfo&, const char*);
void makeMapPartsModelName(sead::BufferedSafeString*, sead::BufferedSafeString*,
                           const PlacementInfo&);
void makeMapPartsModelName(sead::BufferedSafeString*, sead::BufferedSafeString*,
                           const ActorInitInfo&);
const char* tryGetMapPartsSuffix(const ActorInitInfo&, const char*);
void initMapPartsActorWithArchiveName(LiveActor*, const ActorInitInfo&, const char*, const char*);
void initNerve(LiveActor* actor, const Nerve* nerve, s32 maxStates);
void initNerveAction(LiveActor* actor, const char* actionName,
                     alNerveFunction::NerveActionCollector* collector, s32 maxStates);
bool trySyncStageSwitchAppear(LiveActor* actor);
bool trySyncStageSwitchKill(LiveActor* actor);
bool trySyncStageSwitchAppearAndKill(LiveActor* actor);
bool tryListenStageSwitchAppear(LiveActor* actor);
bool tryListenStageSwitchKill(LiveActor* actor);
void syncSensorScaleY(LiveActor* actor);
void syncSensorAndColliderScaleY(LiveActor* actor);
void setMaterialCode(LiveActor* actor, const char*);
void initMaterialCode(LiveActor* actor, const ActorInitInfo& initInfo);
bool tryAddDisplayRotate(LiveActor* actor, const ActorInitInfo& initInfo);
bool tryAddDisplayOffset(LiveActor* actor, const ActorInitInfo& initInfo);
bool tryAddDisplayScale(LiveActor* actor, const ActorInitInfo& initInfo);
const PlacementInfo* getPlacementInfo(const ActorInitInfo& initInfo);
const LayoutInitInfo& getLayoutInitInfo(const ActorInitInfo& initInfo);
AudioDirector* getAudioDirector(const ActorInitInfo& initInfo);
CollisionDirector* getCollisionDirectorFromInfo(const ActorInitInfo& initInfo);
const SceneCameraInfo* getSceneCameraInfoFromInfo(const ActorInitInfo& initInfo);
GraphicsSystemInfo* getGraphicsSystemInfo(const ActorInitInfo& initInfo);
sead::DrawContext* getDrawContext(const ActorInitInfo& initInfo);
void getActorRecourseDataF32(f32*, LiveActor*, const char*, const char*);
void getActorRecourseDataString(const char**, LiveActor*, const char*, const char*);
void getActorRecourseDataV3f(sead::Vector3f*, LiveActor* actor, const char*, const char*);
void getActorRecourseDataBox3f(sead::BoundBox3f* box, LiveActor* actor, const char*, const char*);
PartsEffectGroup* createPartsEffectGroup(LiveActor* actor, const ActorInitInfo& initInfo, s32);
}  // namespace al
