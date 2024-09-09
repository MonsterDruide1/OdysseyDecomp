#include "Library/Math/ParabolicPath.h"

#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/MathUtil.h"

namespace al {

ParabolicPath::ParabolicPath() {}

void ParabolicPath::initFromUpVector(const sead::Vector3f& start, const sead::Vector3f& end,
                                     const sead::Vector3f& up) {
    f32 scalar;
    sead::Vector3f upDir;
    separateScalarAndDirection(&scalar, &upDir, up);
    initFromUpVector(start, end, upDir, scalar);
}

void ParabolicPath::initFromUpVector(const sead::Vector3f& start, const sead::Vector3f& end,
                                     const sead::Vector3f& up, f32 maxHeight) {
    mUp.set(up);
    sead::Vector3f diff = end - start;
    f32 verticalDistance = diff.dot(mUp);
    mHorizontalDirection = diff - (mUp * verticalDistance);
    separateScalarAndDirection(&mHorizontalDistance, &mHorizontalDirection, mHorizontalDirection);
    calcParabolicFunctionParam(&mGravity, &mInitialVelY, maxHeight, verticalDistance);
    mStart.set(start);
}

void ParabolicPath::initFromMaxHeight(const sead::Vector3f& start, const sead::Vector3f& end,
                                      const sead::Vector3f& projectedEnd) {
    sead::Vector3f up;
    f32 scalar;
    separateScalarAndDirection(&scalar, &up, projectedEnd - end);
    initFromUpVector(start, end, up, (projectedEnd - start).dot(up));
}

void ParabolicPath::initFromUpVectorAddHeight(const sead::Vector3f& start,
                                              const sead::Vector3f& end, const sead::Vector3f& up,
                                              f32 height) {
    f32 verticalDistance = sead::Mathf::clampMin((end - start).dot(up), 0.0f);
    initFromUpVector(start, end, up, verticalDistance + height);
}

f32 ParabolicPath::getLength(f32 start, f32 end, s32 iterations) const {
    s32 steps = sead::Mathi::clampMin(iterations, 1);
    f32 stepSize = (end - start) / steps;
    f32 squaredHStepSize = sead::Mathf::square(stepSize * mHorizontalDistance);
    f32 vDist = (mGravity * start + mInitialVelY) * start;
    f32 length = 0;

    for (s32 i = 0; i < steps;) {
        i++;
        f32 prevVDist = vDist;
        f32 curStep = start + i * stepSize;
        vDist = (mGravity * curStep + mInitialVelY) * curStep;
        length += sead::Mathf::sqrt(squaredHStepSize + sead::Mathf::square(vDist - prevVDist));
    }

    return length;
}

f32 ParabolicPath::getTotalLength(s32 iterations) const {
    return getLength(0.0f, 1.0f, iterations);
}

void ParabolicPath::calcPositionHV(sead::Vector3f* pos, f32 h, f32 v) const {
    f32 hDist = mHorizontalDistance * h;
    f32 vDist = (mGravity * v + mInitialVelY) * v;
    *pos = mStart + (vDist * mUp) + (hDist * mHorizontalDirection);
}

void ParabolicPath::calcPosition(sead::Vector3f* pos, f32 prog) const {
    calcPositionHV(pos, prog, prog);
}

void ParabolicPath::calcPositionEaseOutH(sead::Vector3f* pos, f32 prog) const {
    calcPositionHV(pos, easeOut(prog), prog);
}

void ParabolicPath::calcDirection(sead::Vector3f* pos, f32 prog, f32 stepSize) const {
    f32 prog1, prog2;
    if (prog < stepSize) {
        prog1 = 0.0f;
        prog2 = stepSize;
    } else if ((1.0f - stepSize) < prog) {
        prog2 = 1.0f;
        prog1 = 1.0f - stepSize;
    } else {
        prog2 = prog + stepSize;
        prog1 = prog;
    }

    sead::Vector3f pos1, pos2;
    calcPosition(&pos1, prog1);
    calcPosition(&pos2, prog2);
    *pos = pos2 - pos1;
    tryNormalizeOrZero(pos);
}

f32 ParabolicPath::calcPathSpeedFromGravityAccel(f32 frames) const {
    return sead::Mathf::abs(frames / mGravity);
}

f32 ParabolicPath::calcPathSpeedFromAverageSpeed(f32 frames) const {
    return frames / getTotalLength(10);
}

f32 ParabolicPath::calcPathSpeedFromHorizontalSpeed(f32 frames) const {
    return frames / mHorizontalDistance;
}

s32 ParabolicPath::calcPathTimeFromGravityAccel(f32 frames) const {
    return 1.0f / calcPathSpeedFromGravityAccel(frames);
}

s32 ParabolicPath::calcPathTimeFromAverageSpeed(f32 frames) const {
    return 1.0f / calcPathSpeedFromAverageSpeed(frames);
}

s32 ParabolicPath::calcPathTimeFromHorizontalSpeed(f32 frames) const {
    return 1.0f / calcPathSpeedFromHorizontalSpeed(frames);
}

}  // namespace al
