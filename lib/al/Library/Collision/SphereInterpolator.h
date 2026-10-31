#pragma once

#include <math/seadVector.h>

namespace al {

class SphereInterpolator {
public:
    SphereInterpolator() = default;

    void startInterp(const sead::Vector3f& posStart, const sead::Vector3f& posEnd, f32 sizeStart,
                     f32 sizeEnd, f32 steps);
    void nextStep();
    void calcInterpPos(sead::Vector3f* pos) const;
    void calcInterp(sead::Vector3f* pos, f32* size, sead::Vector3f* remainMoveVec) const;
    void calcRemainMoveVector(sead::Vector3f* remainMoveVec) const;
    void getMoveVector(sead::Vector3f* moveVec);
    void calcStepMoveVector(sead::Vector3f* moveVec) const;

    f32 getCurrentStep() const { return mCurrentStep; }

    bool isEnd() const { return mPrevStep == 1.0f && mCurrentStep == 1.0f; }

private:
    sead::Vector3f mPos = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mMove = {0.0f, 0.0f, 0.0f};
    f32 mSizeStart = 0.0f;
    f32 mSizeEnd = 0.0f;
    f32 mStepSize = 0.0f;
    f32 mCurrentStep = 0.0f;
    f32 mPrevStep = 0.0f;
};

}  // namespace al
