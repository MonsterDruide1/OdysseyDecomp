#include "Player/CollidedShapeResult.h"
#include "Util/CollisionShapeFunction.h"

// NON_MATCHING: Should be call to `memset` for all HitInfo members first, then constructor call
CollidedShapeResult::CollidedShapeResult(const CollisionShapeInfoBase* shapeInfoBase) : mShapeInfoBase(shapeInfoBase) {}

void CollidedShapeResult::setArrowHitInfo(const al::ArrowHitInfo& hitInfo) {
    mArrowHitInfo = hitInfo;
}
void CollidedShapeResult::setSphereHitInfo(const al::SphereHitInfo& hitInfo) {
    mSphereHitInfo = hitInfo;
}
void CollidedShapeResult::setDiskHitInfo(const al::DiskHitInfo& hitInfo) {
    mDiskHitInfo = hitInfo;
}

bool CollidedShapeResult::isArrow() const {
    return CollisionShapeFunction::isShapeArrow(mShapeInfoBase);
}
bool CollidedShapeResult::isSphere() const {
    return CollisionShapeFunction::isShapeSphere(mShapeInfoBase);
}
bool CollidedShapeResult::isDisk() const {
    return CollisionShapeFunction::isShapeDisk(mShapeInfoBase);
}

const al::ArrowHitInfo& CollidedShapeResult::getArrowHitInfo() const {
    return mArrowHitInfo;
}
const al::SphereHitInfo& CollidedShapeResult::getSphereHitInfo() const {
    return mSphereHitInfo;
}
const al::DiskHitInfo& CollidedShapeResult::getDiskHitInfo() const {
    return mDiskHitInfo;
}

const CollisionShapeInfoArrow* CollidedShapeResult::getShapeInfoArrow() const {
    return CollisionShapeFunction::getShapeInfoArrow(mShapeInfoBase);
}
const CollisionShapeInfoSphere* CollidedShapeResult::getShapeInfoSphere() const {
    return CollisionShapeFunction::getShapeInfoSphere(mShapeInfoBase);
}
const CollisionShapeInfoDisk* CollidedShapeResult::getShapeInfoDisk() const {
    return CollisionShapeFunction::getShapeInfoDisk(mShapeInfoBase);
}

void CollidedShapeResult::operator=(const CollidedShapeResult& other) {
    mShapeInfoBase = other.mShapeInfoBase;
    mArrowHitInfo = other.mArrowHitInfo;
    mSphereHitInfo = other.mSphereHitInfo;
    mDiskHitInfo = other.mDiskHitInfo;
}
