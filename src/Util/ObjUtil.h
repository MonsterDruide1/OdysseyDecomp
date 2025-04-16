#pragma once

#include <math/seadVector.h>

namespace al {
class CollisionParts;
class HitSensor;
class LiveActor;
}  // namespace al

class IUsePlayerCollision;
class PlayerConst;
class PlayerModelHolder;

namespace rs {

bool trySendMsgPlayerReflectOrTrample(const al::LiveActor*, al::HitSensor*, al::HitSensor*);

bool findWallCatchPos(const al::CollisionParts**, sead::Vector3f*, sead::Vector3f*,
                      const al::LiveActor*, const sead::Vector3f&, const sead::Vector3f&,
                      const sead::Vector3f&, f32, f32, f32, f32, f32, f32, f32);

bool findGrabCeilPosNoWallHit(const al::CollisionParts**, sead::Vector3f*, sead::Vector3f*,
                              sead::Vector3f*, const al::LiveActor*, const sead::Vector3f&,
                              const sead::Vector3f&, f32, f32, f32);

bool findGrabCeilPosWallHit(const al::CollisionParts**, sead::Vector3f*, sead::Vector3f*,
                            sead::Vector3f*, const al::LiveActor*, const sead::Vector3f&,
                            const sead::Vector3f&, f32, f32, f32);

void calcOffsetAllRoot(sead::Vector3f* offset, const PlayerModelHolder* model);

bool convergeOnGroundCount(s32*, const al::LiveActor*, const IUsePlayerCollision*, s32, s32);

bool isOnGroundRunAngle(const al::LiveActor*, const IUsePlayerCollision*, const PlayerConst*);

void startHitReactionHipDropLand(al::LiveActor*, bool);

void waitGround(al::LiveActor*, const IUsePlayerCollision*, f32, f32, f32, f32);

void slerpUpFront(al::LiveActor*, const sead::Vector3f&, const sead::Vector3f&, f32, f32);

bool calcSlideDir(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&);

void moveDivingJump(al::LiveActor*, const sead::Vector3f&, f32, f32, f32, f32, f32, f32, f32, f32);
}  // namespace rs
