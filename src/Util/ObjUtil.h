#pragma once

#include <math/seadVector.h>

namespace al {
class CollisionParts;
class HitSensor;
class LiveActor;
}  // namespace al

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

void slerpUpFront(al::LiveActor*, const sead::Vector3f&, const sead::Vector3f&, f32, f32);
bool calcSlideDir(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&);
}  // namespace rs
