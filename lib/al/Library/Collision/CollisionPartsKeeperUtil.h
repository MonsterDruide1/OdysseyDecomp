#pragma once

#include <math/seadVector.h>

namespace al {
class CollisionPartsFilterBase;
class IUseCollision;
class Triangle;
class TriangleFilterBase;
struct HitInfo;
struct ArrowHitInfo;

bool isFloorCode(const Triangle&, const char*);
}  // namespace al

namespace alCollisionUtil {
bool getFirstPolyOnArrow(const al::IUseCollision*, sead::Vector3f*, al::Triangle*,
                         const sead::Vector3f&, const sead::Vector3f&,
                         const al::CollisionPartsFilterBase*, const al::TriangleFilterBase*);
bool getHitPosAndNormalOnArrow(const al::IUseCollision*, sead::Vector3f*, sead::Vector3f*,
                               const sead::Vector3f&, const sead::Vector3f&,
                               const al::CollisionPartsFilterBase*, const al::TriangleFilterBase*);
bool getHitPosOnArrow(const al::IUseCollision*, sead::Vector3f*, const sead::Vector3f&,
                      const sead::Vector3f&, const al::CollisionPartsFilterBase*,
                      const al::TriangleFilterBase*);
s32 checkStrikeArrow(const al::IUseCollision*, const sead::Vector3f& origin,
                     const sead::Vector3f& delta, const al::CollisionPartsFilterBase*,
                     const al::TriangleFilterBase*);

const sead::Vector3f& getCollisionHitPos(const al::HitInfo*);
const sead::Vector3f& getCollisionHitNormal(const al::HitInfo*);

bool getLastPolyOnArrow(const al::IUseCollision*, const al::ArrowHitInfo**, const sead::Vector3f&,
                        const sead::Vector3f&, const al::CollisionPartsFilterBase*,
                        const al::TriangleFilterBase*);
}  // namespace alCollisionUtil
