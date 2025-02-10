#pragma once

#include <basis/seadTypes.h>
#include <common/aglRenderBuffer.h>
#include <gfx/seadCamera.h>
#include <heap/seadHeap.h>

namespace al {
class ActorFactory;
class ActorInitInfo;
class LayoutInitInfo;
class GameDataHolderBase;
class SceneMsgCtrl;
class PlacementInfo;
class Scene;
struct SceneInitInfo;
class StageInfo;
class PauseCameraCtrl;
class AudioDirectorInitInfo;
class Projection;
class LiveActor;
class LiveActorGroup;
class AreaObjFactory;
class DemoDirector;
class ItemDirectorBase;
class SwitchOnAreaGroup;
class SwitchKeepOnAreaGroup;
class CameraPoserFactory;
class IUseAudioKeeper;
class AreaObjDirector;
class ExecuteDirector;
class PlayerHolder;
class ItemDirectorBase;
class Resource;

s32 getStageInfoMapNum(const Scene*);
s32 getStageInfoDesignNum(const Scene*);
s32 getStageInfoSoundNum(const Scene*);
StageInfo* getStageInfoMap(const Scene*, s32);
StageInfo* getStageInfoDesign(const Scene*, s32);
StageInfo* getStageInfoSound(const Scene*, s32);
StageInfo* getStageResourceMap(const Scene*, s32);
StageInfo* tryGetStageResourceDesign(const Scene*, s32);
StageInfo* tryGetStageResourceSound(const Scene*, s32);
agl::RenderBuffer* getSceneFrameBufferMain(const Scene*);
agl::DrawContext* getSceneDrawContext(const Scene*);
agl::RenderBuffer* getSceneFrameBufferConsole(const Scene*);
agl::RenderBuffer* getSceneFrameBufferHandheld(const Scene*);
f32 getSceneFrameBufferMainAspect(const Scene*);
bool isChangedGraphicsQualityMode(const Scene*);
AreaObjDirector* getSceneAreaObjDirector(const Scene*);
ExecuteDirector* getSceneExecuteDirector(const Scene*);
PlayerHolder* getScenePlayerHolder(const Scene*);
ItemDirectorBase* getSceneItemDirector(const Scene*);
void initActorInitInfo(ActorInitInfo*, const Scene*, const PlacementInfo*, const LayoutInitInfo*,
                       const ActorFactory*, SceneMsgCtrl*, GameDataHolderBase*);
void initLayoutInitInfo(LayoutInitInfo*, const Scene*, const SceneInitInfo&);
void initPlacementAreaObj(Scene*, const ActorInitInfo&);
void initPlacementGravityObj(Scene*);
bool tryGetPlacementInfoAndCount(PlacementInfo*, s32*, const StageInfo*, const char*);
void initPlacementObjectMap(Scene*, const ActorInitInfo&, const char*);
void initPlacementByStageInfo(const StageInfo*, const char*, const ActorInitInfo&);
void initPlacementObjectDesign(Scene*, const ActorInitInfo&, const char*);
void initPlacementObjectSound(Scene*, const ActorInitInfo&, const char*);
LiveActor* tryInitPlacementSingleObject(Scene*, const ActorInitInfo&, s32, const char*);
LiveActor* tryInitPlacementSingleObject(Scene*, const ActorInitInfo&, s32, const char*,
                                        const char*);
bool tryInitPlacementActorGroup(LiveActorGroup*, Scene*, const ActorInitInfo&, s32, const char*,
                                const char*);
void initPlacementByStageInfoSingle(const StageInfo*, const char*, const ActorInitInfo&);
bool tryGetPlacementInfo(PlacementInfo*, const StageInfo*, const char*);
void getPlacementInfo(PlacementInfo*, const StageInfo*, const char*);
void getPlacementInfoAndCount(PlacementInfo*, s32*, const StageInfo*, const char*);
void initAreaObjDirector(Scene*, const AreaObjFactory*);
void initDemoDirector(Scene*, DemoDirector*);
void initHitSensorDirector(Scene*);
void initGravityHolder(Scene*);
void initItemDirector(Scene*, ItemDirectorBase*);
void initNatureDirector(Scene*);
void initSwitchAreaDirector(Scene*, s32, s32);
void registerSwitchOnAreaGroup(Scene*, SwitchOnAreaGroup*);
void registerSwitchKeepOnAreaGroup(Scene*, SwitchKeepOnAreaGroup*);
void initGraphicsSystemInfo(Scene*, const char*, s32);
void initCameraDirector(Scene*, const char*, s32, const CameraPoserFactory*);
void initCameraDirectorWithoutStageResource(Scene*, const CameraPoserFactory*);
void initCameraDirectorWithoutStageResource(Scene*, const CameraPoserFactory*);
void initCameraDirectorFix(Scene*, const sead::Vector3f&, const sead::Vector3f&,
                           const CameraPoserFactory*);
void initSceneCameraFovyDegree(Scene*, f32);
void initSnapShotCameraAudioKeeper(Scene*, IUseAudioKeeper*);
void setCameraAspect(Scene*, f32, f32);
void resetSceneInitEntranceCamera(Scene*);
void stopCameraByDeathPlayer(Scene*);
void restartCameraByDeathPlayer(Scene*);
void startInvalidCameraStopJudgeByDemo(Scene*);
void endInvalidCameraStopJudgeByDemo(Scene*);
void startCameraSnapShotMode(Scene*, bool);
void endCameraSnapShotMode(Scene*);
bool isCameraReverseInputH(const Scene*);
void onCameraReverseInputH(Scene*);
void offCameraReverseInputH(Scene*);
bool isCameraReverseInputV(const Scene*);
void onCameraReverseInputV(Scene*);
void offCameraReverseInputV(Scene*);
s32 getCameraStickSensitivityLevel(const Scene*);
void setCameraStickSensitivityLevel(Scene*, s32);
bool isValidCameraGyro(const Scene*);
void validateCameraGyro(Scene*);
void invalidateCameraGyro(Scene*);
s32 getCameraGyroSensitivityLevel(const Scene*);
void setCameraGyroSensitivityLevel(Scene*, s32);
PauseCameraCtrl* initAndCreatePauseCameraCtrl(Scene*, f32);
void startCameraPause(PauseCameraCtrl*);
void endCameraPause(PauseCameraCtrl*);
void initAudioDirector2D(Scene*, const SceneInitInfo&, AudioDirectorInitInfo&);
void initAudioDirector3D(Scene*, const SceneInitInfo&, AudioDirectorInitInfo&);
void initAudioDirector3D(Scene*, const SceneInitInfo&, AudioDirectorInitInfo&,
                         const sead::LookAtCamera*, const Projection*, AreaObjDirector*);
void initSceneAudioKeeper(Scene*, const SceneInitInfo&, const char*);
void setIsSafeFinalizingInParallelThread(Scene*, bool);
void updateKit(Scene*);
void updateKitTable(Scene*, const char*);
void updateKitList(Scene*, const char*, const char*);
void updateKitList(Scene*, const char*);
void updateLayoutKit(Scene*);
void updateEffect(Scene*);
void updateEffectSystem(Scene*);
void updateEffectPlayer(Scene*);
void updateEffectDemo(Scene*);
void updateEffectDemoWithPause(Scene*);
void updateEffectLayout(Scene*);
void updateGraphicsPrev(Scene*);
void updateKitListPrev(Scene*);
void updateKitListPost(Scene*);
void updateKitListPostDemoWithPauseNormalEffect(Scene*);
void updateKitListPostOnNerveEnd(Scene*);
void drawKit(const Scene*, const char*);
void drawKitList(const Scene*, const char*, const char*);
void drawLayoutKit(const Scene*, const char*);
void drawEffectDeferred(const Scene*, s32);
void startForwardPlayerScreenFader(const Scene*, s32, s32, f32);
void endForwardPlayerScreenFader(const Scene*, s32);
bool isStopScene(const Scene*);
void startCheckViewCtrlByCameraPos(Scene*);
void startCheckViewCtrlByLookAtPos(Scene*);
void startCheckViewCtrlByPlayerPos(Scene*);
bool isExistScreenCoverCtrl(const Scene*);
bool isRequestCaptureScreenCover(const Scene*);
void resetRequestCaptureScreenCover(const Scene*);
bool isOffDrawScreenCover(const Scene*);
void resetCaptureScreenCover(const Scene*);
void validatePostProcessingFilter(const Scene*);
void invalidatePostProcessingFilter(const Scene*);
void incrementPostProcessingFilterPreset(const Scene*);
void decrementPostProcessingFilterPreset(const Scene*);
s32 getPostProcessingFilterPresetId(const Scene*);
bool isActiveDemo(const Scene*);
const char* getActiveDemoName(const Scene*);
LiveActor* getDemoActorList(const Scene*);
s32 getDemoActorNum(const Scene*);
void updateDemoActor(const Scene*);
void updateDemoActorForPauseEffect(const Scene*);
void stopAllSe(const Scene*, u32);
void initPadRumble(const Scene*, const SceneInitInfo&);
void stopPadRumble(const Scene*);
void pausePadRumble(const Scene*);
void endPausePadRumble(const Scene*);
void validatePadRumble(Scene*);
void invalidatePadRumble(Scene*);
void setPadRumblePowerLevel(Scene*, s32);
const Resource* getPreLoadFileListArc();
bool tryRequestPreLoadFile(const Scene*, const SceneInitInfo&, s32, sead::Heap*);
}  // namespace al

namespace alSceneFunction {
void initAreaCameraSwitcherMultiForPrototype(const al::Scene*);
}
