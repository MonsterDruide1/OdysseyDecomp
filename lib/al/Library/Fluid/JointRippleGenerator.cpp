#include "Library/Fluid/JointRippleGenerator.h"

#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nature/NatureUtil.h"

namespace al {
JointRippleGenerator::JointRippleGenerator(const LiveActor* parentActor) : mParent(parentActor) {}

void JointRippleGenerator::reset() {
    sead::Vector3f jointOffset;
    calcJointOffsetPos(&jointOffset, mParent, mJoint, mOffset);
    mLastJointOffset = jointOffset;
}

void JointRippleGenerator::updateAndGenerate() {
    sead::Vector3f jointOffset;
    calcJointOffsetPos(&jointOffset, mParent, mJoint, mOffset);

    f32 rate =
        calcRate01((jointOffset - mLastJointOffset).length(), 0.0f, mMaxDistance) * mRateAmplitude;
    tryAddRippleRandomBlur(mParent, jointOffset, rate * _2c, _30, 10.0f);
    mLastJointOffset.set(jointOffset);
}

}  // namespace al
