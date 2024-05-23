#pragma once

#include <math/seadVector.h>
#include <math/seadBoundBox.h>
#include "basis/seadTypes.h"

namespace al {
class CollisionPartsFilterBase;
class TriangleFilterBase;

struct CollisionCheckInfoBase {
public:
    CollisionCheckInfoBase(const sead::Vector3f& pos, const al::CollisionPartsFilterBase* collisionFilter, const al::TriangleFilterBase* triFilter)
        : mPos(pos), mCollisionPartsFilter(collisionFilter), mTriFilterBase(triFilter) {}

    CollisionCheckInfoBase(const sead::Vector3f& pos)
        : mPos(pos) {}


    const sead::Vector3f& mPos;
    const al::CollisionPartsFilterBase* mCollisionPartsFilter = nullptr;
    const al::TriangleFilterBase* mTriFilterBase = nullptr;
};

struct SphereCheckInfo : public CollisionCheckInfoBase {
public:
    SphereCheckInfo(const sead::Vector3f& pos, const al::CollisionPartsFilterBase* collisionFilter, const al::TriangleFilterBase* triFilter, f32 radius)
        : CollisionCheckInfoBase(pos, collisionFilter, triFilter), mRadius(radius) {}
    
    SphereCheckInfo(const sead::Vector3f& pos, f32 radius)
        : CollisionCheckInfoBase(pos), mRadius(radius) {}

    
    f32 mRadius;
};

struct ArrowCheckInfo : public CollisionCheckInfoBase {
public:
    ArrowCheckInfo(const sead::Vector3f& pos, const sead::Vector3f& unk1) : CollisionCheckInfoBase(pos), unk1(unk1), unk2(pos+unk1), unk3() {
        // TODO does this differently, somehow incremental
        unk3.set(pos.x, pos.y, pos.z, unk2.x, unk2.y, unk2.z);
    }

    const sead::Vector3f& unk1;
    sead::Vector3f unk2;
    sead::BoundBox3f unk3;
};

struct DiskCheckInfo : public CollisionCheckInfoBase {
public:
    // TODO make this a function being generated, not inlined
    DiskCheckInfo(const sead::Vector3f& pos, f32 unk1, f32 unk2, const sead::Vector3f& unk3) : CollisionCheckInfoBase(pos), unk1(unk1), unk2(unk2), unk3(unk3) {
        if(unk1 <= unk2)
            unk4 = sead::Mathf::sqrt(sead::Mathf::square(unk2) + sead::Mathf::square(unk1));
        else
            unk4 = unk1 * 1.4142f;
    }


    f32 unk1;
    f32 unk2;
    const sead::Vector3f& unk3;
    f32 unk4 = 0.0f;
};


}  // namespace al
