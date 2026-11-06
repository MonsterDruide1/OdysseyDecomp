#include "Library/Collision/SphereInterpolator.h"

namespace al {

void SphereInterpolator::startInterp(const sead::Vector3f& posStart, const sead::Vector3f& posEnd,
                                     f32 radiusStart, f32 radiusEnd, f32 stepSize) {
    mCurrInterp = 0.0f;
    mPrevInterp = 0.0f;
    mPos = posStart;
    mMove = posEnd - posStart;
    mRadiusStart = radiusStart;
    mRadiusEnd = radiusEnd;

    f32 dist = mMove.length() + radiusEnd - radiusStart;
    mInterpStep = (dist <= 0.0f) ? 1.0f : stepSize / dist;
}

void SphereInterpolator::nextStep() {
    mPrevInterp = mCurrInterp;
    mCurrInterp = sead::Mathf::clampMax(mCurrInterp + mInterpStep, 1.0f);
}

void SphereInterpolator::calcInterpPos(sead::Vector3f* outPos) const {
    f32 interp = mCurrInterp;
    outPos->x = mMove.x * interp + mPos.x;
    outPos->y = mMove.y * interp + mPos.y;
    outPos->z = mMove.z * interp + mPos.z;
}

void SphereInterpolator::calcInterp(sead::Vector3f* outPos, f32* outRadius,
                                    sead::Vector3f* outRemainMoveVec) const {
    calcInterpPos(outPos);

    *outRadius = mRadiusStart + (mRadiusEnd - mRadiusStart) * mCurrInterp;

    calcRemainMoveVector(outRemainMoveVec);
}

void SphereInterpolator::calcRemainMoveVector(sead::Vector3f* outRemainMoveVec) const {
    if (outRemainMoveVec) {
        f32 remainStep = 1.0f - mCurrInterp;
        outRemainMoveVec->x = mMove.x * remainStep;
        outRemainMoveVec->y = mMove.y * remainStep;
        outRemainMoveVec->z = mMove.z * remainStep;
    }
}

void SphereInterpolator::getMoveVector(sead::Vector3f* outMoveVec) {
    f32 interp = mCurrInterp;
    outMoveVec->x = mMove.x * interp;
    outMoveVec->y = mMove.y * interp;
    outMoveVec->z = mMove.z * interp;
}

void SphereInterpolator::calcStepMoveVector(sead::Vector3f* outMoveVec) const {
    f32 step = mCurrInterp - mPrevInterp;
    outMoveVec->x = mMove.x * step;
    outMoveVec->y = mMove.y * step;
    outMoveVec->z = mMove.z * step;
}

}  // namespace al
