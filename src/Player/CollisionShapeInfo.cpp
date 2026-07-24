#include "Player/CollisionShapeInfo.h"

CollisionShapeInfoBase::CollisionShapeInfoBase(CollisionShapeId id, const char* name)
    : mId(id), mName(name) {}

const sead::Vector3f& CollisionShapeInfoArrow::getBoundingCenter() const {
    return mBoundingCenter;
}

const sead::Vector3f& CollisionShapeInfoArrow::getBoundingCenterWorld() const {
    return mBoundingCenterWorld;
}

f32 CollisionShapeInfoArrow::getBoundingRadius() const {
    return mBoundingRadius;
}

f32 CollisionShapeInfoArrow::getBoundingRadiusWorld() const {
    return mBoundingRadiusWorld;
}

f32 CollisionShapeInfoArrow::getCheckStepRange() const {
    return mBoundingRadius;
}

f32 CollisionShapeInfoArrow::getCheckStepRangeWorld() const {
    return mBoundingRadiusWorld;
}

const sead::Vector3f& CollisionShapeInfoSphere::getBoundingCenter() const {
    return mBoundingCenter;
}

const sead::Vector3f& CollisionShapeInfoSphere::getBoundingCenterWorld() const {
    return mBoundingCenterWorld;
}

f32 CollisionShapeInfoSphere::getBoundingRadius() const {
    return mBoundingRadius;
}

f32 CollisionShapeInfoSphere::getBoundingRadiusWorld() const {
    return mBoundingRadiusWorld;
}

f32 CollisionShapeInfoSphere::getCheckStepRange() const {
    return mBoundingRadius;
}

f32 CollisionShapeInfoSphere::getCheckStepRangeWorld() const {
    return mBoundingRadiusWorld;
}

const sead::Vector3f& CollisionShapeInfoDisk::getBoundingCenter() const {
    return mBoundingCenter;
}

const sead::Vector3f& CollisionShapeInfoDisk::getBoundingCenterWorld() const {
    return mBoundingCenterWorld;
}

f32 CollisionShapeInfoDisk::getBoundingRadius() const {
    return mBoundingRadius;
}

f32 CollisionShapeInfoDisk::getBoundingRadiusWorld() const {
    return mBoundingRadiusWorld;
}

f32 CollisionShapeInfoDisk::getCheckStepRange() const {
    return mCheckStepRange;
}

f32 CollisionShapeInfoDisk::getCheckStepRangeWorld() const {
    return mCheckStepRangeWorld;
}
