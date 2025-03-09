#pragma once

#include <gfx/seadCamera.h>
#include <gfx/seadProjection.h>
#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
class CameraPoser;
class IUseCollision;
class PlacementInfo;
class Projection;
struct CameraObjectRequestInfo;
struct CameraStartInfo;
}  // namespace al

namespace alCameraPoserFunction {
class CameraCollisionHitResult;

s32 getViewIndex(const al::CameraPoser* poser);
const sead::LookAtCamera& getLookAtCamera(const al::CameraPoser* poser);
const sead::Projection& getProjectionSead(const al::CameraPoser* poser);
const al::Projection* getProjection(const al::CameraPoser* poser);
const sead::Matrix44f& getProjectionMtx(const al::CameraPoser* poser);
f32 getNear(const al::CameraPoser* poser);
f32 getFar(const al::CameraPoser* poser);
f32 getAspect(const al::CameraPoser* poser);
const sead::Vector3f& getPreCameraPos(const al::CameraPoser* poser);
const sead::Vector3f& getPreLookAtPos(const al::CameraPoser* poser);
const sead::Vector3f& getPreUpDir(const al::CameraPoser* poser);
f32 getPreFovyDegree(const al::CameraPoser* poser);
f32 getPreFovyRadian(const al::CameraPoser* poser);
bool isPrePriorityDemo(const al::CameraStartInfo& startInfo);
bool isPrePriorityDemo2(const al::CameraStartInfo& startInfo);
bool isPrePriorityDemoTalk(const al::CameraStartInfo& startInfo);
bool isPrePriorityDemoAll(const al::CameraStartInfo& startInfo);
bool isPrePriorityEntranceAll(const al::CameraStartInfo& startInfo);
bool isPrePriorityPlayer(const al::CameraStartInfo& startInfo);
bool isEqualPreCameraName(const al::CameraStartInfo& startInfo, const char* compareName);
bool isPreCameraFixAbsolute(const al::CameraStartInfo& startInfo);
bool isInvalidCollidePreCamera(const al::CameraStartInfo& startInfo);
bool isInvalidKeepPreCameraDistance(const al::CameraStartInfo& startInfo);
bool isInvalidKeepPreCameraDistanceIfNoCollide(const al::CameraStartInfo& startInfo);
bool isValidResetPreCameraPose(const al::CameraStartInfo& startInfo);
bool isValidKeepPreSelfCameraPose(const al::CameraStartInfo& startInfo);
f32 getPreCameraSwingAngleH(const al::CameraStartInfo& startInfo);
f32 getPreCameraSwingAngleV(const al::CameraStartInfo& startInfo);
f32 getPreCameraMaxSwingAngleH(const al::CameraStartInfo& startInfo);
f32 getPreCameraMaxSwingAngleV(const al::CameraStartInfo& startInfo);
bool isExistAreaAngleH(const al::CameraStartInfo& startInfo);
bool isExistAreaAngleV(const al::CameraStartInfo& startInfo);
f32 getAreaAngleH(const al::CameraStartInfo& startInfo);
f32 getAreaAngleV(const al::CameraStartInfo& startInfo);
bool isExistNextPoseByPreCamera(const al::CameraStartInfo& startInfo);
f32 getNextAngleHByPreCamera(const al::CameraStartInfo& startInfo);
f32 getNextAngleVByPreCamera(const al::CameraStartInfo& startInfo);
void calcCameraPose(sead::Quatf* quat, const al::CameraPoser* poser);
void calcLookDir(sead::Vector3f* vec, const al::CameraPoser* poser);
void calcCameraDir(sead::Vector3f* vec, const al::CameraPoser* poser);
void calcCameraDirH(sead::Vector3f* vec, const al::CameraPoser* poser);
void calcLookDirH(sead::Vector3f* vec, const al::CameraPoser* poser);
void calcSideDir(sead::Vector3f* vec, const al::CameraPoser* poser);
void calcPreCameraDir(sead::Vector3f* vec, const al::CameraPoser* poser);
void calcPreCameraDirH(sead::Vector3f* vec, const al::CameraPoser* poser);
void calcPreLookDir(sead::Vector3f* vec, const al::CameraPoser* poser);
void calcPreLookDirH(sead::Vector3f* vec, const al::CameraPoser* poser);
f32 calcPreCameraAngleH(const al::CameraPoser* poser);
f32 calcPreCameraAngleV(const al::CameraPoser* poser);
void calcTargetTrans(sead::Vector3f* vec, const al::CameraPoser* poser);
void calcTargetTransWithOffset(sead::Vector3f* vec, const al::CameraPoser* poser);
void setLookAtPosToTarget(al::CameraPoser* poser);
void setLookAtPosToTargetAddOffset(al::CameraPoser* poser, const sead::Vector3f& offset);
void setCameraPosToTarget(al::CameraPoser* poser);
void setCameraPosToTargetAddOffset(al::CameraPoser* poser, const sead::Vector3f& offset);
void calcTargetVelocity(sead::Vector3f* vec, const al::CameraPoser* poser);
void calcTargetVelocityH(sead::Vector3f* vec, const al::CameraPoser* poser);
void calcTargetUp(sead::Vector3f* vec, const al::CameraPoser* poser);
void calcTargetSpeedV(const al::CameraPoser* poser);
void calcTargetPose(sead::Quatf* quat, const al::CameraPoser* poser);
void calcTargetFront(sead::Vector3f* vec, const al::CameraPoser* poser);
void calcTargetSide(sead::Vector3f* vec, const al::CameraPoser* poser);
void calcTargetGravity(sead::Vector3f* vec, const al::CameraPoser* poser);
void calcTargetSpeedH(const al::CameraPoser* poser);
void calcTargetJumpSpeed(const al::CameraPoser* poser);
void calcTargetFallSpeed(const al::CameraPoser* poser);
bool isChangeTarget(const al::CameraPoser* poser);
bool tryGetTargetRequestDistance(f32*, const al::CameraPoser* poser);
bool tryGetBossDistanceCurve(const al::CameraPoser* poser);
bool tryGetEquipmentDistanceCurve(const al::CameraPoser* poser);
bool isExistCollisionUnderTarget(const al::CameraPoser* poser);
void getUnderTargetCollisionPos(const al::CameraPoser* poser);
void getUnderTargetCollisionNormal(const al::CameraPoser* poser);
bool isExistSlopeCollisionUnderTarget(const al::CameraPoser* poser);
bool isExistWallCollisionUnderTarget(const al::CameraPoser* poser);
bool tryCalcSlopeCollisionDownFrontDirH(sead::Vector3f* vec, const al::CameraPoser* poser);
void getSlopeCollisionUpSpeed(const al::CameraPoser* poser);
void getSlopeCollisionDownSpeed(const al::CameraPoser* poser);
bool isExistSubTarget(const al::CameraPoser* poser);
void checkValidTurnToSubTarget(const al::CameraPoser* poser);
void calcSubTargetBack(sead::Vector3f* vec, const al::CameraPoser* poser);
void calcSubTargetTrans(sead::Vector3f* vec, const al::CameraPoser* poser);
bool isChangeSubTarget(const al::CameraPoser* poser);
void calcSubTargetFront(sead::Vector3f* vec, const al::CameraPoser* poser);
void getSubTargetRequestDistance(const al::CameraPoser* poser);
void getSubTargetTurnSpeedRate1(const al::CameraPoser* poser);
void getSubTargetTurnSpeedRate2(const al::CameraPoser* poser);
void getSubTargetTurnRestartStep(const al::CameraPoser* poser);
bool tryCalcSubTargetTurnBrakeDistanceRate(f32*, const al::CameraPoser* poser);
bool isValidSubTargetTurnV(const al::CameraPoser* poser);
bool isValidSubTargetResetAfterTurnV(const al::CameraPoser* poser);
void clampAngleSubTargetTurnRangeV(f32*, const al::CameraPoser* poser);
void initCameraVerticalAbsorber(al::CameraPoser* poser);
void initCameraVerticalAbsorberNoCameraPosAbsorb(al::CameraPoser* poser);
void getCameraVerticalAbsorbPosUp(const al::CameraPoser* poser);
void getCameraVerticalAbsorbPosDown(const al::CameraPoser* poser);
void liberateVerticalAbsorb(al::CameraPoser* poser);
void stopUpdateVerticalAbsorb(al::CameraPoser* poser);
void stopUpdateVerticalAbsorbForSnapShotMode(al::CameraPoser* poser, const sead::Vector3f& vec);
void restartUpdateVerticalAbsorb(al::CameraPoser* poser);
void validateVerticalAbsorbKeepInFrame(al::CameraPoser* poser);
void invalidateVerticalAbsorbKeepInFrame(al::CameraPoser* poser);
void setVerticalAbsorbKeepInFrameScreenOffsetUp(al::CameraPoser* poser, f32);
void setVerticalAbsorbKeepInFrameScreenOffsetDown(al::CameraPoser* poser, f32);
void initCameraArrowCollider(al::CameraPoser* poser);
void initCameraArrowColliderWithoutThroughPassCollision(al::CameraPoser* poser);
void initCameraMoveLimit(al::CameraPoser* poser);
void initCameraAngleCtrl(al::CameraPoser* poser);
void initCameraAngleCtrlWithRelativeH(al::CameraPoser* poser);
void initCameraDefaultAngleRangeV(al::CameraPoser* poser, f32, f32);
void setCameraStartAngleV(al::CameraPoser* poser, f32);
void setCameraAngleV(al::CameraPoser* poser, f32);
void getCameraAngleH(const al::CameraPoser* poser);
void getCameraAngleV(const al::CameraPoser* poser);
void initAngleSwing(al::CameraPoser* poser);
bool isValidAngleSwing(const al::CameraPoser* poser);
void initCameraOffsetCtrlPreset(al::CameraPoser* poser);
void getOffset(const al::CameraPoser* poser);
void initGyroCameraCtrl(al::CameraPoser* poser);
void resetGyro(al::CameraPoser* poser);
void calcCameraGyroPose(const al::CameraPoser* poser, sead::Vector3f* vec, sead::Vector3f* vec0,
                        sead::Vector3f* vec1);
void getGyroFront(al::CameraPoser* poser);
void getGyroAngleV(al::CameraPoser* poser);
void getGyroAngleH(al::CameraPoser* poser);
void setGyroLimitAngleV(al::CameraPoser* poser, f32, f32);
void setGyroSensitivity(al::CameraPoser* poser, f32, f32);
void reduceGyroSencitivity(al::CameraPoser* poser);
void stopUpdateGyro(al::CameraPoser* poser);
void restartUpdateGyro(al::CameraPoser* poser);
bool isStopUpdateGyro(const al::CameraPoser* poser);
bool isTargetCollideGround(const al::CameraPoser* poser);
bool isTargetInWater(const al::CameraPoser* poser);
bool isTargetInMoonGravity(const al::CameraPoser* poser);
bool isTargetClimbPole(const al::CameraPoser* poser);
bool isTargetGrabCeil(const al::CameraPoser* poser);
bool isTargetInvalidMoveByInput(const al::CameraPoser* poser);
bool isTargetEnableEndAfterInterpole(const al::CameraPoser* poser);
bool isTargetWallCatch(const al::CameraPoser* poser);
bool isSnapShotMode(const al::CameraPoser* poser);
void initSnapShotCameraCtrl(al::CameraPoser* poser);
void initSnapShotCameraCtrlZoomAutoReset(al::CameraPoser* poser);
void initSnapShotCameraCtrlZoomRollMove(al::CameraPoser* poser);
void validateSnapShotCameraLookAtOffset(al::CameraPoser* poser);
void validateSnapShotCameraZoomFovy(al::CameraPoser* poser);
void validateSnapShotCameraRoll(al::CameraPoser* poser);
void updateSnapShotCameraCtrl(al::CameraPoser* poser);
void startResetSnapShotCameraCtrl(al::CameraPoser* poser, s32);
void setSnapShotMaxZoomOutFovyDegree(al::CameraPoser* poser, f32);
void getSnapShotRollDegree(const al::CameraPoser* poser);
void getSnapShotLookAtOffset(const al::CameraPoser* poser);
bool isOffVerticalAbsorb(const al::CameraPoser* poser);
void onVerticalAbsorb(al::CameraPoser* poser);
void offVerticalAbsorb(al::CameraPoser* poser);
void invalidateCameraBlur(al::CameraPoser* poser);
bool isRequestStopVerticalAbsorb(const al::CameraObjectRequestInfo&);
bool isRequestResetPosition(const al::CameraObjectRequestInfo&);
bool isRequestResetAngleV(const al::CameraObjectRequestInfo&);
bool isRequestDownToDefaultAngleBySpeed(const al::CameraObjectRequestInfo&);
bool isRequestUpToTargetAngleBySpeed(const al::CameraObjectRequestInfo&);
void getRequestTargetAngleV(const al::CameraObjectRequestInfo&);
void getRequestAngleSpeed(const al::CameraObjectRequestInfo&);
bool isRequestMoveDownAngleV(const al::CameraObjectRequestInfo&);
bool isRequestSetAngleV(const al::CameraObjectRequestInfo&);
void getRequestAngleV(const al::CameraObjectRequestInfo&);
bool isInvalidCollider(const al::CameraPoser* poser);
void validateCollider(al::CameraPoser* poser);
void invalidateCollider(al::CameraPoser* poser);
void validateCtrlSubjective(al::CameraPoser* poser);
void invalidateChangeSubjective(al::CameraPoser* poser);
void invalidateKeepDistanceNextCamera(al::CameraPoser* poser);
void invalidateKeepDistanceNextCameraIfNoCollide(al::CameraPoser* poser);
void invalidatePreCameraEndAfterInterpole(al::CameraPoser* poser);
bool isInvalidPreCameraEndAfterInterpole(const al::CameraPoser* poser);
bool isSceneCameraFirstCalc(const al::CameraPoser* poser);
bool isActiveInterpole(const al::CameraPoser* poser);
bool isInvalidEndEntranceCamera(const al::CameraPoser* poser);
bool isPause(const al::CameraPoser* poser);
void checkFirstCameraCollisionArrow(sead::Vector3f* vec, sead::Vector3f* vec0,
                                    const al::IUseCollision*, const sead::Vector3f& vec1,
                                    const sead::Vector3f& vec2);
void checkFirstCameraCollisionArrow(CameraCollisionHitResult*, const al::IUseCollision*,
                                    const sead::Vector3f& vec, const sead::Vector3f& vec0);
void checkFirstCameraCollisionArrowOnlyCeiling(sead::Vector3f* vec, sead::Vector3f* vec0,
                                               const al::IUseCollision*, const sead::Vector3f& vec1,
                                               const sead::Vector3f& vec2);
void checkCameraCollisionMoveSphere(sead::Vector3f* vec, const al::IUseCollision*,
                                    const sead::Vector3f& vec0, const sead::Vector3f& vec1, f32);
f32 calcZoneRotateAngleH(f32, const al::CameraPoser* poser);
f32 calcZoneRotateAngleH(f32, const sead::Matrix34f&);
f32 calcZoneInvRotateAngleH(f32, const sead::Matrix34f&);
void multVecZone(sead::Vector3f* vec, const sead::Vector3f& vec0, const al::CameraPoser* poser);
void multVecInvZone(sead::Vector3f* vec, const sead::Vector3f& vec0, const al::CameraPoser* poser);
void rotateVecZone(sead::Vector3f* vec, const sead::Vector3f& vec0, const al::CameraPoser* poser);
void calcOffsetCameraKeepInFrameV(sead::Vector3f* vec, sead::LookAtCamera* lookAtCam,
                                  const sead::Vector3f& vec0, const al::CameraPoser* poser, f32,
                                  f32);
void makeCameraKeepInFrameV(sead::LookAtCamera* lookAtCam, const sead::Vector3f& vec,
                            const al::CameraPoser* poser, f32, f32);
void initCameraRail(al::CameraPoser* poser, const al::PlacementInfo&, const char*);
bool tryGetCameraRailArg(f32*, const al::PlacementInfo&, const char*, const char*);
// void getCameraRailPointObjId(al::CameraPoser const*, s32);
bool tryFindNearestLimitRailKeeper(const al::CameraPoser* poser, const sead::Vector3f& vec);
sead::Vector2f calcCameraRotateStick(sead::Vector2f*, const al::CameraPoser* poser);
f32 calcCameraRotateStickH(const al::CameraPoser* poser);
f32 calcCameraRotateStickV(const al::CameraPoser* poser);
f32 calcCameraRotateStickPower(const al::CameraPoser* poser);
s32 getStickSensitivityLevel(const al::CameraPoser* poser);
f32 getStickSensitivityScale(const al::CameraPoser* poser);
bool isValidGyro(const al::CameraPoser* poser);
s32 getGyroSensitivityLevel(const al::CameraPoser* poser);
f32 getGyroSensitivityScale(const al::CameraPoser* poser);
bool isTriggerCameraResetRotate(const al::CameraPoser* poser);
bool isHoldCameraZoom(const al::CameraPoser* poser);
bool isHoldCameraSnapShotZoomIn(const al::CameraPoser* poser);
bool isHoldCameraSnapShotZoomOut(const al::CameraPoser* poser);
bool isHoldCameraSnapShotRollLeft(const al::CameraPoser* poser);
bool isHoldCameraSnapShotRollRight(const al::CameraPoser* poser);
bool tryCalcCameraSnapShotMoveStick(sead::Vector2f*, const al::CameraPoser* poser);
bool isPlayerTypeFlyer(const al::CameraPoser* poser);
bool isPlayerTypeHighSpeedMove(const al::CameraPoser* poser);
bool isPlayerTypeHighJump(const al::CameraPoser* poser);
bool isPlayerTypeNotTouchGround(const al::CameraPoser* poser);
bool isOnRideObj(const al::CameraPoser* poser);
}  // namespace alCameraPoserFunction
