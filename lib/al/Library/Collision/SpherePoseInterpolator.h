#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {

class SpherePoseInterpolator {
public:
    SpherePoseInterpolator() {}

    void startInterp(const sead::Vector3f& posStart, const sead::Vector3f& posEnd, f32 radiusStart,
                     f32 radiusEnd, const sead::Quatf& poseStart, const sead::Quatf& poseEnd,
                     f32 stepSize);

    void nextStep();

    void calcInterpPos(sead::Vector3f* outPos) const;
    void calcInterp(sead::Vector3f* outPos, f32* outRadius, sead::Quatf* outPose,
                    sead::Vector3f* outRemainMoveVec) const;
    void calcRemainMoveVector(sead::Vector3f* outRemainMoveVec) const;
    f32 calcRadiusBaseScale(f32 radius) const;

    void getMoveVector(sead::Vector3f* outMoveVec);

    f32 getCurrentInterp() const { return mCurrInterp; }

    bool isDone() const { return mCurrInterp >= 1.0f; }

    bool isEnd() const { return mPrevInterp == 1.0f && mCurrInterp == 1.0f; }

private:
    sead::Vector3f mPos;
    sead::Vector3f mMove;
    f32 mRadiusStart;
    f32 mRadiusEnd;
    sead::Quatf mPoseStart;
    sead::Quatf mPoseEnd;

    f32 mInterpStep;
    f32 mCurrInterp;
    f32 mPrevInterp;
};

}  // namespace al
