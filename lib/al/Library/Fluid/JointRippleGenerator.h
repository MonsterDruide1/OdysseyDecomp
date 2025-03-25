#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;

class JointRippleGenerator {
public:
    JointRippleGenerator(const LiveActor* parentActor);
    void reset();
    void updateAndGenerate();

private:
    const LiveActor* mParent = nullptr;
    sead::Vector3f mLastJointOffset = sead::Vector3f::zero;
    const char* mJoint = nullptr;
    sead::Vector3f mOffset = sead::Vector3f::zero;
    f32 _2c = 0.0f;
    f32 _30 = 0.0f;
    f32 mRateAmplitude = 1.0f;
    f32 mMaxDistance = 1.0f;
};

}  // namespace al
