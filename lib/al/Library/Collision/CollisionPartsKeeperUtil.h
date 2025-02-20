#pragma once

#include <math/seadVector.h>

namespace al {
class CollisionPartsFilterBase;
class IUseCollision;
class Triangle;
class TriangleFilterBase;
}  // namespace al

namespace alCollisionUtil {
bool getFirstPolyOnArrow(const al::IUseCollision*, sead::Vector3f*, al::Triangle*,
                         const sead::Vector3f&, const sead::Vector3f&,
                         const al::CollisionPartsFilterBase*, const al::TriangleFilterBase*);
bool getHitPosAndNormalOnArrow(const al::IUseCollision*, sead::Vector3f*, sead::Vector3f*,
                               const sead::Vector3f&, const sead::Vector3f&,
                               const al::CollisionPartsFilterBase*, const al::TriangleFilterBase*);
}  // namespace alCollisionUtil
