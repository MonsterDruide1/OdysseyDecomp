#pragma once

#include <math/seadVector.h>

namespace al {

class SphereInterpolator {
public:
    SphereInterpolator() = default;

    void startInterp(const sead::Vector3f& posStart, const sead::Vector3f& posEnd, f32 radiusStart,
                     f32 radiusEnd, f32 stepSize);

    void nextStep();

    void calcInterpPos(sead::Vector3f* outPos) const;
    void calcInterp(sead::Vector3f* outPos, f32* outRadius, sead::Vector3f* outRemainMoveVec) const;
    void calcRemainMoveVector(sead::Vector3f* outRemainMoveVec) const;

    void getMoveVector(sead::Vector3f* outMoveVec);
    void calcStepMoveVector(sead::Vector3f* outMoveVec) const;

    bool isDone() const { return mCurrInterp >= 1.0f; }

    bool isEnd() const { return mPrevInterp == 1.0f && mCurrInterp == 1.0f; }

private:
    sead::Vector3f mPos = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mMove = {0.0f, 0.0f, 0.0f};
    f32 mRadiusStart = 0.0f;
    f32 mRadiusEnd = 0.0f;

    f32 mInterpStep = 0.0f;
    f32 mCurrInterp = 0.0f;
    f32 mPrevInterp = 0.0f;
};

}  // namespace al
