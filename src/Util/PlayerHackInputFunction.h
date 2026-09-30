#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

class IUsePlayerHack;

namespace al {
class LiveActor;
}  // namespace al

namespace rs {
void calcHackerMoveVec(sead::Vector3f*, const IUsePlayerHack*, const sead::Vector3f&);
bool calcHackerMoveDir(sead::Vector3f*, const IUsePlayerHack*, const sead::Vector3f&);
void calcHackerWallInputVec(sead::Vector3f*, sead::Vector3f*, const IUsePlayerHack*,
                            const sead::Vector3f&);
void calcHackerWallInputVec(sead::Vector3f*, sead::Vector3f*, const sead::Vector2f&,
                            const sead::Vector3f&);
f32 getHackMoveStickRaw(const IUsePlayerHack*);
bool calcHackerWallInputDir(sead::Vector3f*, sead::Vector3f*, const IUsePlayerHack*,
                            const sead::Vector3f&);
bool addHackActorAccelStick(al::LiveActor*, const IUsePlayerHack*, sead::Vector3f*, f32,
                            const sead::Vector3f&);
f32 calcHackMovePower(const IUsePlayerHack*);
bool checkHackerMoveDir(const IUsePlayerHack*, const sead::Vector3f&, const sead::Vector3f&, f32);
void calcHackerTrampleJumpParam(f32*, f32*, s32*, al::LiveActor*, const IUsePlayerHack*, f32, f32,
                                f32, f32, f32, s32, f32, f32, f32, f32, f32);
bool isOnHackMoveStick(const IUsePlayerHack*);
bool isHoldHackJump(const IUsePlayerHack*);
bool isOnHackMoveStickDeepDown(const IUsePlayerHack*);
bool isOnHackMoveStickGreater(const IUsePlayerHack*, f32);
bool isTriggerHackAction(const IUsePlayerHack*);
bool isTriggerHackPreInputAction(const IUsePlayerHack*);
bool isHoldHackAction(const IUsePlayerHack*);
bool isReleaseHackAction(const IUsePlayerHack*);
bool isTriggerHackJump(const IUsePlayerHack*);
bool isTriggerHackPreInputJump(const IUsePlayerHack*);
bool isReleaseHackJump(const IUsePlayerHack*);
bool isTriggerHackAnyButton(const IUsePlayerHack*);
bool isTriggerHackPreInputAnyButton(const IUsePlayerHack*);
bool isHoldHackAnyButton(const IUsePlayerHack*);
bool isReleaseHackAnyButton(const IUsePlayerHack*);
bool isTriggerHackSeparateJump(const IUsePlayerHack*);
bool isHoldHackSeparateJump(const IUsePlayerHack*);
f32 getHackStickRotateSpeed(const IUsePlayerHack*);
bool isTriggerHackSwing(const IUsePlayerHack*);
bool isTriggerHackSwingDirLeft(const IUsePlayerHack*);
bool isTriggerHackSwingDirRight(const IUsePlayerHack*);
bool isTriggerHackSwingDoubleHand(const IUsePlayerHack*);
bool isTriggerHackSwingDoubleHandReverseDir(const IUsePlayerHack*);
bool isTriggerHackSwingDoubleHandReverseInsideDir(const IUsePlayerHack*);
bool isTriggerHackSwingDoubleHandReverseOutsideDir(const IUsePlayerHack*);
f32 getHackPoseRotZDegreeLeft(const IUsePlayerHack*);
f32 getHackPoseRotZDegreeRight(const IUsePlayerHack*);
bool isTriggerTankCameraReset(const IUsePlayerHack*);
bool isTriggerHackSwingLeftHand(const IUsePlayerHack*);
bool isTriggerHackSwingRightHand(const IUsePlayerHack*);
const sead::Vector2f& getHackSwingThrowDir(const IUsePlayerHack*);
const sead::Vector2f& getHackSwingLeftHandDir(const IUsePlayerHack*);
const sead::Vector2f& getHackSwingRightHandDir(const IUsePlayerHack*);
const sead::Vector2f& getHackSwingVelLeftHand(const IUsePlayerHack*);
const sead::Vector2f& getHackSwingVelRightHand(const IUsePlayerHack*);
sead::Vector2f getHackCameraStickRaw(const IUsePlayerHack*);
f32 getRadiconInputSteeringValue(const IUsePlayerHack*);
f32 getRadiconInputThrottleValue(const IUsePlayerHack*);
bool isTriggerRecorderPlay(const IUsePlayerHack*);
bool isTriggerRecorderReverse(const IUsePlayerHack*);
bool isHoldRecorderPlay(const IUsePlayerHack*);
bool isHoldRecorderReverse(const IUsePlayerHack*);
bool isTriggerRecorderJump(const IUsePlayerHack*);
bool isTriggerRecorderCancel(const IUsePlayerHack*);
const sead::Vector2f& getHackGemyLeftStickRaw();
const sead::Vector2f& getHackGemyRightStickRaw();
bool isTriggerGemyResetCamera(const IUsePlayerHack*);
bool isTriggerGemyChangeCamera(const IUsePlayerHack*);
bool isTriggerSubjectiveCamera(const IUsePlayerHack*);
bool isTriggerFlutter(const IUsePlayerHack*, f32);
bool isTriggerFukankunZoomOn(const IUsePlayerHack*);
bool isTriggerFukankunZoomOff(const IUsePlayerHack*);
bool isHoldFukankunZoom(const IUsePlayerHack*);
void getPoleJumpPadPoseMtx(sead::Matrix33f*);
bool getPoleJumpPadAcceleration(sead::Vector3f*);
bool isTriggerIceClimberClimbLeft(const IUsePlayerHack*);
bool isTriggerIceClimberClimbRight(const IUsePlayerHack*);
bool isTriggerIceClimberFall(const IUsePlayerHack*);
}  // namespace rs

namespace HackFunction {
bool isTriggerHackEnd(const IUsePlayerHack*);
bool isTriggerCancelBubbleLauncher(const IUsePlayerHack*);
}  // namespace HackFunction
