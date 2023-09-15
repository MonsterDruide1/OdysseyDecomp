#include "Project/Rail/LinearCurve.h"

#include "Library/Math/MathLengthUtil.h"
#include "Library/Math/VectorUtil.h"

namespace al {

LinearCurve::LinearCurve() = default;

void LinearCurve::set(const sead::Vector3f& start, const sead::Vector3f& end) {
    mStart = start;
    mDiff.x = end.x - start.x;
    mDiff.y = end.y - start.y;
    mDiff.z = end.z - start.z;
    mDistance = sead::Mathf::sqrt(mDiff.x * mDiff.x + mDiff.y * mDiff.y + mDiff.z * mDiff.z);
}
void LinearCurve::calcPos(sead::Vector3f* pos, f32 param) const {
    pos->x = (mDiff.x * param) + mStart.x;
    pos->y = (mDiff.y * param) + mStart.y;
    pos->z = (mDiff.z * param) + mStart.z;
}
void LinearCurve::calcVelocity(sead::Vector3f* vel, f32 param) const {
    *vel = mDiff;
}
f32 LinearCurve::calcLength(f32 param_start, f32 param_end) const {
    return mDistance * sead::Mathf::abs(param_end - param_start);
}
f32 LinearCurve::calcCurveParam(f32 param) const {
    if (al::isNearZero(mDistance, 0.001))
        return 0;

    return sead::Mathf::clamp(param, 0, mDistance) / mDistance;
}

f32 LinearCurve::calcNearestParam(const sead::Vector3f& pos) const {
    if (al::isNearZero(mDistance, 0.001))
        return 0;

    f32 dot = (pos - mStart).dot(mDiff);
    return sead::Mathf::clamp(dot / sead::Mathf::square(mDistance), 0, 1);
}

f32 LinearCurve::calcNearestLength(f32* length, const sead::Vector3f& pos, f32 param) const {
    f32 nearestParam = calcNearestParam(pos);

    sead::Vector3f nearestPos;
    calcPos(&nearestPos, nearestParam);
    auto diff = nearestPos - pos;
    f32 len = diff.squaredLength();

    *length = nearestParam * param;
    return len;
}

// NON_MATCHING: Difference in loading for calcNearestParam
void LinearCurve::calcNearestPos(sead::Vector3f* nearest, const sead::Vector3f& pos) const {
    calcPos(nearest, calcNearestParam(pos));
}

void LinearCurve::calcStartPos(sead::Vector3f* pos) const {
    *pos = mStart;
}

void LinearCurve::calcEndPos(sead::Vector3f* pos) const {
    pos->x = mStart.x + mDiff.x;
    pos->y = mStart.y + mDiff.y;
    pos->z = mStart.z + mDiff.z;
}

}  // namespace al
