#pragma once

#include <math/seadVector.h>
#include "math/seadBoundBox.h"

namespace al {
class CollisionPartsFilterBase;
class TriangleFilterBase;
class CollisionParts;
class IUseCollision;
class HitInfo;

struct CollisionCheckInfoBase {
    const sead::Vector3f* mPos;
    const CollisionPartsFilterBase* mCollisionPartsFilter;
    const TriangleFilterBase* mTriFilterBase;
};

struct SphereCheckInfo : public al::CollisionCheckInfoBase {
    f32 mRadius;
};

bool isNearCollideSphereAabb(const sead::Vector3f&, f32, const sead::BoundBox3f&);

bool checkHitSegmentSphere(sead::Vector3<float> const&,sead::Vector3<float> const&,sead::Vector3<float> const&,float,sead::Vector3<float>*,sead::Vector3<float>*);

bool isFloorCode(const al::HitInfo*, const char*);

}  // namespace al

namespace alCollisionUtil {

bool isFarAway(const al::CollisionParts&, const sead::Vector3f&, f32);
bool checkStrikeArrow(al::IUseCollision const*,sead::Vector3<float> const&,sead::Vector3<float> const&,al::CollisionPartsFilterBase const*,al::TriangleFilterBase const*);

}
