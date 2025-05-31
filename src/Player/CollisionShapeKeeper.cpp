#include "Player/CollisionShapeKeeper.h"

#include "Library/Math/MathUtil.h"

#include "Player/CollidedShapeResult.h"
#include "Player/CollisionShapeInfo.h"
#include "Util/CollisionShapeFunction.h"

CollisionShapeKeeper::CollisionShapeKeeper(s32 maxShapes, s32 maxCollideResults,
                                           s32 maxCollideSupportResults) {
    mCollisionShape.allocBuffer(maxShapes, nullptr);

    if (maxCollideResults >= 1) {
        mCollideShapeResult.allocBuffer(maxCollideResults, nullptr);
        for (s32 i = 0; i < maxCollideResults; i++)
            mCollideShapeResult.pushBack(new CollidedShapeResult(nullptr));
    }

    if (maxCollideSupportResults >= 1) {
        mCollideSupportResult.allocBuffer(maxCollideSupportResults, nullptr);
        for (s32 i = 0; i < maxCollideSupportResults; i++)
            mCollideSupportResult.pushBack(new CollidedShapeResult(nullptr));
    }
}

void CollisionShapeKeeper::createShapeArrow(const char* name, const sead::Vector3f& start,
                                            const sead::Vector3f& end, f32 unk1, s32 unk2) {
    mCollisionShape.pushBack(new CollisionShapeInfoArrow(name, start, end, unk1, unk2));
    mHasShapeArrow = true;
}

void CollisionShapeKeeper::createShapeSphere(const char* name, f32 radius,
                                             const sead::Vector3f& position) {
    mCollisionShape.pushBack(new CollisionShapeInfoSphere(name, radius, position));
}

void CollisionShapeKeeper::createShapeSphereSupportGround(const char* name, f32 radius,
                                                          const sead::Vector3f& pos,
                                                          const sead::Vector3f& up, f32 unk1) {
    auto* info = new CollisionShapeInfoSphere(name, radius, pos);
    info->setSupportGround(up, unk1);
    mCollisionShape.pushBack(info);
}

void CollisionShapeKeeper::createShapeSphereIgnoreGround(const char* name, f32 radius,
                                                         const sead::Vector3f& pos) {
    auto* info = new CollisionShapeInfoSphere(name, radius, pos);
    info->setIgnoreGround();
    mCollisionShape.pushBack(info);
}

void CollisionShapeKeeper::createShapeDisk(const char* name, f32 a1, const sead::Vector3f& a2,
                                           const sead::Vector3f& a3, f32 a4) {
    mCollisionShape.pushBack(new CollisionShapeInfoDisk(name, a1, a2, a3, a4));
}

void CollisionShapeKeeper::createShapeDiskSupportGround(const char* name, f32 a1,
                                                        const sead::Vector3f& a2,
                                                        const sead::Vector3f& a3, f32 a4,
                                                        const sead::Vector3f& a5, f32 a6) {
    auto* info = new CollisionShapeInfoDisk(name, a1, a2, a3, a4);
    info->setSupportGround(a5, a6);
    mCollisionShape.pushBack(info);
}

void CollisionShapeKeeper::createShapeDiskIgnoreGround(const char* name, f32 a1,
                                                       const sead::Vector3f& a2,
                                                       const sead::Vector3f& a3, f32 a4) {
    auto* info = new CollisionShapeInfoDisk(name, a1, a2, a3, a4);
    info->setIgnoreGround();
    mCollisionShape.pushBack(info);
}

void CollisionShapeKeeper::updateShape() {
    mBoundingCenter = {0.0f, 0.0f, 0.0f};
    mBoundingRadius = 0.0f;

    s32 numShapes = mCollisionShape.size();
    for (s32 i = 0; i < numShapes; i++) {
        const sead::Vector3f& center = mCollisionShape[i]->getBoundingCenter();

        if (i == 0) {
            mBoundingCenter = center;
            mBoundingRadius = mCollisionShape[i]->getBoundingRadius();
            continue;
        }

        f32 radius = mCollisionShape[i]->getBoundingRadius();
        sead::Vector3f centerOff = center - mBoundingCenter;
        f32 centerDist = centerOff.length();
        if (al::isNearZero(centerDist, 0.001f)) {
            if (radius > mBoundingRadius)
                mBoundingRadius = radius;
        } else if (radius + centerDist > mBoundingRadius) {
            sead::Vector3f offsetDirection = centerOff;
            al::normalize(&offsetDirection);
            sead::Vector3f newPos = radius * offsetDirection + center;
            sead::Vector3f oldOppositePos = mBoundingCenter - offsetDirection * mBoundingRadius;
            sead::Vector3f fullOffsetWidth = newPos - oldOppositePos;
            mBoundingCenter = oldOppositePos + fullOffsetWidth * 0.5f;
            mBoundingRadius = fullOffsetWidth.length() * 0.5f;
        }
    }

    mCheckStepRange = mBoundingRadius;

    for (s32 i = 0; i < numShapes; i++) {
        mCheckStepRange =
            sead::Mathf::min(mCheckStepRange, mCollisionShape[i]->getCheckStepRange());
    }
}

void CollisionShapeKeeper::clearResult() {
    mNumCollideResult = 0;
    mNumCollideSupportResult = 0;
}

void CollisionShapeKeeper::calcWorldShapeInfo(const sead::Matrix34f& a2, f32 a3) {
    for (auto& shape : mCollisionShape)
        shape.calcWorldShapeInfo(a2, a3);
}

void CollisionShapeKeeper::calcRelativeShapeInfo(const sead::Matrix34f& a2) {
    for (auto& shape : mCollisionShape)
        shape.calcRelativeShapeInfo(a2);
}

void CollisionShapeKeeper::registerCollideResult(const CollidedShapeResult& result) {
    *mCollideShapeResult[mNumCollideResult] = result;
    mNumCollideResult++;
}

void CollisionShapeKeeper::registerCollideSupportResult(const CollidedShapeResult& result) {
    *mCollideSupportResult[mNumCollideSupportResult] = result;
    mNumCollideSupportResult++;
}

bool CollisionShapeKeeper::isCollidedResultFull() const {
    return mNumCollideResult >= mCollideShapeResult.capacity();
}

bool CollisionShapeKeeper::isCollidedSupportResultFull() const {
    return mNumCollideSupportResult >= mCollideSupportResult.capacity();
}

bool CollisionShapeKeeper::isShapeArrow(s32 index) const {
    return CollisionShapeFunction::isShapeArrow(getShapeInfoBase(index));
}

bool CollisionShapeKeeper::isShapeSphere(s32 index) const {
    return CollisionShapeFunction::isShapeSphere(getShapeInfoBase(index));
}

bool CollisionShapeKeeper::isShapeDisk(s32 index) const {
    return CollisionShapeFunction::isShapeDisk(getShapeInfoBase(index));
}

const CollisionShapeInfoBase* CollisionShapeKeeper::getShapeInfoBase(s32 index) const {
    return mCollisionShape[index];
}

const CollisionShapeInfoArrow* CollisionShapeKeeper::getShapeInfoArrow(s32 index) const {
    return CollisionShapeFunction::getShapeInfoArrow(mCollisionShape[index]);
}

const CollisionShapeInfoSphere* CollisionShapeKeeper::getShapeInfoSphere(s32 index) const {
    return CollisionShapeFunction::getShapeInfoSphere(mCollisionShape[index]);
}

const CollisionShapeInfoDisk* CollisionShapeKeeper::getShapeInfoDisk(s32 index) const {
    return CollisionShapeFunction::getShapeInfoDisk(mCollisionShape[index]);
}

u32 CollisionShapeKeeper::findShapeInfoIndex(const char* name) const {
    s32 numShapes = mCollisionShape.size();
    for (s32 i = 0; i < numShapes; i++)
        if (CollisionShapeFunction::isShapeName(mCollisionShape[i], name))
            return i;
    return 0;
}

const CollidedShapeResult* CollisionShapeKeeper::getCollidedShapeResult(s32 index) const {
    return mCollideShapeResult[index];
}

const CollidedShapeResult* CollisionShapeKeeper::getCollidedShapeSupportResult(s32 index) const {
    return mCollideSupportResult[index];
}
