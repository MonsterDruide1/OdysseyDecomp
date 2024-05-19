#include "Player/CollisionShapeInfo.h"
#include "math/seadMatrix.h"
#include "Library/Math/MathUtil.h"

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

CollisionShapeInfoSphere::CollisionShapeInfoSphere(char const* name, float boundingRadius,
                                                   const sead::Vector3f& boundingCenter)
    : CollisionShapeInfoBase(CollisionShapeId::Sphere, name), mBoundingRadius(boundingRadius), mBoundingCenter(boundingCenter) {
    calcWorldShapeInfo(sead::Matrix34f::ident, 1.0f);
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
void CollisionShapeInfoSphere::updateShapeOffset(const sead::Vector3f& offset) {
    mBoundingCenter = offset;
}
void CollisionShapeInfoSphere::calcWorldShapeInfo(const sead::Matrix34f& mtx, f32 scale) {
    mBoundingCenterWorld.setMul(mtx, mBoundingCenter * scale);
    mBoundingRadiusWorld = mBoundingRadius * scale;
    unk2 = unk1 * scale;
    mWorldShapeInfo.setRotated(mtx, mUp);
}
void CollisionShapeInfoSphere::calcRelativeShapeInfo(const sead::Matrix34f& mtx) {
    mRelativeShapeInfo.setMul(mtx, mBoundingCenterWorld);
}


CollisionShapeInfoArrow::CollisionShapeInfoArrow(const char* name, const sead::Vector3f& a3, const sead::Vector3f& a4, f32 a5, s32 a6)
    : CollisionShapeInfoBase(CollisionShapeId::Sphere, name), a3(a3), a4(a4), a5(a5), a6(a6) {
    // inlined call to updateShapeOffset?
    /*mBoundingCenter = (a3 + sead::Vector3f{0.0f, 0.0f, 0.0f}) + (a4 * 0.5f);
    mBoundingRadius = a4.length() * 0.5f;*/
    updateShapeOffset({0.0f, 0.0f, 0.0f});
    calcWorldShapeInfo(sead::Matrix34f::ident, 1.0f);
}
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
void CollisionShapeInfoArrow::updateShapeOffset(const sead::Vector3f& offset) {
    vec1 = offset;
    sead::Vector3f a3 = this->a3;
    sead::Vector3f a4 = this->a4;
    mBoundingCenter = (a3 + offset) + (a4 * 0.5f);
    mBoundingRadius = a4.length() * 0.5f;
}
void CollisionShapeInfoArrow::calcWorldShapeInfo(const sead::Matrix34f& mtx, f32 scale) {
    vec2.setRotated(mtx, (a3 + vec1) * scale);
    vec4.setRotated(mtx, a4 * scale);
    mBoundingCenterWorld.setMul(mtx, mBoundingCenter * scale);
    vec3 = vec2 + vec4;
    mBoundingRadiusWorld = mBoundingRadius * scale;
    al::calcArrowAabb(&mAabb, vec2, vec3);
}
void CollisionShapeInfoArrow::calcRelativeShapeInfo(const sead::Matrix34f& mtx) {
    vec5.setMul(mtx, vec2);
    vec6.setMul(mtx, vec4);
}
