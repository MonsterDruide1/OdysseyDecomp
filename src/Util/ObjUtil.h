#pragma once

#include <math/seadVector.h>

namespace al {
class CollisionParts;
class LiveActor;
class IUseCollision;
}  // namespace al
class PlayerWallActionHistory;

namespace rs {

bool findWallCatchPos(const al::CollisionParts**, sead::Vector3f*, sead::Vector3f*,
                      const al::LiveActor*, const sead::Vector3f&, const sead::Vector3f&,
                      const sead::Vector3f&, f32, f32, f32, f32, f32, f32, f32);
bool judgeEnableWallKeepHistory(const al::LiveActor*, const PlayerWallActionHistory*,
                                const sead::Vector3f&, const sead::Vector3f&, f32, bool);
bool calcExistCollisionBorder(const al::IUseCollision*, const sead::Vector3f&,
                              const sead::Vector3f&);

bool findGrabCeilPosNoWallHit(const al::CollisionParts**, sead::Vector3f*, sead::Vector3f*,
                              sead::Vector3f*, const al::LiveActor*, const sead::Vector3f&,
                              const sead::Vector3f&, f32, f32, f32);

bool findGrabCeilPosWallHit(const al::CollisionParts**, sead::Vector3f*, sead::Vector3f*,
                            sead::Vector3f*, const al::LiveActor*, const sead::Vector3f&,
                            const sead::Vector3f&, f32, f32, f32);

}  // namespace rs
