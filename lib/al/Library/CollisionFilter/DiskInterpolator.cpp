#include "Library/CollisionFilter/DiskInterpolator.h"

#include <math/seadVector.h>

#include "Library/Math/MathUtil.h"

namespace al {

DiskInterpolator::DiskInterpolator() = default;

void DiskInterpolator::startInterp(const sead::Vector3f& posStart, const sead::Vector3f& posEnd,
                                   f32 radiusStart, f32 radiusEnd, f32 halfHeightStart,
                                   f32 halfHeightEnd, const sead::Matrix34f* rotateStart,
                                   const sead::Matrix34f* rotateEnd, const sead::Vector3f& normal,
                                   f32 stepSize) {
    mCurrInterp = 0.0f;
    mPrevInterp = 0.0f;
    mPos.set(posStart);
    mMove.setSub(posEnd, posStart);

    mRadiusStart = radiusStart;
    mRadiusEnd = radiusEnd;
    mHalfHeightStart = halfHeightStart;
    mHalfHeightEnd = halfHeightEnd;

    mRotateStart = rotateStart;
    mRotateEnd = rotateEnd;
    mNormal = normal;

    f32 dist = mMove.length() + radiusEnd - radiusStart;
    mInterpStep = (dist <= 0.0f) ? 1.0f : stepSize / dist;
}

void DiskInterpolator::nextStep() {
    mPrevInterp = mCurrInterp;
    mCurrInterp = sead::Mathf::clampMax(mCurrInterp + mInterpStep, 1.0f);
}

void DiskInterpolator::calcInterpPos(sead::Vector3f* outPos) const {
    f32 step = mCurrInterp;
    outPos->x = mMove.x * step + mPos.x;
    outPos->y = mMove.y * step + mPos.y;
    outPos->z = mMove.z * step + mPos.z;
}

void DiskInterpolator::getMoveVector(sead::Vector3f* moveVec) {
    f32 step = mCurrInterp;
    moveVec->x = mMove.x * step;
    moveVec->y = mMove.y * step;
    moveVec->z = mMove.z * step;
}

void DiskInterpolator::calcInterp(sead::Vector3f* outPos, f32* outRadius, f32* outHalfHeight,
                                  sead::Vector3f* outNormal, sead::Matrix34f* outRotate,
                                  sead::Vector3f* outRemainMoveVec) const {
    calcInterpPos(outPos);

    *outRadius = lerpValue(mRadiusStart, mRadiusEnd, mCurrInterp);
    *outHalfHeight = lerpValue(mHalfHeightStart, mHalfHeightEnd, mCurrInterp);

    outRotate->slerpTo(*mRotateStart, *mRotateEnd, mCurrInterp);
    normalize(outRotate);

    outNormal->setRotated(*outRotate, mNormal);
    normalize(outNormal);

    if (outRemainMoveVec) {
        f32 remainStep = 1.0f - mCurrInterp;
        outRemainMoveVec->x = mMove.x * remainStep;
        outRemainMoveVec->y = mMove.y * remainStep;
        outRemainMoveVec->z = mMove.z * remainStep;
    }
}

}  // namespace al
