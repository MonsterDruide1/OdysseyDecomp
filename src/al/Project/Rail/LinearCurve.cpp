#include "al/Project/Rail/LinearCurve.h"

#include "al/Library/Math/VectorUtil.h"

namespace al {

LinearCurve::LinearCurve() = default;

void LinearCurve::set(const sead::Vector3f& start, const sead::Vector3f& end) {
    mStart = start;
    mDiff.x = end.x - start.x;
    mDiff.y = end.y - start.y;
    mDiff.z = end.z - start.z;
    mDistance = sead::Mathf::sqrt(mDiff.x * mDiff.x + mDiff.y * mDiff.y + mDiff.z * mDiff.z);
}
void LinearCurve::calcPos(sead::Vector3f* pos, float param) const {
    pos->x = (mDiff.x * param) + mStart.x;
    pos->y = (mDiff.y * param) + mStart.y;
    pos->z = (mDiff.z * param) + mStart.z;
}
void LinearCurve::calcVelocity(sead::Vector3f* vel, float param) const {
    *vel = mDiff;
}
float LinearCurve::calcLength(float param_start, float param_end) const {
    return mDistance * sead::Mathf::abs(param_end - param_start);
}
float LinearCurve::calcCurveParam(float param) const {
    if (al::isNearZero(mDistance, 0.001))
        return 0;

    return sead::Mathf::clamp(param, 0, mDistance) / mDistance;
}

float LinearCurve::calcNearestParam(const sead::Vector3f& pos) const {
    if (al::isNearZero(mDistance, 0.001))
        return 0;

    float dot = (pos - mStart).dot(mDiff);
    return sead::Mathf::clamp(dot / sead::Mathf::square(mDistance), 0, 1);
}

float LinearCurve::calcNearestLength(float* length, const sead::Vector3f& pos, float param) const {
    float nearestParam = calcNearestParam(pos);

    sead::Vector3f nearestPos;
    calcPos(&nearestPos, nearestParam);
    auto diff = nearestPos - pos;
    float len = diff.squaredLength();

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
