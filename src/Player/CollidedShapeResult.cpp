#include "Player/CollidedShapeResult.h"
#include "Library/Collision/KCollisionServer.h"
#include "Util/CollisionShapeFunction.h"

#include <cstdio>

// NON_MATCHING: Should be call to `memset` for all HitInfo members first, then constructor call
CollidedShapeResult::CollidedShapeResult(const CollisionShapeInfoBase* shapeInfoBase) : mShapeInfoBase(shapeInfoBase) {}

void CollidedShapeResult::setArrowHitInfo(const al::ArrowHitInfo& hitInfo) {
    printf("CollidedShapeResult::setArrowHitInfo{tri.index=%d, unk=%f, mCollisionHitPos=(%f, %f, %f), unk3=(%f, %f, %f), mCollisionMovingReaction=(%f, %f, %f), mCollisionLocation=%d}\n",
           hitInfo.mTriangle.mKCPrismData->mTriIndex, hitInfo.unk, hitInfo.mCollisionHitPos.x, hitInfo.mCollisionHitPos.y, hitInfo.mCollisionHitPos.z,
           hitInfo.unk3.x, hitInfo.unk3.y, hitInfo.unk3.z, hitInfo.mCollisionMovingReaction.x, hitInfo.mCollisionMovingReaction.y, hitInfo.mCollisionMovingReaction.z, hitInfo.mCollisionLocation);
    mArrowHitInfo = hitInfo;
}
void CollidedShapeResult::setSphereHitInfo(const al::SphereHitInfo& hitInfo) {
    printf("CollidedShapeResult::setSphereHitInfo{tri.index=%d, unk=%f, mCollisionHitPos=(%f, %f, %f), unk3=(%f, %f, %f), mCollisionMovingReaction=(%f, %f, %f), mCollisionLocation=%d}\n",
           hitInfo.mTriangle.mKCPrismData->mTriIndex, hitInfo.unk, hitInfo.mCollisionHitPos.x, hitInfo.mCollisionHitPos.y, hitInfo.mCollisionHitPos.z,
           hitInfo.unk3.x, hitInfo.unk3.y, hitInfo.unk3.z, hitInfo.mCollisionMovingReaction.x, hitInfo.mCollisionMovingReaction.y, hitInfo.mCollisionMovingReaction.z, hitInfo.mCollisionLocation);
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
