#include "Library/Collision/SpherePoseInterpolator.h"

#include "Library/Math/MathUtil.h"

namespace al {

void SpherePoseInterpolator::startInterp(const sead::Vector3f& posStart,
                                         const sead::Vector3f& posEnd, f32 radiusStart,
                                         f32 radiusEnd, const sead::Quatf& poseStart,
                                         const sead::Quatf& poseEnd, f32 stepSize) {
    mCurrInterp = 0.0f;
    mPrevInterp = 0.0f;
    mPos = posStart;
    mMove = posEnd - posStart;

    mPoseStart = poseStart;
    mPoseEnd = poseEnd;

    mRadiusStart = radiusStart;
    mRadiusEnd = radiusEnd;

    f32 dist = mMove.length() + radiusEnd - radiusStart;
    mInterpStep = (dist <= 0.0f) ? 1.0f : stepSize / dist;
}

void SpherePoseInterpolator::nextStep() {
    mPrevInterp = mCurrInterp;
    mCurrInterp = sead::Mathf::clampMax(mCurrInterp + mInterpStep, 1.0f);
}

void SpherePoseInterpolator::calcInterpPos(sead::Vector3f* outPos) const {
    f32 step = mCurrInterp;
    outPos->x = mMove.x * step + mPos.x;
    outPos->y = mMove.y * step + mPos.y;
    outPos->z = mMove.z * step + mPos.z;
}

void SpherePoseInterpolator::calcInterp(sead::Vector3f* outPos, f32* outRadius,
                                        sead::Quatf* outPose,
                                        sead::Vector3f* outRemainMoveVec) const {
    calcInterpPos(outPos);

    *outRadius = mRadiusStart + (mRadiusEnd - mRadiusStart) * mCurrInterp;

    slerpQuat(outPose, mPoseStart, mPoseEnd, mCurrInterp);
    outPose->normalize();

    calcRemainMoveVector(outRemainMoveVec);
}

void SpherePoseInterpolator::calcRemainMoveVector(sead::Vector3f* remainMoveVec) const {
    if (remainMoveVec) {
        f32 remainStep = 1.0f - mCurrInterp;
        remainMoveVec->x = mMove.x * remainStep;
        remainMoveVec->y = mMove.y * remainStep;
        remainMoveVec->z = mMove.z * remainStep;
    }
}

f32 SpherePoseInterpolator::calcRadiusBaseScale(f32 radius) const {
    return calcRate01(radius, 0.0f, mRadiusEnd);
}

void SpherePoseInterpolator::getMoveVector(sead::Vector3f* moveVec) {
    f32 step = mCurrInterp;
    moveVec->x = mMove.x * step;
    moveVec->y = mMove.y * step;
    moveVec->z = mMove.z * step;
}

}  // namespace al
