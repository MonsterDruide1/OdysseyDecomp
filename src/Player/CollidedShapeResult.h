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
    sead::StorageFor<al::ArrowHitInfo> mArrowHitInfo{sead::ZeroInitializeTag{}};
    sead::StorageFor<al::SphereHitInfo> mSphereHitInfo{sead::ZeroInitializeTag{}};
    sead::StorageFor<al::DiskHitInfo> mDiskHitInfo{sead::ZeroInitializeTag{}};
};

static_assert(sizeof(CollidedShapeResult) == 0x1e8);
