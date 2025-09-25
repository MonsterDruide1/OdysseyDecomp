#pragma once

#include <math/seadVector.h>

namespace al {
class CollisionParts;
class HitSensor;
class LiveActor;
class IUseCollision;
}  // namespace al
class PlayerWallActionHistory;
class IUsePlayerCollision;
class PlayerConst;
class PlayerModelHolder;
class PlayerInput;
class PlayerTrigger;

namespace rs {

void reflectCeiling(al::LiveActor*, f32);

void reflectCeilingUpperPunch(al::LiveActor*, const IUsePlayerCollision*, const PlayerInput*,
                              const PlayerConst*, const PlayerTrigger*, bool);

void setupLongJumpVelocity(al::LiveActor*, const IUsePlayerCollision*, f32, f32, f32, f32, f32);

void noticePlayerJumpStart(PlayerTrigger*, const al::LiveActor*);

bool trySendMsgPlayerReflectOrTrample(const al::LiveActor*, al::HitSensor*, al::HitSensor*);

bool findWallCatchPos(const al::CollisionParts**, sead::Vector3f*, sead::Vector3f*,
                      const al::LiveActor*, const sead::Vector3f&, const sead::Vector3f&,
                      const sead::Vector3f&, f32, f32, f32, f32, f32, f32, f32);
bool judgeEnableWallKeepHistory(const al::LiveActor*, const PlayerWallActionHistory*,
                                const sead::Vector3f&, const sead::Vector3f&, f32, bool);
bool calcExistCollisionBorder(const al::IUseCollision*, const sead::Vector3f&,
                              const sead::Vector3f&);

bool findWallCatchPosWallHit(const al::CollisionParts**, sead::Vector3f*, sead::Vector3f*,
                             sead::Vector3f*, const al::LiveActor*, const sead::Vector3f&,
                             const sead::Vector3f&, f32, f32, f32, f32, f32, f32, f32, f32);

bool findGrabCeilPosNoWallHit(const al::CollisionParts**, sead::Vector3f*, sead::Vector3f*,
                              sead::Vector3f*, const al::LiveActor*, const sead::Vector3f&,
                              const sead::Vector3f&, f32, f32, f32);

bool findGrabCeilPosWallHit(const al::CollisionParts**, sead::Vector3f*, sead::Vector3f*,
                            sead::Vector3f*, const al::LiveActor*, const sead::Vector3f&,
                            const sead::Vector3f&, f32, f32, f32);

void brakeLandVelocityGroundNormal(al::LiveActor*, sead::Vector3f*, const IUsePlayerCollision*,
                                   const sead::Vector3f&, f32, f32);

void faceToCamera(al::LiveActor*);

void slerpUp(al::LiveActor*, const sead::Vector3f&, f32, f32);

void calcOffsetAllRoot(sead::Vector3f* offset, const PlayerModelHolder* model);

bool convergeOnGroundCount(s32*, const al::LiveActor*, const IUsePlayerCollision*, s32, s32);

bool isOnGroundRunAngle(const al::LiveActor*, const IUsePlayerCollision*, const PlayerConst*);

void startHitReactionHipDropLand(al::LiveActor*, bool);

void waitGround(al::LiveActor*, const IUsePlayerCollision*, f32, f32, f32, f32);

void slerpUpFront(al::LiveActor*, const sead::Vector3f&, const sead::Vector3f&, f32, f32);

bool calcSlideDir(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&);

void moveDivingJump(al::LiveActor*, const sead::Vector3f&, f32, f32, f32, f32, f32, f32, f32, f32);

void sendPlayerCollisionTouchMsg(const al::LiveActor*, al::HitSensor*, const IUsePlayerCollision*);

bool calcAlongSkyFront(sead::Vector3f*, const al::LiveActor*);

void calcGroundNormalOrUpDir(sead::Vector3f*, const al::LiveActor*, const IUsePlayerCollision*);

void moveInertiaSlide(sead::Vector3f*, al::LiveActor*, const IUsePlayerCollision*,
                      const sead::Vector3f&, f32, f32, f32, f32, f32, f32, f32);
void moveInertiaSlideOnSkate(sead::Vector3f*, al::LiveActor*, const IUsePlayerCollision*,
                             const sead::Vector3f&, f32, f32, f32, f32, f32, f32, f32);
}  // namespace rs
