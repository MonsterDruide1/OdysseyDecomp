#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
class HitSensor;
class LiveActor;
class SensorMsg;
}  // namespace al
class IUsePlayerPuppet;
class BindKeepDemoInfo;
class IUsePlayerCollision;

namespace rs {
IUsePlayerPuppet* startPuppet(al::HitSensor* actorHitSensor, al::HitSensor* playerHitSensor);
void endBindAndPuppetNull(IUsePlayerPuppet** playerPuppet);
void endBindOnGroundAndPuppetNull(IUsePlayerPuppet** playerPuppet);
void endBindJumpAndPuppetNull(IUsePlayerPuppet** playerPuppet, const sead::Vector3f& endTrans,
                              s32 timeBeforeEnd);
void endBindWallJumpAndPuppetNull(IUsePlayerPuppet** playerPuppet, const sead::Vector3f& endTrans,
                                  s32 timeBeforeEnd);
void endBindCapThrow(IUsePlayerPuppet** playerPuppet);
bool tryReceiveBindCancelMsgAndPuppetNull(IUsePlayerPuppet** playerPuppet,
                                          const al::SensorMsg* msg);
void setPuppetTrans(IUsePlayerPuppet* playerPuppet, const sead::Vector3f& trans);
void setPuppetVelocity(IUsePlayerPuppet* playerPuppet, const sead::Vector3f& velocity);
void setPuppetFront(IUsePlayerPuppet* playerPuppet, const sead::Vector3f& front);
void calcPuppetUp(sead::Vector3f* up, const IUsePlayerPuppet* playerPuppet);
void setPuppetUp(IUsePlayerPuppet* playerPuppet, const sead::Vector3f& up);
void calcPuppetFront(sead::Vector3f* front, const IUsePlayerPuppet* playerPuppet);
void setPuppetQuat(IUsePlayerPuppet* playerPuppet, const sead::Quatf& quat);
void resetPuppetPosition(IUsePlayerPuppet* playerPuppet, const sead::Vector3f& trans);
void addPuppetVelocityFall(IUsePlayerPuppet* playerPuppet);
const sead::Vector3f& getPuppetGravity(const IUsePlayerPuppet* playerPuppet);
const sead::Vector3f& getPuppetVelocity(const IUsePlayerPuppet* playerPuppet);
const sead::Vector3f& getPuppetTrans(const IUsePlayerPuppet* playerPuppet);
void calcPuppetQuat(sead::Quatf* quat, const IUsePlayerPuppet* playerPuppet);
void startPuppetAction(IUsePlayerPuppet* playerPuppet, const char* action);
bool isPuppetActionEnd(const IUsePlayerPuppet* playerPuppet);
bool isPuppetActionPlaying(const IUsePlayerPuppet* playerPuppet, const char* action);
void setPuppetAnimRate(IUsePlayerPuppet* playerPuppet, f32 animRate);
void setPuppetAnimBlendWeight(IUsePlayerPuppet* playerPuppet, f32 weight1, f32 weight2, f32 weight3,
                              f32 weight4, f32 weight5, f32 weight6);
f32 getPuppetAnimFrameMax(const IUsePlayerPuppet* playerPuppet);
void startPuppetHitReactionLand(IUsePlayerPuppet* playerPuppet);
void hidePuppet(IUsePlayerPuppet* playerPuppet);
void showPuppet(IUsePlayerPuppet* playerPuppet);
bool isPuppetHidden(const IUsePlayerPuppet* playerPuppet);
void syncPuppetVisibility(al::LiveActor* actor, const IUsePlayerPuppet* playerPuppet);
void hidePuppetSilhouette(IUsePlayerPuppet* playerPuppet);
void showPuppetSilhouette(IUsePlayerPuppet* playerPuppet);
void hidePuppetShadow(IUsePlayerPuppet* playerPuppet);
void showPuppetShadow(IUsePlayerPuppet* playerPuppet);
void validatePuppetSensor(IUsePlayerPuppet* playerPuppet);
void invalidatePuppetSensor(IUsePlayerPuppet* playerPuppet);
void invalidatePuppetCollider(IUsePlayerPuppet* playerPuppet);
bool isCollidedPuppetGround(IUsePlayerPuppet* playerPuppet);
const sead::Vector3f& getCollidedPuppetGroundNormal(IUsePlayerPuppet* playerPuppet);
void validatePuppetCollider(IUsePlayerPuppet* playerPuppet);
void requestDamage(IUsePlayerPuppet* playerPuppet);
void clearStainEffect(IUsePlayerPuppet* playerPuppet);
void copyPuppetDitherAlpha(IUsePlayerPuppet* playerPuppet, const al::LiveActor* actor);
void validatePuppetLookAt(IUsePlayerPuppet* playerPuppet);
void invalidatePuppetLookAt(IUsePlayerPuppet* playerPuppet);
void validatePuppetForceLookAt(IUsePlayerPuppet* playerPuppet, const sead::Vector3f& lookAt);
void invalidatePuppetForceLookAt(IUsePlayerPuppet* playerPuppet);
void validatePuppetSeparateMode(IUsePlayerPuppet* playerPuppet);
void invalidatePuppetSeparateMode(IUsePlayerPuppet* playerPuppet);
void validatePuppetReceivePush(IUsePlayerPuppet* playerPuppet);
void invalidatePuppetReceivePush(IUsePlayerPuppet* playerPuppet);
void forcePutOnPuppetCap(IUsePlayerPuppet* playerPuppet);
void forcePutOnPuppetCapWithEffect(IUsePlayerPuppet* playerPuppet);
void setForceRunAfterEndBind(IUsePlayerPuppet* playerPuppet, s32, f32);
void validateRecoveryArea(IUsePlayerPuppet* playerPuppet);
void invalidateRecoveryArea(IUsePlayerPuppet* playerPuppet);
void validateGuideArrow(IUsePlayerPuppet* playerPuppet);
void invalidateGuideArrow(IUsePlayerPuppet* playerPuppet);
void validateWaterSurfaceShadow(IUsePlayerPuppet* playerPuppet);
void invalidateWaterSurfaceShadow(IUsePlayerPuppet* playerPuppet);
bool isPuppetInputCapThrow(const IUsePlayerPuppet* playerPuppet);
bool isPuppetInputSwing(const IUsePlayerPuppet* playerPuppet);
BindKeepDemoInfo* initBindKeepDemoInfo();
bool isActiveBindKeepDemo(const BindKeepDemoInfo* bindKeepDemoInfo);
bool tryStartBindKeepDemo(BindKeepDemoInfo* bindKeepDemoInfo, const al::SensorMsg* msg,
                          const IUsePlayerPuppet* playerPuppet);
bool tryEndBindKeepDemo(BindKeepDemoInfo* bindKeepDemoInfo, const al::SensorMsg* msg,
                        IUsePlayerPuppet* playerPuppet);
bool isPuppetTrigJumpButton(const IUsePlayerPuppet* playerPuppet);
bool isPuppetHoldJumpButton(const IUsePlayerPuppet* playerPuppet);
bool isPuppetTrigActionButton(const IUsePlayerPuppet* playerPuppet);
bool isPuppetHoldActionButton(const IUsePlayerPuppet* playerPuppet);
bool isPuppetTriggerAnyButton(const IUsePlayerPuppet* playerPuppet);
bool isPuppetHoldAnyButton(const IUsePlayerPuppet* playerPuppet);
bool isPuppetMoveStickOn(const IUsePlayerPuppet* playerPuppet);
bool isPuppetStartWorldWarp(const IUsePlayerPuppet* playerPuppet);
bool isPuppetCancelWorldWarp(const IUsePlayerPuppet* playerPuppet);
const sead::Vector2f& getPuppetMoveStick(const IUsePlayerPuppet* playerPuppet);
bool isTriggerGetOff(const IUsePlayerPuppet* playerPuppet);
f32 getPuppetPoseRotZDegreeLeft(const IUsePlayerPuppet* playerPuppet);
f32 getPuppetPoseRotZDegreeRight(const IUsePlayerPuppet* playerPuppet);
void requestUpdateSafetyPoint(IUsePlayerPuppet* playerPuppet, const al::LiveActor* actor,
                              const IUsePlayerCollision* playerCollision);
void requestForceSafetyPoint(IUsePlayerPuppet* playerPuppet, const sead::Vector3f& recoveryTrans,
                             const sead::Vector3f& recoveryNormal);
}  // namespace rs
