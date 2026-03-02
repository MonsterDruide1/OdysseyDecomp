#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
class AreaObj;
class CollisionParts;
class CollisionPartsFilterBase;
struct HitInfo;
class HitSensor;
class IUseCollision;
class LiveActor;
class Triangle;
}  // namespace al

class IUsePlayerCollision;
class IUsePlayerHeightCheck;
class PlayerConst;

namespace rs {
void resetCollision(IUsePlayerCollision*);
void resetCollisionPose(const IUsePlayerCollision*, const sead::Quatf&);
void resetCollisionExpandCheck(IUsePlayerCollision*);
bool isCollidedGround(const IUsePlayerCollision*);
bool isCollidedGround(const IUsePlayerCollision*);
bool isCollidedWall(const IUsePlayerCollision*);
bool isCollidedCeiling(const IUsePlayerCollision*);
bool isCollidedGroundFace(const IUsePlayerCollision*);
bool isCollidedWallFace(const IUsePlayerCollision*);
const sead::Vector3f& getCollidedGroundNormal(const IUsePlayerCollision*);
const sead::Vector3f& getCollidedWallNormal(const IUsePlayerCollision*);
const sead::Vector3f& getCollidedCeilingNormal(const IUsePlayerCollision*);
const sead::Vector3f& getCollidedGroundPos(const IUsePlayerCollision*);
const sead::Vector3f& getCollidedWallPos(const IUsePlayerCollision*);
const sead::Vector3f& getCollidedCeilingPos(const IUsePlayerCollision*);
void* getCollidedFixReaction(const IUsePlayerCollision*);
al::HitSensor* tryGetCollidedGroundSensor(const IUsePlayerCollision*);
al::HitSensor* tryGetCollidedWallSensor(const IUsePlayerCollision*);
al::HitSensor* tryGetCollidedCeilingSensor(const IUsePlayerCollision*);
const al::CollisionParts* getCollidedGroundCollisionParts(const IUsePlayerCollision*);
const al::CollisionParts* getCollidedWallCollisionParts(const IUsePlayerCollision*);
const al::CollisionParts* getCollidedCeilingCollisionParts(const IUsePlayerCollision*);
s32 getGroundHitInfoNum(const IUsePlayerCollision*);
const sead::Vector3f& getGroundHitInfoNormal(const IUsePlayerCollision*, s32);
al::HitSensor* getGroundHitInfoSensor(const IUsePlayerCollision*, s32);
const al::CollisionParts* getGroundHitInfoCollisionParts(const IUsePlayerCollision*, s32);
s32 getWallHitInfoNum(const IUsePlayerCollision*);
const sead::Vector3f& getWallHitInfoNormal(const IUsePlayerCollision*, s32);
al::HitSensor* getWallHitInfoSensor(const IUsePlayerCollision*, s32);
const al::CollisionParts* getWallHitInfoCollisionParts(const IUsePlayerCollision*, s32);
bool isExistCollidedGroundParts(const IUsePlayerCollision*, const al::CollisionParts*);

bool isCollisionCodePress(const al::HitInfo&);
bool isCollisionCodeGrabCeil(const al::HitInfo&);
bool isCollisionCodeDeadGround(const IUsePlayerCollision*);
bool isCollisionCodeDeadWall(const IUsePlayerCollision*);
bool isCollisionCodePoisonTouch(const IUsePlayerCollision*);
bool isCollisionCodeDead2dGround(const IUsePlayerCollision*);
bool isCollisionCodeDead2dCeiling(const IUsePlayerCollision*);
bool isCollisionCodeDamageGround(const IUsePlayerCollision*);
bool isCollisionCodeDamageWall(const IUsePlayerCollision*);
bool isCollisionCodeDamageCeiling(const IUsePlayerCollision*);
bool isCollisionCodeDamageFireGround(const IUsePlayerCollision*);
bool isCollisionCodeDamageFireWall(const IUsePlayerCollision*);
bool isCollisionCodeDamageFireCeiling(const IUsePlayerCollision*);
bool isCollisionCodeSandSink(const IUsePlayerCollision*);
bool isCollisionCodeSlide(const IUsePlayerCollision*);
bool isCollisionCodeJump(const IUsePlayerCollision*);
bool isCollisionCodeJumpSmall(const IUsePlayerCollision*);
bool isCollisionCodeBed(const IUsePlayerCollision*);
bool isCollisionCodeChair(const IUsePlayerCollision*);
bool isCollisionCodeGrabCeilAny(const IUsePlayerCollision*);
bool isCollisionCodeGrabCeilGrround(const IUsePlayerCollision*);
bool isCollisionCodeGrabCeilWall(const IUsePlayerCollision*);
bool isCollisionCodeGrabCeilCeiling(const IUsePlayerCollision*);
bool isCollisionCodePoleClimbAny(const IUsePlayerCollision*);
bool isCollisionCodePoleClimbGround(const IUsePlayerCollision*);
bool isCollisionCodePoleClimbWall(const IUsePlayerCollision*);
bool isCollisionCodePoleClimbCeiling(const IUsePlayerCollision*);
bool isCollisionCodePoleClimb(const al::HitInfo&);
bool isCollisionCodePole(const al::Triangle&);

bool isActionCodeNoWallKeepWall(const IUsePlayerCollision*);
bool isActionCodeNoActionGround(const IUsePlayerCollision*);
bool isActionCodeNoActionWall(const IUsePlayerCollision*);
bool isActionCodeNoActionCeiling(const IUsePlayerCollision*);
bool isActionCodeNoWallGrab(const IUsePlayerCollision*);
bool isActionCodeNoWallGrab(const al::HitInfo&);
bool isActionCodeNoWallPopUp(const al::HitInfo&);
bool isActionCodeNoTongueClingGround(const IUsePlayerCollision*);
bool isActionCodeNoTongueClingWall(const IUsePlayerCollision*);

f32 getPoleCodeAngleOffsetGround(const IUsePlayerCollision*);
f32 getPoleCodeAngleOffset(const al::HitInfo&);
f32 getPoleCodeAngleOffsetWall(const IUsePlayerCollision*);
f32 getPoleCodeAngleOffsetCeiling(const IUsePlayerCollision*);

const char* getMaterialCodeGround(const IUsePlayerCollision*);
const char* getMaterialCodeWall(const IUsePlayerCollision*);
const char* getMaterialCodeCeiling(const IUsePlayerCollision*);

bool isRippleGenearateMaterialLawn(const al::Triangle&);
const char* getRippleGenerateMaterialFlower();
bool isCollidedDamageCodeAnyWallHit(const IUsePlayerCollision*);
bool isEnableRecordSafetyPoint(sead::Vector3f**, const al::HitInfo&, al::HitSensor*,
                               const sead::Vector3f&);
bool isCollisionCodeSafetyPoint(const al::HitInfo&);
bool isCollisionCodeSafetyPoint(const al::HitInfo&);
void calcCollisionCodeNoSafetyPointPos(sead::Vector3f*, bool*, const al::LiveActor*,
                                       const IUsePlayerCollision*);
void calcActorCollisionCodeNoSafetyPointPos(sead::Vector3f*, bool*, const al::LiveActor*,
                                            const IUsePlayerCollision*);
bool calcCollidedGroundSafetyPoint(sead::Vector3f*, sead::Vector3f*, const al::AreaObj**,
                                   sead::Vector3f**, const al::LiveActor*,
                                   const IUsePlayerCollision*, al::HitSensor*,
                                   const sead::Vector3f&, const sead::Vector3f&);
bool calcActorCollidedGroundSafetyPoint(sead::Vector3f*, sead::Vector3f*, const al::AreaObj**,
                                        sead::Vector3f**, const al::LiveActor*, al::HitSensor*,
                                        const IUsePlayerCollision*);
bool isJustLand(const IUsePlayerCollision*);
void calcMovePowerGround(sead::Vector3f*, const IUsePlayerCollision*, const sead::Vector3f&);
void calcMovePowerWall(sead::Vector3f*, const IUsePlayerCollision*, const sead::Vector3f&);
void calcMovePowerCeiling(sead::Vector3f*, const IUsePlayerCollision*, const sead::Vector3f&);
bool isOnGround(const al::LiveActor*, const IUsePlayerCollision*);
bool isOnGroundAndGravity(const al::LiveActor*, const IUsePlayerCollision*);
bool isCollidedWallVelocity(const al::LiveActor*, const IUsePlayerCollision*);
bool isCollidedCeilingVelocity(const al::LiveActor*, const IUsePlayerCollision*);
void calcCollidedNormalSum(sead::Vector3f*, const IUsePlayerCollision*);
bool reboundVelocityFromCollision(al::LiveActor*, const IUsePlayerCollision*, f32, f32, f32);
u32 reboundVelocityPart(al::LiveActor*, const IUsePlayerCollision*, f32, f32, f32, f32);
void cutVectorCollision(sead::Vector3f*, const IUsePlayerCollision*, f32);

bool isCollidedNoScaleVelocityWall(const IUsePlayerCollision*);
bool isCollidedGroundOverAngle(const al::LiveActor*, const IUsePlayerCollision*, f32);
bool isCollidedGroundLessAngle(const al::LiveActor*, const IUsePlayerCollision*, f32);
bool isCollidedGroundRunAngle(const al::LiveActor*, const IUsePlayerCollision*, const PlayerConst*);
bool isOnGroundRunAngle(const al::LiveActor*, const IUsePlayerCollision*, const PlayerConst*);
bool isOnGroundLessAngle(const al::LiveActor*, const IUsePlayerCollision*, f32);
bool isLandGroundRunAngle(const al::LiveActor*, const IUsePlayerCollision*, const PlayerConst*);
bool isOnGroundSlopeSlideStart(const al::LiveActor*, const IUsePlayerCollision*,
                               const PlayerConst*);
bool isOnGroundSlopeSlideEnd(const al::LiveActor*, const IUsePlayerCollision*, const PlayerConst*);
bool isOnGroundForceSlideCode(const al::LiveActor*, const IUsePlayerCollision*, const PlayerConst*);
bool isOnGroundForceRollingCode(const al::LiveActor*, const IUsePlayerCollision*);
bool isOnGroundSkateCode(const al::LiveActor*, const IUsePlayerCollision*);
bool isAutoRunOnGroundSkateCode(const al::LiveActor*, const IUsePlayerCollision*, f32);
bool isPressedCollision(const IUsePlayerCollision*);
bool isPressedGround(const IUsePlayerCollision*);
bool isPressedWall(const IUsePlayerCollision*);

const sead::Vector3f& getPressCollisionNormal(const IUsePlayerCollision*);
const sead::Vector3f& getPressCollisionHitPos(const IUsePlayerCollision*);
void setColliderFilterCollisionParts(IUsePlayerCollision*, const al::CollisionPartsFilterBase*);
void createAndSetColliderFilterSpecialPurpose(IUsePlayerCollision*, const char*);
void startHitReactionLandIfLanding(const al::LiveActor*, const IUsePlayerCollision*, bool);
void startHitReactionLandJumpIfLanding(const al::LiveActor*, const IUsePlayerCollision*, bool);
void startHitReactionLandRunIfLanding(const al::LiveActor*, const IUsePlayerCollision*, bool);
bool isAboveGround(const IUsePlayerHeightCheck*);
f32 getGroundHeight(const IUsePlayerHeightCheck*);
void updateCollider(al::LiveActor*, IUsePlayerCollision*, const sead::Vector3f&);
bool calcGroundHeight(f32*, sead::Vector3f*, const al::IUseCollision*, const sead::Vector3f&,
                      const sead::Vector3f&, f32, f32);

}  // namespace rs
