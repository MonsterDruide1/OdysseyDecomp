#pragma once

#include <math/seadVector.h>

namespace al {
class CollisionPartsFilterBase;
class IUseCollision;
class Triangle;
class TriangleFilterBase;
class SphereHitInfo;

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
u32 getStrikeSphereInfoNum(const al::IUseCollision* collision);
al::SphereHitInfo* getStrikeSphereInfo(const al::IUseCollision* collision, u32 index);
s32 checkStrikeSphereMovingReaction(const al::IUseCollision*, const sead::Vector3f&, f32,
                                     const sead::Vector3f&, const al::CollisionPartsFilterBase*,
                                     const al::TriangleFilterBase*);
s32 checkStrikeSphere(const al::IUseCollision*, const sead::Vector3f&, f32,
                       const al::CollisionPartsFilterBase*, const al::TriangleFilterBase*);
}  // namespace alCollisionUtil
