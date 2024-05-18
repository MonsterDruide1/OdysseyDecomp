#pragma once

#include <math/seadVector.h>

namespace al {
class CollisionPartsFilterBase;
class TriangleFilterBase;
class CollisionParts;

struct CollisionCheckInfoBase {
    const sead::Vector3f* mPos;
    const CollisionPartsFilterBase* mCollisionPartsFilter;
    const TriangleFilterBase* mTriFilterBase;
};

struct SphereCheckInfo : public al::CollisionCheckInfoBase {
    f32 mRadius;
};

}  // namespace al

namespace alCollisionUtil {

bool isFarAway(const al::CollisionParts&, const sead::Vector3f&, f32);

}
