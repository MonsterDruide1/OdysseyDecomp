#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
class CameraTicket;
class AreaObj;
}  // namespace al

class HackObjInfo;

namespace PlayerCameraFunction {
void calcCameraMoveInput(sead::Vector2f* input, const al::LiveActor* actor);
bool tryCalcCameraSubMoveInput(sead::Vector2f* input, const al::LiveActor* actor);
bool checkNoCollisionForPlayerSubjectiveCamera(const al::LiveActor* actor,
                                               const al::CameraTicket* cameraTicket, bool);
bool isInvalidatePlayerInput(const al::LiveActor* actor);
bool isTriggerCameraReset(const al::LiveActor* actor);
bool isTriggerCameraSubjective(const al::LiveActor* actor);
bool isHoldCameraZoom(const al::LiveActor* actor);
bool isHoldCameraSnapShotZoomIn(const al::LiveActor* actor);
bool isHoldCameraSnapShotZoomOut(const al::LiveActor* actor);
bool isHoldCameraSnapShotRollLeft(const al::LiveActor* actor);
bool isHoldCameraSnapShotRollRight(const al::LiveActor* actor);
bool isCameraInWater(const al::LiveActor* actor);
bool isCameraClimbPole(const al::LiveActor* actor);
bool isCameraGrabCeil(const al::LiveActor* actor);
bool isCameraWallCatch(const al::LiveActor* actor);
bool isCameraMoonGravity(const al::LiveActor* actor);
bool isFailureCameraSubjective(const al::LiveActor* actor);
void setCameraHackName(al::LiveActor* hack, const HackObjInfo* hackObjInfo);
void resetCameraHackName(al::LiveActor* actor);
bool tryCalcPlayerRecoveryTarget(sead::Vector3f* targetPos, const al::AreaObj** safetyPointArea,
                                 const al::LiveActor* actor);
}  // namespace PlayerCameraFunction
