#pragma once

#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {

class DiskInterpolator {
public:
    DiskInterpolator();

    void startInterp(const sead::Vector3f& posStart, const sead::Vector3f& posEnd, f32 radiusStart,
                     f32 radiusEnd, f32 halfHeightStart, f32 halfHeightEnd,
                     const sead::Matrix34f* rotateStart, const sead::Matrix34f* rotateEnd,
                     const sead::Vector3f& normal, f32 stepSize);

    void nextStep();

    void calcInterpPos(sead::Vector3f* outPos) const;
    void getMoveVector(sead::Vector3f* outMoveVec);

    void calcInterp(sead::Vector3f* outPos, f32* outRadius, f32* outHalfHeight,
                    sead::Vector3f* outNormal, sead::Matrix34f* outRotate,
                    sead::Vector3f* outRemainMoveVec) const;

    f32 getCurrentInterp() const { return mCurrInterp; }

    bool isDone() const { return mCurrInterp >= 1.0f; }

    bool isEnd() const { return mPrevInterp == 1.0f && mCurrInterp == 1.0f; }

private:
    sead::Vector3f mPos = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mMove = {0.0f, 0.0f, 0.0f};

    f32 mRadiusStart = 0.0f;
    f32 mRadiusEnd = 0.0f;
    f32 mHalfHeightStart = 0.0f;
    f32 mHalfHeightEnd = 0.0f;

    const sead::Matrix34f* mRotateStart = nullptr;
    const sead::Matrix34f* mRotateEnd = nullptr;
    sead::Vector3f mNormal = {0.0f, 0.0f, 0.0f};

    f32 mInterpStep = 0.0f;
    f32 mCurrInterp = 0.0f;
    f32 mPrevInterp = 0.0f;
};

}  // namespace al
