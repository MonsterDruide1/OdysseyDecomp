#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/HostIO/HioNode.h"

enum class CollisionShapeId : u32 {};

class CollisionShapeInfoBase : public al::HioNode {
public:
    CollisionShapeInfoBase(CollisionShapeId, const char*);

    virtual const sead::Vector3f& getBoundingCenter() const;
    virtual const sead::Vector3f& getBoundingCenterWorld() const;
    virtual f32 getBoundingRadius() const;
    virtual f32 getBoundingRadiusWorld() const;
    virtual f32 getCheckStepRange() const;
    virtual f32 getCheckStepRangeWorld() const;
    virtual void updateShapeOffset(const sead::Vector3f&);
    virtual void calcWorldShapeInfo(const sead::Matrix34f&, f32);
    virtual void calcRelativeShapeInfo(const sead::Matrix34f&);

private:
    CollisionShapeId mId;
    const char* mName;
};
