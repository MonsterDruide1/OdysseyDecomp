#pragma once

#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

class IUsePlayerHack;
class IUsePlayerCollision;
class CapTargetInfo;

namespace al {
class LiveActor;
class HitSensor;
class SensorMsg;
}  // namespace al

namespace rs {
CapTargetInfo* createCapTargetInfo(al::LiveActor*, const char*);
CapTargetInfo* createCapTargetInfoWithPlayerCollider(al::LiveActor*, IUsePlayerCollision*,
                                                     const char*);
CapTargetInfo* createCapTargetInfoHackMapParts(al::LiveActor*);
CapTargetInfo* createCapTargetInfoLockOnOnlyNoArchive(al::LiveActor*, const sead::Matrix34f*);
void requestLockOnCapHitReaction(const al::LiveActor*, const CapTargetInfo*, const char*);
IUsePlayerHack* startHack(al::HitSensor*, al::HitSensor*, al::LiveActor*);
void endHack(IUsePlayerHack**);
void endHackDir(IUsePlayerHack**, const sead::Vector3f&);
void endHackTargetQuat(IUsePlayerHack**, const sead::Quatf&, const sead::Vector3f&);
void endHackFromTargetPos(IUsePlayerHack**, const sead::Vector3f&);
void endHackFromTargetPos(IUsePlayerHack**, const sead::Vector3f&, const sead::Vector3f&);
void endHackFromTargetPos(IUsePlayerHack**, const sead::Vector3f&, const sead::Quatf&,
                          const sead::Vector3f&);
void endHackWithDamage(IUsePlayerHack**);
void endHackWithDamageAndVelocity(IUsePlayerHack**, const sead::Vector3f&);
void endHackEscapeScale(IUsePlayerHack**, f32);
void endHackDirEscapeScale(IUsePlayerHack**, const sead::Vector3f&, f32);
void endHackDirEscapeScalePose(IUsePlayerHack**, const sead::Quatf&, const sead::Vector3f&, f32);
void endHackAirVelocity(IUsePlayerHack**, const sead::Vector3f&, const sead::Quatf&,
                        const sead::Vector3f&, s32);
void endHackElectricWireToDestination(IUsePlayerHack**, const sead::Vector3f&,
                                      const sead::Vector3f&);
void endHackFastenerToDestination(IUsePlayerHack**, const sead::Vector3f&, const sead::Vector3f&);
void endHackBazookaElectricToDestination(IUsePlayerHack**, const sead::Vector3f&,
                                         const sead::Vector3f&, const sead::Quatf&, s32, f32);
void endHackThrowed(IUsePlayerHack**, const sead::Vector3f&);
bool requestDamage(IUsePlayerHack*);
void syncDamageVisibility(al::LiveActor*, const IUsePlayerHack*);
bool isDamageVisibilityHide(const IUsePlayerHack*);
bool isTouchHackCancelCollisionCode(const al::LiveActor*, const IUsePlayerCollision*);
bool isTouchHackDamageCollisionCode(const al::LiveActor*, const IUsePlayerCollision*);
void updateMaterialCode(al::LiveActor*, const IUsePlayerCollision*);
IUsePlayerHack* startHackStartDemo(IUsePlayerHack*, al::LiveActor*);
IUsePlayerHack* startHackStartDemoPuppetable(IUsePlayerHack*, al::LiveActor*);
void addHackStartDemoActor(IUsePlayerHack*, al::LiveActor*);
void endHackStartDemo(IUsePlayerHack*, al::LiveActor*);
bool tryEndHackStartDemo(IUsePlayerHack*, al::LiveActor*);
bool isActiveHackStartDemo(const IUsePlayerHack*);
bool isHackStartDemoEnterMario(const IUsePlayerHack*);
bool isHackerStopMove(const al::LiveActor*, const IUsePlayerHack*, f32);
bool isHackerStopMoveGround(const al::LiveActor*, const IUsePlayerHack*, f32,
                            const IUsePlayerCollision*);
bool sendMsgHackerNoReaction(const IUsePlayerHack*, al::HitSensor*, al::HitSensor*);
bool sendMsgHackerNoReactionWithoutShine(const IUsePlayerHack*, al::HitSensor*, al::HitSensor*);
bool isMsgHackerDamageAndCancel(const al::SensorMsg*);
bool isMsgEnableMapCheckPointWarpCollidedGround(const al::SensorMsg*, const al::LiveActor*);
bool isMsgEnableMapCheckPointWarpCollidedGround(const al::SensorMsg*, const IUsePlayerCollision*);
bool receiveMsgRequestTransferHack(const al::SensorMsg*, IUsePlayerHack*, al::HitSensor*);
void hideHackCap(IUsePlayerHack*);
void showHackCap(IUsePlayerHack*);
void hideShadowHackCap(IUsePlayerHack*);
bool isHackCapSeparateFlying(IUsePlayerHack*);
void calcGroundHeightHackerDepthShadowMapLength(al::LiveActor*, const IUsePlayerHack*,
                                                const sead::Vector3f&, f32, f32);
void cutGroundHeightHackerDepthShadowMapLength(al::LiveActor*, const IUsePlayerHack*, const char*,
                                               const sead::Vector3f&, f32, f32);
void checkExistHeightSpaceAboveGround(f32*, const IUsePlayerHack*, f32);
void requestRecoverySafetyPoint(IUsePlayerHack*, const sead::Vector3f&, const sead::Vector3f&);
bool trySnapTransToSnapMoveArea(al::LiveActor*, bool, const sead::Vector3f&, f32);
void updateHackActorVisibilityBySeparatePlay(al::LiveActor*, IUsePlayerHack*, const char*,
                                             const char*);
void startReset(al::LiveActor*);
void endReset(al::LiveActor*);
void calcHackerMoveVec(sead::Vector3f*, const IUsePlayerHack*, const sead::Vector3f&);
bool calcHackerMoveDir(sead::Vector3f*, const IUsePlayerHack*, const sead::Vector3f&);
void calcHackerWallInputVec(sead::Vector3f*, sead::Vector3f*, const IUsePlayerHack*,
                            const sead::Vector3f&);
void calcHackerWallInputVec(sead::Vector3f*, sead::Vector3f*, const sead::Vector2f&,
                            const sead::Vector3f&);
f32 getHackMoveStickRaw(const IUsePlayerHack*);
void calcHackerWallInputDir(sead::Vector3f*, sead::Vector3f*, const IUsePlayerHack*,
                            const sead::Vector3f&);
bool addHackActorAccelStick(al::LiveActor*, const IUsePlayerHack*, sead::Vector3f*, f32,
                            const sead::Vector3f&);
void calcHackMovePower(const IUsePlayerHack*);
void checkHackerMoveDir(const IUsePlayerHack*, const sead::Vector3f&, const sead::Vector3f&, f32);
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
void getHackStickRotateSpeed(const IUsePlayerHack*);
bool isTriggerHackSwing(const IUsePlayerHack*);
bool isTriggerHackSwingDirLeft(const IUsePlayerHack*);
bool isTriggerHackSwingDirRight(const IUsePlayerHack*);
bool isTriggerHackSwingDoubleHand(const IUsePlayerHack*);
bool isTriggerHackSwingDoubleHandReverseDir(const IUsePlayerHack*);
bool isTriggerHackSwingDoubleHandReverseInsideDir(const IUsePlayerHack*);
bool isTriggerHackSwingDoubleHandReverseOutsideDir(const IUsePlayerHack*);
void getHackPoseRotZDegreeLeft(const IUsePlayerHack*);
void getHackPoseRotZDegreeRight(const IUsePlayerHack*);
bool isTriggerTankCameraReset(const IUsePlayerHack*);
bool isTriggerHackSwingLeftHand(const IUsePlayerHack*);
bool isTriggerHackSwingRightHand(const IUsePlayerHack*);
void getHackSwingThrowDir(const IUsePlayerHack*);
void getHackSwingLeftHandDir(const IUsePlayerHack*);
void getHackSwingRightHandDir(const IUsePlayerHack*);
void getHackSwingVelLeftHand(const IUsePlayerHack*);
void getHackSwingVelRightHand(const IUsePlayerHack*);
void getHackCameraStickRaw(const IUsePlayerHack*);
void getRadiconInputSteeringValue(const IUsePlayerHack*);
void getRadiconInputThrottleValue(const IUsePlayerHack*);
bool isTriggerRecorderPlay(const IUsePlayerHack*);
bool isTriggerRecorderReverse(const IUsePlayerHack*);
bool isHoldRecorderPlay(const IUsePlayerHack*);
bool isHoldRecorderReverse(const IUsePlayerHack*);
bool isTriggerRecorderJump(const IUsePlayerHack*);
bool isTriggerRecorderCancel(const IUsePlayerHack*);
void getHackGemyLeftStickRaw();
void getHackGemyRightStickRaw();
bool isTriggerGemyResetCamera(const IUsePlayerHack*);
bool isTriggerGemyChangeCamera(const IUsePlayerHack*);
bool isTriggerSubjectiveCamera(const IUsePlayerHack*);
bool isTriggerFlutter(const IUsePlayerHack*, f32);
bool isTriggerFukankunZoomOn(const IUsePlayerHack*);
bool isTriggerFukankunZoomOff(const IUsePlayerHack*);
bool isHoldFukankunZoom(const IUsePlayerHack*);
void getPoleJumpPadPoseMtx(sead::Matrix33f*);
void getPoleJumpPadAcceleration(sead::Vector3f*);
bool isTriggerIceClimberClimbLeft(const IUsePlayerHack*);
bool isTriggerIceClimberClimbRight(const IUsePlayerHack*);
bool isTriggerIceClimberFall(const IUsePlayerHack*);
}  // namespace rs

namespace HackFunction {
bool isTriggerHackEnd(const IUsePlayerHack*);
bool isTriggerCancelBubbleLauncher(const IUsePlayerHack*);
}  // namespace HackFunction
