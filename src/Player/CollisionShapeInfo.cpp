#include "Player/CollisionShapeInfo.h"

CollisionShapeInfoBase::CollisionShapeInfoBase(CollisionShapeId id, const char* name)
    : mId(id), mName(name) {}

const sead::Vector3f& CollisionShapeInfoBase::getBoundingCenter() const {
    return sead::Vector3f::zero;
}
const sead::Vector3f& CollisionShapeInfoBase::getBoundingCenterWorld() const {
    return sead::Vector3f::zero;
}
f32 CollisionShapeInfoBase::getBoundingRadius() const {
    return 0.0f;
}
f32 CollisionShapeInfoBase::getBoundingRadiusWorld() const {
    return 0.0f;
}
f32 CollisionShapeInfoBase::getCheckStepRange() const {
    return 100000.0f;
}
f32 CollisionShapeInfoBase::getCheckStepRangeWorld() const {
    return 100000.0f;
}
void CollisionShapeInfoBase::updateShapeOffset(const sead::Vector3f& offset) {}
void CollisionShapeInfoBase::calcWorldShapeInfo(const sead::Matrix34f& matrix, f32) {}
void CollisionShapeInfoBase::calcRelativeShapeInfo(const sead::Matrix34f& matrix) {}
