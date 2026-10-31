#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {

class SpherePoseInterpolator {
public:
    SpherePoseInterpolator() {}

    void startInterp(const sead::Vector3f& posStart, const sead::Vector3f& posEnd, f32 sizeStart,
                     f32 sizeEnd, const sead::Quatf& quatStart, const sead::Quatf& quatEnd,
                     f32 steps);
    void nextStep();
    void calcInterpPos(sead::Vector3f* pos) const;
    void calcInterp(sead::Vector3f* pos, f32* size, sead::Quatf* quat,
                    sead::Vector3f* remainMoveVec) const;
    void calcRemainMoveVector(sead::Vector3f* remainMoveVec) const;
    f32 calcRadiusBaseScale(f32 unk) const;
    void getMoveVector(sead::Vector3f* moveVec);

    f32 getCurrentStep() const { return mCurrentStep; }

    bool isEnd() const { return mPrevStep == 1.0f && mCurrentStep == 1.0f; }

private:
    sead::Vector3f mPos;
    sead::Vector3f mMove;
    f32 mSizeStart;
    f32 mSizeEnd;
    sead::Quatf mQuatStart;
    sead::Quatf mQuatEnd;
    f32 mStepSize;
    f32 mCurrentStep;
    f32 mPrevStep;
};

}  // namespace al
