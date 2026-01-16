#pragma once

#include <math/seadBoundBox.h>
#include <prim/seadSafeString.h>

namespace agl {
class DrawContext;
}

namespace alNerveFunction {
class NerveActionCollector;
}

namespace al {
struct ActorInitInfo;
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
LiveActor* createChildLinkSimpleActor(const char* actorName, const char* linkName,
                                      const ActorInitInfo& initInfo, bool alive);
LiveActor* createChildLinkMapPartsActor(const char* actorName, const char* linkName,
                                        const ActorInitInfo& initInfo, s32 linkIndex, bool alive);
void initMapPartsActor(LiveActor* actor, const ActorInitInfo& initInfo, const char* suffix);
void initLinksActor(LiveActor* actor, const ActorInitInfo& initInfo, const char* suffix,
                    s32 linkIndex);
ActorInitInfo* createLinksPlayerActorInfo(LiveActor* actor, const ActorInitInfo& initInfo);
const char* getLinksActorClassName(const ActorInitInfo& initInfo, const char* linkName,
                                   s32 linkIndex);
const char* getLinksActorDisplayName(const ActorInitInfo& initInfo, const char* linkName,
                                     s32 linkIndex);
const char* getLinksActorObjectName(const ActorInitInfo& initInfo, const char* linkName,
                                    s32 linkIndex);
void initCreateActorWithPlacementInfo(LiveActor* actor, const ActorInitInfo& initInfo);
void initCreateActorWithPlacementInfo(LiveActor* actor, const ActorInitInfo& initInfo,
                                      const PlacementInfo& placementInfo);
void initCreateActorNoPlacementInfo(LiveActor* actor, const ActorInitInfo& initInfo);
void initCreateActorNoPlacementInfoNoViewId(LiveActor* actor, const ActorInitInfo& initInfo);
LiveActor* createPlacementActorFromFactory(const ActorInitInfo& initInfo,
                                           const PlacementInfo* placementInfo);
LiveActor* createLinksActorFromFactory(const ActorInitInfo& initInfo, const char* linkName,
                                       s32 linkNum);
LiveActorGroup* createLinksActorGroupFromFactory(const ActorInitInfo& initInfo,
                                                 const char* linkName, const char* groupName);
LiveActorGroup* tryCreateLinksActorGroupFromFactory(const ActorInitInfo& initInfo,
                                                    const char* linkName, const char* groupName);
LiveActor* tryCreateLinksActorFromFactorySingle(const ActorInitInfo& initInfo,
                                                const char* linkName);
void createAndRegisterLinksActorFromFactory(LiveActorGroup* group, const ActorInitInfo& initInfo,
                                            const char* linkName);
void makeMapPartsModelName(sead::BufferedSafeString* modelName, sead::BufferedSafeString* path,
                           const PlacementInfo& placementInfo);
void makeMapPartsModelName(sead::BufferedSafeString* modelName, sead::BufferedSafeString* path,
                           const ActorInitInfo& initInfo);
const char* tryGetMapPartsSuffix(const ActorInitInfo& initInfo, const char* suffix);
void initMapPartsActorWithArchiveName(LiveActor* actor, const ActorInitInfo& initInfo,
                                      const char* fileName, const char* suffix);
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
void setMaterialCode(LiveActor* actor, const char* materialCode);
void initMaterialCode(LiveActor* actor, const ActorInitInfo& initInfo);
bool tryAddDisplayRotate(LiveActor* actor, const ActorInitInfo& initInfo);
bool tryAddDisplayOffset(LiveActor* actor, const ActorInitInfo& initInfo);
bool tryAddDisplayScale(LiveActor* actor, const ActorInitInfo& initInfo);
const PlacementInfo& getPlacementInfo(const ActorInitInfo& initInfo);
const LayoutInitInfo& getLayoutInitInfo(const ActorInitInfo& initInfo);
AudioDirector* getAudioDirector(const ActorInitInfo& initInfo);
CollisionDirector* getCollisionDirectorFromInfo(const ActorInitInfo& initInfo);
const SceneCameraInfo* getSceneCameraInfoFromInfo(const ActorInitInfo& initInfo);
GraphicsSystemInfo* getGraphicsSystemInfo(const ActorInitInfo& initInfo);
agl::DrawContext* getDrawContext(const ActorInitInfo& initInfo);
void getActorRecourseDataF32(f32* val, LiveActor* actor, const char* fileName,
                             const char* entryName);
void getActorRecourseDataString(const char** val, LiveActor* actor, const char* fileName,
                                const char* entryName);
void getActorRecourseDataV3f(sead::Vector3f* val, LiveActor* actor, const char* fileName,
                             const char* entryName);
void getActorRecourseDataBox3f(sead::BoundBox3f* box, LiveActor* actor, const char* fileName,
                               const char* entryName);
PartsEffectGroup* createPartsEffectGroup(LiveActor* actor, const ActorInitInfo& initInfo,
                                         s32 maxEffects);

}  // namespace al
