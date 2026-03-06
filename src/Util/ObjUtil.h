#pragma once

#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
class CollisionParts;
class HitSensor;
class IUseCollision;
class LiveActor;
class WaterSurfaceFinder;
}  // namespace al

class IPlayerModelChanger;
class IUseDimension;
class IUsePlayerCeilingCheck;
class IUsePlayerCollision;
class IUsePlayerHeightCheck;
class PlayerActionGroundMoveControl;
class PlayerAnimator;
class PlayerConst;
class PlayerInput;
class PlayerJointParamCenterDynamics;
class PlayerModelHolder;
class PlayerTrigger;
class PlayerWallActionHistory;

namespace rs {
bool isEnableSendTrampleMsg(const al::LiveActor*, al::HitSensor*, al::HitSensor*);
bool isEnableSendUpperPunchMsg(const al::LiveActor*, al::HitSensor*, al::HitSensor*);
bool trySendMsgPlayerAttackTrample(const al::LiveActor*, al::HitSensor*, al::HitSensor*);
bool trySendMsgPlayerReflectOrTrample(const al::LiveActor*, al::HitSensor*, al::HitSensor*);
void sendPlayerCollisionTouchMsg(const al::LiveActor*, al::HitSensor*, const IUsePlayerCollision*);
bool sendPlayerCollisionUpperPunchMsg(const al::LiveActor*, al::HitSensor*,
                                      const IUsePlayerCollision*, const IPlayerModelChanger*);
void calcGroundNormalOrUpDir(sead::Vector3f*, const al::LiveActor*, const IUsePlayerCollision*);
void calcGroundNormalOrGravityDir(sead::Vector3f*, const al::LiveActor*,
                                  const IUsePlayerCollision*);
void calcGroundNormalExceptJustLandOrGravityDir(sead::Vector3f*, const al::LiveActor*,
                                                const IUsePlayerCollision*);
bool calcOnGroundNormalOrGravityDir(sead::Vector3f*, const al::LiveActor*,
                                    const IUsePlayerCollision*);
bool calcSnapGroundNormalOrGravityDir(sead::Vector3f*, sead::Vector3f*, al::LiveActor*,
                                      const IUsePlayerCollision*, f32, const sead::Vector3f&);
void calcWallNormalOrReverseFrontDir(sead::Vector3f*, const al::LiveActor*,
                                     const IUsePlayerCollision*);
bool calcAlongSkyFront(sead::Vector3f*, const al::LiveActor*);
bool calcAlongDirFront(sead::Vector3f*, const al::LiveActor*, const sead::Vector3f&);
void calcFrontVerticalGravity(sead::Vector3f*, const al::LiveActor*);
void faceToAttackSensor(al::LiveActor*, al::HitSensor*);
void faceToCamera(al::LiveActor*);
void convertWallRunDirection(sead::Vector3f*, sead::Vector3f*, const al::LiveActor*,
                             const sead::Vector3f&);
void calcFrontVelocityAndDirH(sead::Vector3f*, sead::Vector3f*, const al::LiveActor*,
                              const IUsePlayerCollision*);
void reflectCeiling(al::LiveActor*, f32);
void reflectCeilingUpperPunch(al::LiveActor*, const IUsePlayerCollision*, const PlayerInput*,
                              const PlayerConst*, const PlayerTrigger*, bool);
f32 calcUpperPunchReflectCeilingPower(const PlayerTrigger*, const PlayerConst*);
void setupLongJumpVelocity(al::LiveActor*, const IUsePlayerCollision*, f32, f32, f32, f32, f32);
void calcJumpInertia(sead::Vector3f*, al::LiveActor*, const IUsePlayerCollision*,
                     const sead::Vector3f&, f32);
void controlDirectionalVelocity(sead::Vector3f*, const al::LiveActor*, const sead::Vector3f&, f32,
                                f32, f32, f32, f32);
void slerpUpFront(al::LiveActor*, const sead::Vector3f&, const sead::Vector3f&, f32, f32);
void slerpUp(al::LiveActor*, const sead::Vector3f&, f32, f32);
void slerpSkyPoseAir(al::LiveActor*, const sead::Vector3f&, const sead::Vector3f&, f32);
void slerpSkyPoseAirSnapSide(al::LiveActor*, const sead::Vector3f&, const sead::Vector3f&, f32,
                             f32);
void slerpGroundStandFront(al::LiveActor*, const IUsePlayerCollision*, const sead::Vector3f&, f32,
                           f32);
void slerpGroundStand(al::LiveActor*, const IUsePlayerCollision*, f32, f32);
void slerpGravity(al::LiveActor*, f32);
void slerpGravityVelH(al::LiveActor*, f32);
void calcOffsetAllRoot(sead::Vector3f*, const PlayerModelHolder*);
void calcLocalOffsetAllRoot(sead::Vector3f*, const PlayerModelHolder*);
void calcSpineSideDir(sead::Vector3f*, const PlayerModelHolder*);
void startCenterJointControl(PlayerJointParamCenterDynamics*, const al::LiveActor*,
                             const IUsePlayerCollision*, const sead::Vector3f&, bool);
bool isPlayerSideFaceToCameraZ(const al::LiveActor*);
bool tryCalcEffectMtxWaterIn(sead::Matrix34f*, f32*, const al::LiveActor*,
                             const al::WaterSurfaceFinder*, bool, f32);
bool tryCalcEffectMtxDoubleCheckWaterIn(sead::Matrix34f*, f32*, const al::LiveActor*,
                                        const al::WaterSurfaceFinder*, bool, f32);
bool tryCalcEffectMtxWaterOut(sead::Matrix34f*, f32*, const al::LiveActor*,
                              const al::WaterSurfaceFinder*, bool, f32);
bool tryCalcEffectMtxDoubleCheckWaterOut(sead::Matrix34f*, f32*, const al::LiveActor*,
                                         const al::WaterSurfaceFinder*, bool, f32);
void validatePlayerGroundShadow(const PlayerModelHolder*);
void invalidatePlayerGroundShadow(const PlayerModelHolder*);
void setPlayerDirectionalShadowMaskTypeNone(const PlayerModelHolder*);
void setPlayerDirectionalShadowMaskTypeSelf(const PlayerModelHolder*);
bool isTouchDeadCode(const al::LiveActor*, const IUsePlayerCollision*, const IPlayerModelChanger*,
                     const IUseDimension*, f32);
bool isTouchDeadCode3D(const al::LiveActor*, const IUsePlayerCollision*);
bool isTouchDamageCode(const al::LiveActor*, const IUsePlayerCollision*);
bool isTouchDamageFireCode(const al::LiveActor*, const IUsePlayerCollision*,
                           const IPlayerModelChanger*);
bool isTouchDamageFireCode3D(const al::LiveActor*, const IUsePlayerCollision*);
bool isTouchJumpCode(const al::LiveActor*, const IUsePlayerCollision*);
void requestReactionJumpCode(const al::LiveActor*, const IUsePlayerCollision*, al::HitSensor*);
void requestReactionJumpCode(const al::LiveActor*, const char*, al::HitSensor*, al::HitSensor*);
bool calcExistCollisionBorder(const al::IUseCollision*, const sead::Vector3f&,
                              const sead::Vector3f&);
bool calcExistCollisionBorderWallCatch(const al::IUseCollision*, const sead::Vector3f&,
                                       const sead::Vector3f&, const sead::Vector3f&);
void cutVerticalVelocityGroundNormal(al::LiveActor*, const IUsePlayerCollision*);
void brakeLandVelocityGroundNormal(al::LiveActor*, sead::Vector3f*, const IUsePlayerCollision*,
                                   const sead::Vector3f&, f32, f32);
bool calcSlideDir(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&);
bool trySlideVelocityOverGroundNormalAngle(al::LiveActor*, const IUsePlayerCollision*, f32, f32);
void calcJumpInertiaWall(sead::Vector3f*, al::LiveActor*, const IUsePlayerCollision*, f32);
void scaleVelocityInertiaWallHit(al::LiveActor*, const IUsePlayerCollision*, f32, f32, f32);
bool findWallEnablePush(const al::LiveActor*, const IUsePlayerCollision*, f32, f32);
bool findWallCatchPosWallHit(const al::CollisionParts**, sead::Vector3f*, sead::Vector3f*,
                             sead::Vector3f*, const al::LiveActor*, const sead::Vector3f&,
                             const sead::Vector3f&, f32, f32, f32, f32, f32, f32, f32, f32);
bool findWallCatchPos(const al::CollisionParts**, sead::Vector3f*, sead::Vector3f*,
                      const al::LiveActor*, const sead::Vector3f&, const sead::Vector3f&,
                      const sead::Vector3f&, f32, f32, f32, f32, f32, f32, f32);

bool findWallCatchMovePos(const al::CollisionParts**, sead::Vector3f*, sead::Vector3f*,
                          sead::Vector3f*, const al::LiveActor*, const sead::Vector3f&,
                          const sead::Vector3f&, const sead::Vector3f&, f32, f32, f32, f32, f32,
                          f32, f32);

bool findWallClimbPos(const al::CollisionParts**, sead::Vector3f*, sead::Vector3f*,
                      const al::LiveActor*, const sead::Vector3f&, const sead::Vector3f&,
                      const sead::Vector3f&, f32, f32, f32, f32, f32, f32, f32);
bool judgeWallCatchClimb(const al::LiveActor*, const IUsePlayerCollision*,
                         const IUsePlayerCeilingCheck*, const sead::Vector3f&,
                         const sead::Vector3f&, f32, bool, const sead::Vector3f&);
bool findYoshiWallPopUpPos(const al::CollisionParts**, sead::Vector3f*, sead::Vector3f*,
                           const al::LiveActor*, const sead::Vector3f&, const sead::Vector3f&,
                           const sead::Vector3f&, f32, f32, f32, f32, f32, f32, f32);
bool findPoleClimbStartPos(const al::CollisionParts**, sead::Vector3f*, sead::Vector3f*,
                           sead::Vector3f*, f32*, const al::LiveActor*, const al::CollisionParts*,
                           const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&, f32,
                           f32, f32);

bool findPoleClimbMovePos(const al::CollisionParts**, sead::Vector3f*, sead::Vector3f*,
                          sead::Vector3f*, f32*, f32*, const char**, bool*, const al::LiveActor*,
                          const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&, f32,
                          f32, f32, f32, f32, f32);

bool findPoleClimbTurnPos(const al::CollisionParts**, sead::Vector3f*, sead::Vector3f*,
                          sead::Vector3f*, f32*, f32*, const char**, const al::LiveActor*,
                          const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&,
                          const sead::Vector3f&, f32, const sead::Vector3f&, f32, f32, f32, f32,
                          f32);

bool findPoleClimbFromTopPos(const al::CollisionParts**, sead::Vector3f*, sead::Vector3f*,
                             sead::Vector3f*, f32*, f32*, const char**, const al::LiveActor*,
                             const sead::Vector3f&, f32, f32, f32);
bool findGrabCeilPosWallHit(const al::CollisionParts**, sead::Vector3f*, sead::Vector3f*,
                            sead::Vector3f*, const al::LiveActor*, const sead::Vector3f&,
                            const sead::Vector3f&, f32, f32, f32);

bool findGrabCeilPosNoWallHit(const al::CollisionParts**, sead::Vector3f*, sead::Vector3f*,
                              sead::Vector3f*, const al::LiveActor*, const sead::Vector3f&,
                              const sead::Vector3f&, f32, f32, f32);

bool judgeEnableWallKeepHistory(const al::LiveActor*, const PlayerWallActionHistory*,
                                const sead::Vector3f&, const sead::Vector3f&, f32, bool);
bool isInPuddleHeight(const al::WaterSurfaceFinder*, const IUsePlayerHeightCheck*, f32);
bool isUnderWaterSurface(const al::WaterSurfaceFinder*, const IUsePlayerHeightCheck*, f32);
void startHitReactionHipDropLand(al::LiveActor*, bool);
bool convergeOnGroundCount(s32*, const al::LiveActor*, const IUsePlayerCollision*, s32, s32);
void noticePlayerJumpStart(PlayerTrigger*, const al::LiveActor*);
bool tryFindSnapMoveAreaDir(sead::Vector3f*, const al::LiveActor*, const IUsePlayerCollision*);

bool calcSnapVelocitySnapMoveArea(sead::Vector3f*, const al::LiveActor*, const IUsePlayerCollision*,
                                  const sead::Vector3f&, f32);

bool calcSnapVelocitySnapMoveAreaWithCutDir(sead::Vector3f*, const al::LiveActor*,
                                            const IUsePlayerCollision*, const sead::Vector3f&, f32,
                                            const sead::Vector3f&);
bool calcSnapTransSnapMoveAreaNoCollider(sead::Vector3f*, const al::LiveActor*, bool,
                                         const sead::Vector3f&, const sead::Vector3f&, f32);
bool tryStartHeadSlidingLandSubAnim(PlayerAnimator*, const PlayerTrigger*, f32, f32);
bool tryEndHeadSlidingLandSubAnim(PlayerAnimator*, f32);
bool tryCancelHeadSlidingLandSubAnim(PlayerAnimator*);
bool isSubAnimHeadSlidingLand(PlayerAnimator*);
bool tryStartHipDropCancelHeadSlidingSubAnim(PlayerAnimator*, f32);
bool tryCancelHipDropCancelHeadSlidingSubAnim(PlayerAnimator*);
bool updateCollidedWallVibration(al::LiveActor*, const IUsePlayerCollision*, bool, const char*, f32,
                                 f32);
void calcLookAtSensorTargetPos(sead::Vector3f*, al::HitSensor*);
bool tryCalcLookAtTargetInDistance(sead::Vector3f*, al::HitSensor*, const sead::Vector3f&, f32);
void moveInertiaSlide(sead::Vector3f*, al::LiveActor*, const IUsePlayerCollision*,
                      const sead::Vector3f&, f32, f32, f32, f32, f32, f32, f32);
void moveInertiaSlideOnSkate(sead::Vector3f*, al::LiveActor*, const IUsePlayerCollision*,
                             const sead::Vector3f&, f32, f32, f32, f32, f32, f32, f32);
void moveInertiaTurn(sead::Vector3f*, sead::Quatf*, al::LiveActor*, const IUsePlayerCollision*,
                     const sead::Vector3f&, f32, f32, f32, f32, f32);
void moveDivingJump(al::LiveActor*, const sead::Vector3f&, f32, f32, f32, f32, f32, f32, f32, f32);
void moveParallelJump(al::LiveActor*, const sead::Vector3f&, f32, f32, f32, f32, f32, f32, f32);
void moveDiveSwim(al::LiveActor*, f32, f32, f32);
void waitGround(al::LiveActor*, const IUsePlayerCollision*, f32, f32, f32, f32);
bool tryFollowRotateFrontAxisUpIfCollidedGround(al::LiveActor*, const IUsePlayerCollision*);
void landGroundPoseAndSnap(al::LiveActor*, PlayerTrigger*, f32*, const IUsePlayerCollision*,
                           const PlayerConst*);
f32 moveBrakeRun(f32*, sead::Vector3f*, al::LiveActor*, PlayerActionGroundMoveControl*, f32, s32,
                 f32, f32, f32);

}  // namespace rs
