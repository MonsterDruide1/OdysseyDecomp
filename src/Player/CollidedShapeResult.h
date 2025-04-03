#pragma once

#include <prim/seadStorageFor.h>

#include "Library/Collision/CollisionPartsTriangle.h"

class CollisionShapeInfoBase;
class CollisionShapeInfoArrow;
class CollisionShapeInfoSphere;
class CollisionShapeInfoDisk;

class CollidedShapeResult {
public:
    CollidedShapeResult(const CollisionShapeInfoBase* shapeInfo);

    void setArrowHitInfo(const al::ArrowHitInfo& arrowHitInfo);
    void setSphereHitInfo(const al::SphereHitInfo& sphereHitInfo);
    void setDiskHitInfo(const al::DiskHitInfo& diskHitInfo);

    bool isArrow() const;
    bool isSphere() const;
    bool isDisk() const;

    const al::ArrowHitInfo& getArrowHitInfo() const;
    const al::SphereHitInfo& getSphereHitInfo() const;
    const al::DiskHitInfo& getDiskHitInfo() const;

    const CollisionShapeInfoArrow* getShapeInfoArrow() const;
    const CollisionShapeInfoSphere* getShapeInfoSphere() const;
    const CollisionShapeInfoDisk* getShapeInfoDisk() const;

    void operator=(const CollidedShapeResult& other);

private:
    const CollisionShapeInfoBase* mShapeInfo;
    al::ArrowHitInfo mArrowHitInfo;
    al::SphereHitInfo mSphereHitInfo;
    al::DiskHitInfo mDiskHitInfo;
};

static_assert(sizeof(CollidedShapeResult) == 0x1e8);
