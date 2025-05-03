#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/HostIO/HioNode.h"

enum class CollisionShapeId : u32 {
    Arrow,
    Sphere,
    Disk,
};

class CollisionShapeInfoBase : public al::HioNode {
public:
    CollisionShapeInfoBase(CollisionShapeId, const char*);

    virtual const sead::Vector3f& getBoundingCenter() const { return sead::Vector3f::zero; }

    virtual const sead::Vector3f& getBoundingCenterWorld() const { return sead::Vector3f::zero; }

    virtual f32 getBoundingRadius() const { return 0.0f; }

    virtual f32 getBoundingRadiusWorld() const { return 0.0f; }

    virtual f32 getCheckStepRange() const { return 100000.0f; }

    virtual f32 getCheckStepRangeWorld() const { return 100000.0f; }

    virtual void updateShapeOffset(const sead::Vector3f&) {}

    virtual void calcWorldShapeInfo(const sead::Matrix34f&, f32) {}

    virtual void calcRelativeShapeInfo(const sead::Matrix34f&) {}

    CollisionShapeId getId() const { return mId; }

    const char* getName() const { return mName; }

private:
    CollisionShapeId mId;
    const char* mName;
};
