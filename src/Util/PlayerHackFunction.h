#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
class HitSensor;
class LiveActor;
class SensorMsg;
}  // namespace al

class CapTargetInfo;
class IUsePlayerCollision;
class IUsePlayerHack;
struct HackEndParam;
class PlayerHackKeeper;

namespace rs {
CapTargetInfo* createCapTargetInfo(al::LiveActor* actor, const char* name);
CapTargetInfo* createCapTargetInfoWithPlayerCollider(al::LiveActor* actor,
                                                     IUsePlayerCollision* playerCollider,
                                                     const char* name);
CapTargetInfo* createCapTargetInfoHackMapParts(al::LiveActor* actor);
CapTargetInfo* createCapTargetInfoLockOnOnlyNoArchive(al::LiveActor* actor,
                                                      const sead::Matrix34f* poseMtx);
bool requestLockOnCapHitReaction(const al::LiveActor*, const CapTargetInfo* capTargetInfo,
                                 const char* name);
IUsePlayerHack* startHack(al::HitSensor* hackSensor, al::HitSensor* parentSensor,
                          al::LiveActor* hackActor);
void endHack(IUsePlayerHack** playerHack);
void initHackEndParam(HackEndParam*, PlayerHackKeeper*);
void endHackDir(IUsePlayerHack** playerHack, const sead::Vector3f& dir);
void endHackTargetQuat(IUsePlayerHack** playerHack, const sead::Quatf& pose,
                       const sead::Vector3f& dir);
void endHackFromTargetPos(IUsePlayerHack** playerHack, const sead::Vector3f& targetPos);
void endHackFromTargetPos(IUsePlayerHack** playerHack, const sead::Vector3f& dir,
                          const sead::Vector3f& targetPos);
void endHackFromTargetPos(IUsePlayerHack** playerHack, const sead::Vector3f& targetPos,
                          const sead::Quatf& pose, const sead::Vector3f& dir);
void endHackWithDamage(IUsePlayerHack** playerHack);
void endHackWithDamageAndVelocity(IUsePlayerHack** playerHack, const sead::Vector3f& vel);
void endHackEscapeScale(IUsePlayerHack** playerHack, f32 escapeScale);
void endHackDirEscapeScale(IUsePlayerHack** playerHack, const sead::Vector3f& dir, f32 escapeScale);
void endHackDirEscapeScalePose(IUsePlayerHack** playerHack, const sead::Quatf& pose,
                               const sead::Vector3f& dir, f32 escapeScale);
void endHackAirVelocity(IUsePlayerHack** playerHack, const sead::Vector3f& targetPos,
                        const sead::Quatf& pose, const sead::Vector3f& vel, s32 frameDelay);
void endHackElectricWireToDestination(IUsePlayerHack** playerHack, const sead::Vector3f& targetPos,
                                      const sead::Vector3f& vel);
void endHackElectricWireToDestination(IUsePlayerHack** playerHack, const sead::Vector3f& targetPos,
                                      const sead::Vector3f& posAfterHack);
void endHackFastenerToDestination(IUsePlayerHack** playerHack, const sead::Vector3f& start,
                                  const sead::Vector3f& end);
void endHackBazookaElectricToDestination(IUsePlayerHack** playerHack, const sead::Vector3f& start,
                                         const sead::Vector3f& end, const sead::Quatf& pose,
                                         s32 frameDelay, f32 height);
void endHackThrowed(IUsePlayerHack** playerHack, const sead::Vector3f& vel);
bool requestDamage(IUsePlayerHack* playerHack);
void syncDamageVisibility(al::LiveActor* actor, const IUsePlayerHack* playerHack);
bool isDamageVisibilityHide(const IUsePlayerHack* playerHack);
bool isTouchHackCancelCollisionCode(const al::LiveActor* actor,
                                    const IUsePlayerCollision* playerCollider);
bool isTouchHackDamageCollisionCode(const al::LiveActor* actor,
                                    const IUsePlayerCollision* playerCollider);
void updateMaterialCode(al::LiveActor* actor, const IUsePlayerCollision* playerCollider);
void startHackStartDemo(IUsePlayerHack* playerHack, al::LiveActor* actor);
void startHackStartDemoPuppetable(IUsePlayerHack* playerHack, al::LiveActor* actor);
void addHackStartDemoActor(IUsePlayerHack* playerHack, al::LiveActor* actor);
void endHackStartDemo(IUsePlayerHack* playerHack, al::LiveActor* actor);
bool tryEndHackStartDemo(IUsePlayerHack* playerHack, al::LiveActor* actor);
bool isActiveHackStartDemo(const IUsePlayerHack* playerHack);
bool isHackStartDemoEnterMario(const IUsePlayerHack* playerHack);
bool isHackerStopMove(const al::LiveActor* actor, const IUsePlayerHack* playerHack, f32 speedLimit);
bool isHackerStopMoveGround(const al::LiveActor* actor, const IUsePlayerHack* playerHack,
                            f32 speedLimit, const IUsePlayerCollision* playerCollider);
bool sendMsgHackerNoReaction(const IUsePlayerHack* playerHack, al::HitSensor* receiver,
                             al::HitSensor* sender);
void sendMsgHackerNoReactionWithoutShine(const IUsePlayerHack* playerHack, al::HitSensor* receiver,
                                         al::HitSensor* sender);
bool isMsgHackerDamageAndCancel(const al::SensorMsg* message);
bool isMsgEnableMapCheckPointWarpCollidedGround(const al::SensorMsg* message,
                                                const al::LiveActor* actor);
bool isMsgEnableMapCheckPointWarpCollidedGround(const al::SensorMsg* message,
                                                const IUsePlayerCollision* playerCollider);
bool receiveMsgRequestTransferHack(const al::SensorMsg* message, IUsePlayerHack* playerHack,
                                   al::HitSensor* hitSensor);
void hideHackCap(IUsePlayerHack* playerHack);
void showHackCap(IUsePlayerHack* playerHack);
void hideShadowHackCap(IUsePlayerHack* playerHack);
bool isHackCapSeparateFlying(IUsePlayerHack* playerHack);
f32 calcGroundHeightHackerDepthShadowMapLength(al::LiveActor* actor,
                                               const IUsePlayerHack* playerHack,
                                               const sead::Vector3f&, f32, f32 maxHeight);
void cutGroundHeightHackerDepthShadowMapLength(al::LiveActor* actor,
                                               const IUsePlayerHack* playerHack,
                                               const char* mapName, const sead::Vector3f&, f32,
                                               f32 maxHeight);
bool checkExistHeightSpaceAboveGround(f32* out, const IUsePlayerHack* playerHack, f32 maxHeight);
bool requestRecoverySafetyPoint(IUsePlayerHack* playerHack, const sead::Vector3f& pos,
                                const sead::Vector3f& normal);
bool trySnapTransToSnapMoveArea(al::LiveActor* actor, bool, const sead::Vector3f&, f32);
void updateHackActorVisibilityBySeparatePlay(al::LiveActor* actor, IUsePlayerHack* playerHack,
                                             const char* animName,
                                             const char* animSeparateFlyingName);
void startReset(al::LiveActor* actor);
void endReset(al::LiveActor* actor);
}  // namespace rs
