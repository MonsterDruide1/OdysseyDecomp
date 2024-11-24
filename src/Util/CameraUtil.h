#pragma once

#include <basis/seadTypes.h>
#include <gfx/seadCamera.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
class CameraTicket;
class IUseCamera;
struct ActorInitInfo;
struct CameraStartInfo;
class CameraDistanceCurve;
}  // namespace al

namespace rs {
bool isEnableStartSubjectiveCamera(const al::LiveActor* actor);
al::CameraTicket* initFollowLimitCamera(const al::IUseCamera* user,
                                        const al::ActorInitInfo& actorInitInfo, const char*);
al::CameraTicket* initParallel2DCamera(const al::IUseCamera* user,
                                       const al::ActorInitInfo& actorInitInfo, const char*);
void invalidateFollowLimitCameraAutoResetLowAngleV(al::CameraTicket* ticket);
void updateCameraPosFromDistanceAndAngleHV(sead::Vector3f* newPos, const sead::Vector3f& cameraPos,
                                           f32 distance, f32 angleY, f32 angleH);
bool isPreCameraFollowOrParallel(const al::CameraStartInfo& cameraStartInfo);
al::CameraTicket* initGetMoonCamera(const al::IUseCamera* user, const al::LiveActor* moon,
                                    const al::ActorInitInfo& actorInitInfo, const char* name,
                                    sead::Vector3f*, sead::Vector3f*, sead::Vector3f*,
                                    sead::LookAtCamera* lookAtCamera);
al::CameraTicket* initWarpCheckpointCamera(const al::IUseCamera* user,
                                           const al::LiveActor* checkpoint,
                                           const al::ActorInitInfo& actorInitInfo, const char* name,
                                           sead::Vector3f*, sead::Vector3f*, sead::Vector3f*);
bool requestSetCameraAngleV(const al::LiveActor* actor, f32 requestAngleV, s32 poseUpdaterIdx);
bool requestResetCameraPosition(const al::LiveActor* actor, s32 poseUpdaterIdx);
bool requestResetCameraAngleV(const al::LiveActor* actor, s32 poseUpdaterIdx);
bool requestDownToDefaultCameraAngleBySpeed(const al::LiveActor* actor, f32 requestAngleSpeed,
                                            s32 poseUpdaterIdx);
bool requestUpToTargetCameraAngleBySpeed(const al::LiveActor* actor, f32 targetAngleV,
                                         f32 requestAngleSpeed, s32 poseUpdaterIdx);
bool requestMoveDownCameraAngleV(const al::LiveActor* actor, s32 poseUpdaterIdx);
bool requestRiseCameraAngleByPlayerCollideJumpCode(const al::LiveActor* actor);
bool requestCameraTurnToFront(const al::LiveActor* actor, f32, f32, s32 poseUpdaterIdx);
bool requestCameraTurnToFront(const al::LiveActor* actor, const sead::Vector3f& trans, f32, f32,
                              s32 poseUpdaterIdx);
void setEquipmentCameraDistanceCurve(al::LiveActor* actor, const al::CameraDistanceCurve* curve);
void resetEquipmentCameraDistanceCurve(al::LiveActor* actor);
void setBossCameraDistanceCurve(al::LiveActor* actor, const al::CameraDistanceCurve* curve);
void resetBossCameraDistanceCurve(al::LiveActor* actor);
void setKoopaShellCameraDistanceCurve(al::LiveActor* actor);
void resetKoopaShellCameraDistanceCurve(al::LiveActor* actor);
void setTRexPatrolCameraDistanceCurve(al::LiveActor* actor);
void resetTRexPatrolCameraDistanceCurve(al::LiveActor* actor);
}  // namespace rs
