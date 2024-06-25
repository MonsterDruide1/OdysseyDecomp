#pragma once

#include <math/seadVector.h>

namespace al {
class CollisionParts;
class LiveActor;
}  // namespace al

namespace rs {

bool findWallCatchPos(const al::CollisionParts**, sead::Vector3f*, sead::Vector3f*,
                      const al::LiveActor*, const sead::Vector3f&, const sead::Vector3f&,
                      const sead::Vector3f&, f32, f32, f32, f32, f32, f32, f32);

bool findGrabCeilPosNoWallHit(const al::CollisionParts**, sead::Vector3f*, sead::Vector3f*,
                              sead::Vector3f*, const al::LiveActor*, const sead::Vector3f&,
                              const sead::Vector3f&, f32, f32, f32);

bool findGrabCeilPosWallHit(const al::CollisionParts**, sead::Vector3f*, sead::Vector3f*,
                            sead::Vector3f*, const al::LiveActor*, const sead::Vector3f&,
                            const sead::Vector3f&, f32, f32, f32);

}  // namespace rs
