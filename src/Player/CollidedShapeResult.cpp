#include "Player/CollidedShapeResult.h"

#include "Util/CollisionShapeFunction.h"

CollidedShapeResult::CollidedShapeResult(const CollisionShapeInfoBase* shapeInfo)
    : mShapeInfo(shapeInfo) {}

void CollidedShapeResult::setArrowHitInfo(const al::ArrowHitInfo& arrowHitInfo) {
    *mArrowHitInfo = arrowHitInfo;
}

void CollidedShapeResult::setSphereHitInfo(const al::SphereHitInfo& sphereHitInfo) {
    *mSphereHitInfo = sphereHitInfo;
}

void CollidedShapeResult::setDiskHitInfo(const al::DiskHitInfo& diskHitInfo) {
    *mDiskHitInfo = diskHitInfo;
}

bool CollidedShapeResult::isArrow() const {
    return CollisionShapeFunction::isShapeArrow(mShapeInfo);
}

bool CollidedShapeResult::isSphere() const {
    return CollisionShapeFunction::isShapeSphere(mShapeInfo);
}

bool CollidedShapeResult::isDisk() const {
    return CollisionShapeFunction::isShapeDisk(mShapeInfo);
}

const al::ArrowHitInfo& CollidedShapeResult::getArrowHitInfo() const {
    return *mArrowHitInfo;
}

const al::SphereHitInfo& CollidedShapeResult::getSphereHitInfo() const {
    return *mSphereHitInfo;
}

const al::DiskHitInfo& CollidedShapeResult::getDiskHitInfo() const {
    return *mDiskHitInfo;
}

const CollisionShapeInfoArrow* CollidedShapeResult::getShapeInfoArrow() const {
    return CollisionShapeFunction::getShapeInfoArrow(mShapeInfo);
}

const CollisionShapeInfoSphere* CollidedShapeResult::getShapeInfoSphere() const {
    return CollisionShapeFunction::getShapeInfoSphere(mShapeInfo);
}

const CollisionShapeInfoDisk* CollidedShapeResult::getShapeInfoDisk() const {
    return CollisionShapeFunction::getShapeInfoDisk(mShapeInfo);
}

void CollidedShapeResult::operator=(const CollidedShapeResult& other) {
    mShapeInfo = other.mShapeInfo;
    *mArrowHitInfo = *other.mArrowHitInfo;
    *mSphereHitInfo = *other.mSphereHitInfo;
    *mDiskHitInfo = *other.mDiskHitInfo;
}
