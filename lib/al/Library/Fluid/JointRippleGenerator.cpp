#include "Library/Fluid/JointRippleGenerator.h"

#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nature/NatureUtil.h"

namespace al {
JointRippleGenerator::JointRippleGenerator(const LiveActor* parentActor) : mParent(parentActor) {}

void JointRippleGenerator::reset() {
    sead::Vector3f offsetPos;
    calcJointOffsetPos(&offsetPos, mParent, mJoint, mOffset);
    mOffsetPos = offsetPos;
}

void JointRippleGenerator::updateAndGenerate() {
    sead::Vector3f offsetPos;
    calcJointOffsetPos(&offsetPos, mParent, mJoint, mOffset);

    f32 rate = calcRate01((offsetPos - mOffsetPos).length(), 0.0f, mMaxDistance) * mRateAmplitude;
    tryAddRippleRandomBlur(mParent, offsetPos, rate * _2c, _30, 10.0f);
    mOffsetPos.set(offsetPos);
}

}  // namespace al
