#pragma once

#include "Library/Collision/KCollisionFunc.h"

class CollisionShapeInfoBase;
class CollisionShapeInfoArrow;
class CollisionShapeInfoSphere;
class CollisionShapeInfoDisk;

class CollidedShapeResult {
public:
    CollidedShapeResult(const CollisionShapeInfoBase*);

    void setArrowHitInfo(const al::ArrowHitInfo&);
    void setSphereHitInfo(const al::SphereHitInfo&);
    void setDiskHitInfo(const al::DiskHitInfo&);

    bool isArrow() const;
    bool isSphere() const;
    bool isDisk() const;

    const al::ArrowHitInfo& getArrowHitInfo() const;
    const al::SphereHitInfo& getSphereHitInfo() const;
    const al::DiskHitInfo& getDiskHitInfo() const;

    const CollisionShapeInfoArrow* getShapeInfoArrow() const;
    const CollisionShapeInfoSphere* getShapeInfoSphere() const;
    const CollisionShapeInfoDisk* getShapeInfoDisk() const;

    void operator=(const CollidedShapeResult&);
private:
    const CollisionShapeInfoBase* mShapeInfoBase;
    al::ArrowHitInfo mArrowHitInfo;
    al::SphereHitInfo mSphereHitInfo;
    al::DiskHitInfo mDiskHitInfo;
};
static_assert(sizeof(CollidedShapeResult) == 0x1e8);
