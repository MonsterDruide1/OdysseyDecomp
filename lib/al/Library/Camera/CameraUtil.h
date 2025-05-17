#pragma once

#include <basis/seadTypes.h>
#include <gfx/seadCamera.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
class CameraTicket;
class ICameraInput;
struct CameraFlagCtrl;
class IUseCamera;
class CameraSubTargetBase;
class CameraSubTargetTurnParam;
class SceneCameraInfo;
class PlacementId;
class Resource;
struct ActorInitInfo;
class LiveActor;
class PlacementInfo;
class CameraPoser;
struct CameraPoseInfo;
class CameraTargetBase;
class Projection;
class SimpleCameraInput;
class ActorCameraTarget;
class ActorMatrixCameraTarget;
class ActorCameraSubTarget;
class ActorBackAroundCameraSubTarget;
class TransCameraSubTarget;
class CameraDistanceCurve;

SceneCameraInfo* getSceneCameraInfo(const IUseCamera* user);
s32 getViewNumMax(const IUseCamera* user);
s32 getViewNumMax(const SceneCameraInfo* info);
bool isValidView(const IUseCamera* user, s32 viewIdx);
bool isValidView(const SceneCameraInfo* info, s32 viewIdx);
const char* getViewName(const IUseCamera* user, s32 viewIdx);
const char* getViewName(const SceneCameraInfo* info, s32 viewIdx);
const sead::Matrix34f& getViewMtx(const IUseCamera* user, s32 viewIdx);
const sead::Matrix34f& getViewMtx(const SceneCameraInfo* info, s32 viewIdx);
const sead::Matrix34f* getViewMtxPtr(const IUseCamera* user, s32 viewIdx);
const sead::Matrix34f* getViewMtxPtr(const SceneCameraInfo* info, s32 viewIdx);
const sead::Matrix44f& getProjectionMtx(const IUseCamera* user, s32 viewIdx);
const sead::Matrix44f& getProjectionMtx(const SceneCameraInfo* info, s32 viewIdx);
const sead::Matrix44f* getProjectionMtxPtr(const IUseCamera* user, s32 viewIdx);
const sead::Matrix44f* getProjectionMtxPtr(const SceneCameraInfo* info, s32 viewIdx);
const sead::LookAtCamera& getLookAtCamera(const IUseCamera* user, s32 viewIdx);
const sead::LookAtCamera& getLookAtCamera(const SceneCameraInfo* info, s32 viewIdx);
const sead::Projection& getProjectionSead(const IUseCamera* user, s32 viewIdx);
const sead::Projection& getProjectionSead(const SceneCameraInfo* info, s32 viewIdx);
const Projection& getProjection(const IUseCamera* user, s32 viewIdx);
const Projection& getProjection(const SceneCameraInfo* info, s32 viewIdx);
const sead::Vector3f& getCameraPos(const IUseCamera* user, s32 viewIdx);
const sead::Vector3f& getCameraPos(const SceneCameraInfo* info, s32 viewIdx);
const sead::Vector3f& getCameraAt(const IUseCamera* user, s32 viewIdx);
const sead::Vector3f& getCameraAt(const SceneCameraInfo* info, s32 viewIdx);
const sead::Vector3f& getCameraUp(const IUseCamera* user, s32 viewIdx);
const sead::Vector3f& getCameraUp(const SceneCameraInfo* info, s32 viewIdx);
f32 getFovyDegree(const IUseCamera* user, s32 viewIdx);
f32 getFovyDegree(const SceneCameraInfo* info, s32 viewIdx);
f32 getFovy(const IUseCamera* user, s32 viewIdx);
f32 getFovy(const SceneCameraInfo* info, s32 viewIdx);
f32 getNear(const IUseCamera* user, s32 viewIdx);
f32 getNear(const SceneCameraInfo* info, s32 viewIdx);
f32 getFar(const IUseCamera* user, s32 viewIdx);
f32 getFar(const SceneCameraInfo* info, s32 viewIdx);
f32 calcCameraDistance(const IUseCamera* user, s32 viewIdx);
f32 calcFovxDegree(const IUseCamera* user, s32 viewIdx);
f32 calcCurrentFovyRate(const IUseCamera* user, s32 viewIdx);
void calcCameraFront(sead::Vector3f*, const IUseCamera* user, s32 viewIdx);
void setNearClipDistance(const IUseCamera* user, f32 distance, s32 updaterIdx);
void setFarClipDistance(const IUseCamera* user, f32 distance, s32 updaterIdx);
void setCurrentCameraPose(CameraPoseInfo* poseInfo, const IUseCamera* user);
void calcCameraDir(sead::Vector3f* dir, const IUseCamera* user, s32 viewIdx);
void calcCameraLookDir(sead::Vector3f* lookDir, const IUseCamera* user, s32 viewIdx);
void calcCameraSideDir(sead::Vector3f* sideDir, const IUseCamera* user, s32 viewIdx);
void calcCameraUpDir(sead::Vector3f* upDir, const IUseCamera* user, s32 viewIdx);
bool tryCalcCameraDir(sead::Vector3f* dir, const SceneCameraInfo* info, s32 viewIdx);
bool tryCalcCameraDirH(sead::Vector3f* dirH, const SceneCameraInfo* info,
                       const sead::Vector3f& upDir, s32 viewIdx);
bool tryCalcCameraLookDirH(sead::Vector3f* lookDirH, const SceneCameraInfo* info,
                           const sead::Vector3f& upDir, s32 viewIdx);
void startCamera(const IUseCamera* user, CameraTicket* ticket, s32 unk = -1);
void startCameraSub(const IUseCamera* user, CameraTicket* ticket, s32 unk = -1);
void startAnimCamera(const IUseCamera* user, CameraTicket* ticket, const char* animName,
                     s32 unk = 0);
void startAnimCameraAnim(CameraTicket* ticket, const char*, s32, s32, s32);
void startAnimCameraWithStartStepAndEndStepAndPlayStep(const IUseCamera* user, CameraTicket* ticket,
                                                       const char* animName, s32, s32, s32, s32);
void endCamera(const IUseCamera* user, CameraTicket* ticket, s32, bool);
void endCameraWithNextCameraPose(const IUseCamera* user, CameraTicket* ticket,
                                 const CameraPoseInfo* poseInfo, s32);
void endCameraSub(const IUseCamera* user, CameraTicket* ticket, s32);
bool isActiveCamera(const CameraTicket* ticket);
CameraTicket* initObjectCamera(const IUseCamera* user, const PlacementInfo& placementInfo,
                               const char*, const char*);
CameraTicket* initObjectCamera(const IUseCamera* user, const ActorInitInfo& actorInitInfo,
                               const char*, const char*);
CameraTicket* initObjectCameraNoPlacementInfo(const IUseCamera* user, const char*, const char*);
CameraTicket* initFixCamera(const IUseCamera* user, const char*, const sead::Vector3f&,
                            const sead::Vector3f& lookAtPos);
CameraTicket* initFixDoorwayCamera(const IUseCamera* user, const char*, const sead::Vector3f&,
                                   const sead::Vector3f& lookAtPos);
CameraTicket* initFixActorCamera(const LiveActor* actor, const ActorInitInfo& actorInitInfo,
                                 const char*, const sead::Vector3f&, f32, f32, f32, bool);
CameraTicket* initFixLookCamera(LiveActor* actor, const ActorInitInfo& actorInitInfo, const char*);
CameraTicket* initFixTalkCamera(const LiveActor* actor, const ActorInitInfo& actorInitInfo,
                                const char*, const sead::Vector3f&, f32, f32, f32, bool);
CameraTicket* initFixFishingCamera(const LiveActor* actor, const ActorInitInfo& actorInitInfo,
                                   const char*, const sead::Vector3f&, const sead::Vector3f&, f32,
                                   f32, f32, bool);
CameraTicket* initFixPointCamera(const IUseCamera* user, const ActorInitInfo& actorInitInfo,
                                 const char*, bool);
CameraTicket* initLookDownCamera(const LiveActor* actor, const ActorInitInfo& actorInitInfo,
                                 const char*);
CameraTicket* initProgramableCamera(const IUseCamera* user, const ActorInitInfo& actorInitInfo,
                                    const char*, const sead::Vector3f*, const sead::Vector3f*,
                                    const sead::Vector3f*);
CameraTicket* initProgramableCamera(const IUseCamera* user, const char*, const sead::Vector3f*,
                                    const sead::Vector3f*, const sead::Vector3f*);
CameraTicket* initProgramableCameraWithCollider(const IUseCamera* user,
                                                const ActorInitInfo& actorInitInfo, const char*,
                                                const sead::Vector3f*, const sead::Vector3f*,
                                                const sead::Vector3f*);
CameraTicket* initProgramableAngleCamera(const IUseCamera* user, const PlacementInfo& placementInfo,
                                         const char*, const sead::Vector3f*, const f32*, const f32*,
                                         const f32*);
CameraTicket* initProgramableCameraKeepColliderPreCamera(const IUseCamera* user,
                                                         const ActorInitInfo& actorInitInfo,
                                                         const char*, const sead::Vector3f*,
                                                         const sead::Vector3f*,
                                                         const sead::Vector3f*);
CameraTicket* initShooterCameraSingle(const IUseCamera* user, const char*);
CameraTicket* initTowerCameraWithSave(const IUseCamera* user, const sead::Vector3f*,
                                      const ActorInitInfo& actorInitInfo, const char*);
CameraTicket* initTowerCamera(const IUseCamera* user, const sead::Vector3f*,
                              const ActorInitInfo& actorInitInfo, const char*);
CameraTicket* initBossBattleCamera(const IUseCamera* user, const sead::Vector3f*,
                                   const ActorInitInfo& actorInitInfo, const char*);
void initProgramableCameraAngleSwing(CameraTicket* ticket);
CameraTicket* initFollowCameraSimple(const IUseCamera* user, const char*);
CameraTicket* initFollowCameraSimple(const IUseCamera* user, const ActorInitInfo& actorInitInfo,
                                     const char*);
CameraTicket* initDemoObjectCamera(const IUseCamera* user, const ActorInitInfo& actorInitInfo,
                                   const char*, const char*);
CameraTicket* initDemoProgramableCamera(const IUseCamera* user, const ActorInitInfo& actorInitInfo,
                                        const char*, const sead::Vector3f*, const sead::Vector3f*,
                                        const sead::Vector3f*);
CameraTicket* initDemoProgramableCameraKeepColliderPreCamera(const IUseCamera* user,
                                                             const ActorInitInfo& actorInitInfo,
                                                             const char*, const sead::Vector3f*,
                                                             const sead::Vector3f*,
                                                             const sead::Vector3f*);
CameraTicket* initDemoAnimCamera(const IUseCamera* user, const ActorInitInfo& actorInitInfo,
                                 const Resource*, const sead::Matrix34f*, const char*);
CameraTicket* initAnimCamera(const IUseCamera* user, const ActorInitInfo& actorInitInfo,
                             const Resource*, const sead::Matrix34f*, const char*);
CameraTicket* initDemoAnimCamera(const LiveActor* actor, const ActorInitInfo& actorInitInfo,
                                 const char*);
void loadActorCameraParam(CameraTicket* ticket, const LiveActor* actor, const char*, const char*);
void loadActorCameraParamInitFile(CameraTicket* ticket, const LiveActor* actor, const char*);
void setFixActorCameraTarget(CameraTicket* ticket, const LiveActor* actor);
void setFixActorCameraAngleH(CameraTicket* ticket, f32 angleH);
void setTowerCameraDistance(CameraTicket* ticket, f32 distance);
void setTowerCameraStartAngleV(CameraTicket* ticket, f32 startAngleV);
void setTowerCameraUserMarginAngleH(CameraTicket* ticket, f32 angleH);
void resetTowerCameraUserMarginAngleH(CameraTicket* ticket);
void resetTowerCameraInputRotate(CameraTicket* ticket, f32, s32);
CameraTicket* initSubjectiveCamera(const IUseCamera* user, const ActorInitInfo& actorInitInfo,
                                   const char*);
CameraTicket* initSubjectiveCameraNoSave(const IUseCamera* user, const char*);
f32 getSubjectiveCameraOffsetUp(const CameraTicket* ticket);
f32 getSubjectiveCameraOffsetFront();
void setSubjectiveCameraStartAngleH(const CameraTicket* ticket, f32 angleH);
void validateSubjectiveCameraResetAngleH(CameraTicket* ticket);
void requestSubjectiveCameraZoomIn(CameraTicket* ticket);
CameraTicket* initParallelCamera(const IUseCamera* user, const char*);
CameraTicket* initParallelCamera(const IUseCamera* user, const ActorInitInfo& actorInitInfo,
                                 const char*);
void setParallelCameraLookAtOffset(const CameraTicket* ticket, const sead::Vector3f& offset);
void setParallelCameraDistance(const CameraTicket* ticket, f32 distance);
void setParallelCameraAngleH(const CameraTicket* ticket, f32 angleH);
void setParallelCameraAngleV(const CameraTicket* ticket, f32 angleV);
CameraTicket* initQuickTurnCamera(const IUseCamera* user, const char*);
void setQuickTurnCameraFollow(CameraTicket* ticket);
void setQuickTurnCameraRotateFast(CameraTicket* ticket);
CameraTicket* initRaceCamera(const IUseCamera* user, const ActorInitInfo& actorInitInfo,
                             const char*);
void setRaceCameraFrontDirPtr(const CameraTicket* ticket, const sead::Vector3f* frontDirPtr);
void setRaceCameraDistance(const CameraTicket* ticket, f32 distance);
void setRaceCameraOffsetY(const CameraTicket* ticket, f32 offsetY);
void setRaceCameraAngleDegreeV(const CameraTicket* ticket, f32 angleV);
CameraTicket* initCartCamera(const IUseCamera* user, const ActorInitInfo& actorInitInfo,
                             const char*);
void stopCartCamera(const CameraTicket* ticket);
void restartCartCamera(const CameraTicket* ticket);
CameraTicket* initActorRailParallelCamera(const LiveActor* actor,
                                          const ActorInitInfo& actorInitInfo, const char*);
CameraTicket* initKinopioBrigadeCamera(const IUseCamera* user, const ActorInitInfo& actorInitInfo,
                                       const char*);
CameraTicket* initAnimCamera(const LiveActor* actor, const ActorInitInfo& actorInitInfo,
                             const char*);
void validateAnimCameraAngleSwing(CameraTicket* ticket);
void invalidateAnimCameraAngleSwing(CameraTicket* ticket);
void setAnimCameraBaseMtxPtr(CameraTicket* ticket, const sead::Matrix34f* baseMtxPtr);
CameraTicket* initEntranceCamera(const IUseCamera* user, const PlacementInfo& placementInfo,
                                 const char*);
CameraTicket* initEntranceCamera(const IUseCamera* user, const ActorInitInfo& actorInitInfo,
                                 const char*);
CameraTicket* initEntranceCameraNoSave(const IUseCamera* user, const PlacementInfo& placementInfo,
                                       const char*);
void setEntranceCameraParam(CameraTicket* ticket, f32, const sead::Vector3f&,
                            const sead::Vector3f&);
void setEntranceCameraLookAt(CameraTicket* ticket, const sead::Vector3f& lookAt);
void invalidateEndEntranceCamera(LiveActor* actor);
void invalidateEndEntranceCameraWithName(IUseCamera* user, const char* name);
void validateEndEntranceCamera(IUseCamera* user);
bool isPlayingEntranceCamera(const IUseCamera* user, s32 updaterIdx);
void setCameraInterpoleStep(CameraTicket* ticket, s32 step);
void setCameraFovyDegree(CameraTicket* ticket, f32 fovy);
SimpleCameraInput* createSimpleCameraInput(s32 port);
void setCameraInput(IUseCamera* user, const ICameraInput* cameraInput);
void setViewCameraInput(IUseCamera* user, const ICameraInput* cameraInput, s32 inputIdx);
bool isExistCameraInputAtDisableTiming(const IUseCamera* user, s32 inputIdx);
ActorCameraTarget* createActorCameraTarget(const LiveActor* actor, f32);
ActorCameraTarget* createActorCameraTarget(const LiveActor* actor, const sead::Vector3f*);
ActorMatrixCameraTarget* createActorJointCameraTarget(const LiveActor* actor,
                                                      const char* jointName);
ActorMatrixCameraTarget* createActorMatrixCameraTarget(const LiveActor* actor,
                                                       const sead::Matrix34f* matrix);
bool isActiveCameraTarget(const CameraTargetBase* target);
void setCameraTarget(IUseCamera* user, CameraTargetBase* target);
void resetCameraTarget(IUseCamera* user, CameraTargetBase* target);
ActorCameraSubTarget* createActorCameraSubTarget(const LiveActor* actor, const sead::Vector3f*);
ActorBackAroundCameraSubTarget* createActorBackAroundCameraSubTarget(const LiveActor* actor,
                                                                     const sead::Vector3f*);
TransCameraSubTarget* createTransCameraSubTarget(const char*, const sead::Vector3f*);
void initCameraSubTargetTurnParam(CameraSubTargetBase* target,
                                  const CameraSubTargetTurnParam* turnParam);
bool isActiveCameraSubTarget(const CameraSubTargetBase* target);
void setCameraSubTarget(IUseCamera* user, CameraSubTargetBase* target);
void resetCameraSubTarget(IUseCamera* user, CameraSubTargetBase* target);
void setCameraPlacementSubTarget(IUseCamera* user, CameraSubTargetBase* target);
void resetCameraPlacementSubTarget(IUseCamera* user, CameraSubTargetBase* target);
const CameraDistanceCurve* getCameraDistanceRocketFlowerCurve();
void setViewCameraTarget(IUseCamera* user, CameraTargetBase* target, s32);
void startCameraShakeByAction(const LiveActor* actor, const char*, const char*, s32, s32);
void startCameraShakeByHitReaction(const IUseCamera* user, const char*, const char*, const char*,
                                   s32, s32);
void requestCameraLoopShakeWeak(const IUseCamera* user);
bool isActiveCameraInterpole(const IUseCamera* user, s32);
bool isActiveCameraInterpole(const SceneCameraInfo* info, s32);
void startCameraInterpole(const IUseCamera* user, s32, s32);
void requestCancelCameraInterpole(const IUseCamera* user, s32);
bool tryCalcCameraPoseWithoutInterpole(sead::LookAtCamera* camera, const IUseCamera* user, s32);
void invalidateCameraPoserVerticalAbsorber(CameraTicket* ticket);
void requestStopCameraVerticalAbsorb(IUseCamera* user);
void validateSnapShotCameraZoomFovy(CameraTicket* ticket);
void validateSnapShotCameraRoll(CameraTicket* ticket);
bool isSnapShotOrientationRotate90(const IUseCamera* user);
bool isSnapShotOrientationRotate270(const IUseCamera* user);
bool isValidCameraGyro(const IUseCamera* user);
bool isInvalidChangeSubjectiveCamera(const IUseCamera* user);
bool isCurrentCameraZooming(const IUseCamera* user, s32);
void onCameraRideObj(const LiveActor* actor);
void offCameraRideObj(const LiveActor* actor);
bool isExistAnimCameraData(const CameraTicket* ticket, const char*);
bool isEndAnimCamera(const CameraTicket* ticket);
bool isAnimCameraPlaying(const CameraTicket* ticket);
bool isAnimCameraAnimPlaying(const CameraTicket* ticket, const char*);
s32 getAnimCameraStepMax(const CameraTicket* ticket);
s32 getAnimCameraStep(const CameraTicket* ticket);
s32 calcAnimCameraAnimStepMax(const CameraTicket* ticket, const char*);
void setAnimCameraRotateBaseUp(const CameraTicket* ticket);
}  // namespace al

namespace alCameraFunction {
al::CameraTicket* initCameraNoPlacementInfoNoSave(al::CameraPoser* poser,
                                                  const al::IUseCamera* user,
                                                  const al::PlacementId* placementId, const char*,
                                                  s32, const sead::Matrix34f&);
al::CameraTicket* initCameraNoSave(al::CameraPoser* poser, const al::IUseCamera* user,
                                   const al::ActorInitInfo& actorInitInfo, const char*, s32);
al::CameraTicket* initCamera(al::CameraPoser* poser, const al::IUseCamera* user,
                             const al::ActorInitInfo& actorInitInfo, const char*, s32);
al::CameraTicket* initCameraNoSave(al::CameraPoser* poser, const al::IUseCamera* user,
                                   const al::PlacementInfo& placementInfo, const char*, s32);
al::CameraTicket* initCameraNoPlacementInfo(al::CameraPoser* poser, const al::IUseCamera* user,
                                            const al::PlacementId* placementId, const char*, s32,
                                            const sead::Matrix34f&);
void requestCameraShakeLoop(const al::IUseCamera* user, const char*);
al::CameraTicket* initCamera(al::CameraPoser* poser, const al::IUseCamera* user,
                             const al::PlacementInfo& placementInfo, const char*, s32);
al::CameraTicket* initAreaCamera(const al::IUseCamera* user, const al::PlacementInfo& placementInfo,
                                 const char*);
al::CameraTicket* initForceAreaCamera(const al::IUseCamera* user,
                                      const al::PlacementInfo& placementInfo, const char*);
void initPriorityBossField(al::CameraTicket* ticket);
void initPriorityCapture(al::CameraTicket* ticket);
void initPriorityObject(al::CameraTicket* ticket);
void initPrioritySafetyPointRecovery(al::CameraTicket* ticket);
void initPriorityDemoTalk(al::CameraTicket* ticket);
void initPriorityDemo(al::CameraTicket* ticket);
void initPriorityDemo2(al::CameraTicket* ticket);
bool isCurrentCameraPriorityPlayer(const al::IUseCamera* user, s32);
void setPoserNearClipDistance(al::CameraTicket* ticket, f32 distance);
f32 getNearClipDistance(const al::IUseCamera* user, s32);
void validateCameraArea2D(al::IUseCamera* user);
void invalidateCameraArea2D(al::IUseCamera* user);
void validateCameraAreaKids(al::IUseCamera* user);
bool isValidCameraAreaKids(const al::CameraFlagCtrl* flagCtrl);
void onSeparatePlayMode(al::IUseCamera* user);
void offSeparatePlayMode(al::IUseCamera* user);
void validateResetPoseNextCamera(al::CameraTicket* ticket);
void validateKeepPreSelfPoseNextCamera(al::CameraTicket* ticket);
void validateCameraInterpoleEaseOut(al::CameraTicket* ticket);
void onForceCollideAtStartInterpole(al::CameraTicket* ticket);
void initCameraSettingCloudSea(al::IUseCamera* user, f32);
al::CameraTicket* initMirrorAreaCamera(const al::IUseCamera* user,
                                       const al::PlacementInfo& placementInfo, const char*);
}  // namespace alCameraFunction

void validateFixPointCameraUsePreCameraPos(al::CameraTicket* ticket);
