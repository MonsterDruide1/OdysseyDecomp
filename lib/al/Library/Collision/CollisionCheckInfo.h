#pragma once

#include <math/seadBoundBox.h>
#include <math/seadVector.h>

namespace al {
class CollisionPartsFilterBase;
class TriangleFilterBase;

struct CollisionCheckInfoBase {
public:
    CollisionCheckInfoBase(const sead::Vector3f& pos,
                           const al::CollisionPartsFilterBase* collisionFilter,
                           const al::TriangleFilterBase* triFilter)
        : mPos(pos), mCollisionPartsFilter(collisionFilter), mTriFilterBase(triFilter) {}

    CollisionCheckInfoBase(const sead::Vector3f& pos) : mPos(pos) {}

    const sead::Vector3f& mPos;
    const al::CollisionPartsFilterBase* mCollisionPartsFilter = nullptr;
    const al::TriangleFilterBase* mTriFilterBase = nullptr;
};

struct SphereCheckInfo : public CollisionCheckInfoBase {
public:
    SphereCheckInfo(const sead::Vector3f& pos, const al::CollisionPartsFilterBase* collisionFilter,
                    const al::TriangleFilterBase* triFilter, f32 radius)
        : CollisionCheckInfoBase(pos, collisionFilter, triFilter), mRadius(radius) {}

    SphereCheckInfo(const sead::Vector3f& pos, f32 radius)
        : CollisionCheckInfoBase(pos), mRadius(radius) {}

    f32 mRadius;
};

struct ArrowCheckInfo : public CollisionCheckInfoBase {
public:
    ArrowCheckInfo(const sead::Vector3f& pos, const sead::Vector3f* u1)
        : CollisionCheckInfoBase(pos), unk1(u1) {
        unk2.setAdd(pos, *u1);
        unk3.addPoint(pos);
        unk3.addPoint(unk2);
    }

    const sead::Vector3f* unk1;
    sead::Vector3f unk2;
    sead::BoundBox3f unk3;
};

struct DiskCheckInfo : public CollisionCheckInfoBase {
public:
    DiskCheckInfo(const sead::Vector3f& pos, f32 unk1, f32 unk2, const sead::Vector3f& unk3);

    f32 unk1;
    f32 unk2;
    const sead::Vector3f& unk3;
    f32 unk4 = 0.0f;
};

}  // namespace al
