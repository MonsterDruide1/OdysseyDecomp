#include "Library/Collision/KCollisionServer.h"

#include "Library/Math/MathUtil.h"

namespace al {

void SphereInterpolator::startInterp(const sead::Vector3f& posStart, const sead::Vector3f& posEnd,
                                     f32 sizeStart, f32 sizeEnd, f32 steps) {
    mCurrentStep = 0.0f;
    mPrevStep = 0.0f;
    mPos = posStart;
    mMove = posEnd - posStart;
    mSizeStart = sizeStart;
    mSizeEnd = sizeEnd;

    f32 dist = mMove.length() + sizeEnd - sizeStart;
    mStepSize = (dist <= 0.0f) ? 1.0f : steps / dist;
}
void SphereInterpolator::nextStep() {
    // re-interpreting between f32/s32 required to match
    s32 curStep = *(s32*)&mCurrentStep;
    f32 stepAsFloat = *(f32*)&curStep;
    f32 newStep = sead::Mathf::clampMax(stepAsFloat + mStepSize, 1.0f);
    *(s32*)&mPrevStep = curStep;
    mCurrentStep = newStep;
}
void SphereInterpolator::calcInterpPos(sead::Vector3f* pos) const {
    f32 step = mCurrentStep;
    pos->x = mMove.x * step + mPos.x;
    pos->y = mMove.y * step + mPos.y;
    pos->z = mMove.z * step + mPos.z;
}
void SphereInterpolator::calcInterp(sead::Vector3f* pos, f32* size,
                                    sead::Vector3f* remainMoveVec) const {
    calcInterpPos(pos);
    *size = mSizeStart + (mSizeEnd - mSizeStart) * mCurrentStep;
    calcRemainMoveVector(remainMoveVec);
}
void SphereInterpolator::calcRemainMoveVector(sead::Vector3f* remainMoveVec) const {
    if (remainMoveVec) {
        f32 remainStep = 1.0f - mCurrentStep;
        remainMoveVec->x = mMove.x * remainStep;
        remainMoveVec->y = mMove.y * remainStep;
        remainMoveVec->z = mMove.z * remainStep;
    }
}
void SphereInterpolator::getMoveVector(sead::Vector3f* moveVec) {
    f32 step = mCurrentStep;
    moveVec->x = mMove.x * step;
    moveVec->y = mMove.y * step;
    moveVec->z = mMove.z * step;
}
void SphereInterpolator::calcStepMoveVector(sead::Vector3f* moveVec) const {
    f32 step = mCurrentStep - mPrevStep;
    moveVec->x = mMove.x * step;
    moveVec->y = mMove.y * step;
    moveVec->z = mMove.z * step;
}

void SpherePoseInterpolator::startInterp(const sead::Vector3f& posStart,
                                         const sead::Vector3f& posEnd, f32 sizeStart, f32 sizeEnd,
                                         const sead::Quatf& quatStart, const sead::Quatf& quatEnd,
                                         f32 steps) {
    mCurrentStep = 0.0f;
    mPrevStep = 0.0f;
    mPos = posStart;
    mMove = posEnd - posStart;

    mQuatStart.x = quatStart.x;
    mQuatStart.y = quatStart.y;
    mQuatStart.z = quatStart.z;
    mQuatStart.w = quatStart.w;

    mQuatEnd.x = quatEnd.x;
    mQuatEnd.y = quatEnd.y;
    mQuatEnd.z = quatEnd.z;
    mQuatEnd.w = quatEnd.w;

    mSizeStart = sizeStart;
    mSizeEnd = sizeEnd;

    f32 dist = mMove.length() + sizeEnd - sizeStart;
    mStepSize = (dist <= 0.0f) ? 1.0f : steps / dist;
}

void SpherePoseInterpolator::nextStep() {
    // re-interpreting between f32/s32 required to match
    s32 curStep = *(s32*)&mCurrentStep;
    f32 stepAsFloat = *(f32*)&curStep;
    f32 newStep = sead::Mathf::clampMax(stepAsFloat + mStepSize, 1.0f);
    *(s32*)&mPrevStep = curStep;
    mCurrentStep = newStep;
}
void SpherePoseInterpolator::calcInterpPos(sead::Vector3f* pos) const {
    f32 step = mCurrentStep;
    pos->x = mMove.x * step + mPos.x;
    pos->y = mMove.y * step + mPos.y;
    pos->z = mMove.z * step + mPos.z;
}
void SpherePoseInterpolator::calcInterp(sead::Vector3f* pos, f32* size, sead::Quatf* quat,
                                        sead::Vector3f* remainMoveVec) const {
    calcInterpPos(pos);
    *size = mSizeStart + (mSizeEnd - mSizeStart) * mCurrentStep;
    slerpQuat(quat, mQuatStart, mQuatEnd, mCurrentStep);
    quat->normalize();
    calcRemainMoveVector(remainMoveVec);
}
void SpherePoseInterpolator::calcRemainMoveVector(sead::Vector3f* remainMoveVec) const {
    if (remainMoveVec) {
        f32 remainStep = 1.0f - mCurrentStep;
        remainMoveVec->x = mMove.x * remainStep;
        remainMoveVec->y = mMove.y * remainStep;
        remainMoveVec->z = mMove.z * remainStep;
    }
}
f32 SpherePoseInterpolator::calcRadiusBaseScale(f32 unk) const {
    return calcRate01(unk, 0.0f, mSizeEnd);
}
void SpherePoseInterpolator::getMoveVector(sead::Vector3f* moveVec) {
    f32 step = mCurrentStep;
    moveVec->x = mMove.x * step;
    moveVec->y = mMove.y * step;
    moveVec->z = mMove.z * step;
}

}  // namespace al
