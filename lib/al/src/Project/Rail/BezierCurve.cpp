#include "Project/Rail/BezierCurve.h"

namespace al {

BezierCurve::BezierCurve() = default;

void BezierCurve::set(const sead::Vector3f& start, const sead::Vector3f& startHandle,
                      const sead::Vector3f& endHandle, const sead::Vector3f& end) {
    sead::Vector3f diff1 = startHandle - start;
    sead::Vector3f diff2 = endHandle - startHandle;
    sead::Vector3f diff3 = end - endHandle;

    sead::Vector3f diffDiff1 = diff2 - diff1;
    sead::Vector3f diffDiff2 = diff3 - diff2;

    sead::Vector3f diffDiffDiff = diffDiff2 - diffDiff1;

    mStart = start;
    unk = diff1 * 3;
    unk2 = diffDiff1 * 3;
    unk3 = diffDiffDiff;

    mDistance = calcLength(0.0, 1.0, 10);
}

f32 BezierCurve::calcLength(f32 startParam, f32 endParam, s32 stepCount) const {
    f32 avgVelocity = (calcDeltaLength(startParam) + calcDeltaLength(endParam)) / 2;
    f32 halfStepSize = (endParam - startParam) * (1.0f / (stepCount * 2.0f));

    f32 sumVelHalfStep = 0.0f;
    f32 sumVelFullStep = 0.0f;
    for (s32 i = 1; i <= stepCount; i++) {
        f32 doubleI = i * 2.0f;

        sumVelHalfStep += calcDeltaLength((halfStepSize * (doubleI - 1)) + startParam);

        if (i != stepCount) {
            sumVelFullStep += calcDeltaLength((halfStepSize * (doubleI)) + startParam);
        }
    }

    return std::floor((halfStepSize * 0.33333f) *
                      (avgVelocity + (sumVelFullStep * 2) + (sumVelHalfStep * 4)) * 1024.0f) /
           1024.f;
}

void BezierCurve::calcPos(sead::Vector3f* pos, f32 param) const {
    f32 square = param * param;
    f32 cube = square * param;

    pos->x = (unk.x * param) + mStart.x;
    pos->y = (unk.y * param) + mStart.y;
    pos->z = (unk.z * param) + mStart.z;

    pos->x = (unk2.x * square) + pos->x;
    pos->y = (unk2.y * square) + pos->y;
    pos->z = (unk2.z * square) + pos->z;

    pos->x = (unk3.x * cube) + pos->x;
    pos->y = (unk3.y * cube) + pos->y;
    pos->z = (unk3.z * cube) + pos->z;
}

void BezierCurve::calcVelocity(sead::Vector3f* vel, f32 param) const {
    f32 fac1 = param + param;
    f32 fac2 = 3 * param * param;

    vel->x = (unk2.x * fac1) + unk.x;
    vel->y = (unk2.y * fac1) + unk.y;
    vel->z = (unk2.z * fac1) + unk.z;

    vel->x = (unk3.x * fac2) + vel->x;
    vel->y = (unk3.y * fac2) + vel->y;
    vel->z = (unk3.z * fac2) + vel->z;
}

f32 BezierCurve::calcDeltaLength(f32 param) const {
    sead::Vector3f tmp;
    calcVelocity(&tmp, param);
    return tmp.length();
}

// NON_MATCHING: flipped parts of if in last statement and unoptimized 1.0f - load
f32 BezierCurve::calcCurveParam(f32 distance) const {
    f32 percent = distance / mDistance;
    f32 partLength = calcLength(0, percent, 10);
    if (sead::Mathf::abs(distance - partLength) <= 0.01f)
        return percent;

    for (s32 i = 0; i <= 4; i++) {
        f32 len = std::max(calcDeltaLength(percent), 0.001f);
        f32 newPercent = percent + ((distance - partLength) / len);

        percent = sead::Mathf::clamp(newPercent, 0.0f, 1.0f);
        partLength = calcLength(0.0f, percent, 10);
        if (sead::Mathf::abs(distance - partLength) <= 0.01f)
            return percent;
    }

    if (percent > 1.0f || partLength < 0.0f) {
        return sead::Mathf::clamp(percent, 0.0f, 1.0f);
    }
    return percent;
}

f32 BezierCurve::calcNearestParam(const sead::Vector3f& pos, f32 interval) const {
    f32 currentParam = 0.0;
    f32 bestParam = -1.0;
    f32 bestDist = 3.4028e38;
    do {
        sead::Vector3f nearest;
        calcPos(&nearest, currentParam);
        f32 currentDist = (nearest - pos).squaredLength();

        if (currentDist < bestDist) {
            bestParam = currentParam;
            bestDist = currentDist;
        }
        currentParam = currentParam + interval;
    } while (currentParam <= 1.0);
    return bestParam;
}

f32 BezierCurve::calcNearestLength(f32* param, const sead::Vector3f& pos, f32 max,
                                   f32 interval) const {
    f32 bestParam = -1.0;
    f32 currentParam = 0.0;
    f32 bestDist = 3.4028e38;
    while (currentParam < max) {
        sead::Vector3f nearest;
        calcPos(&nearest, calcCurveParam(currentParam));
        f32 currentDist = (nearest - pos).squaredLength();

        if (currentDist < bestDist) {
            bestParam = currentParam;
            bestDist = currentDist;
        }
        currentParam = currentParam + interval;
    }
    *param = bestParam;
    return bestDist;
}

// NON_MATCHING: Difference in loading for calcNearestParam
void BezierCurve::calcNearestPos(sead::Vector3f* nearest, const sead::Vector3f& pos,
                                 f32 interval) const {
    calcPos(nearest, calcNearestParam(pos, interval));
}

void BezierCurve::calcStartPos(sead::Vector3f* pos) const {
    *pos = mStart;
}

void BezierCurve::calcCtrlPos1(sead::Vector3f* pos) const {
    pos->x = unk.x * 0.333333f;
    pos->y = unk.y * 0.333333f;
    pos->z = unk.z * 0.333333f;

    pos->x = pos->x + mStart.x;
    pos->y = pos->y + mStart.y;
    pos->z = pos->z + mStart.z;
}

void BezierCurve::calcCtrlPos2(sead::Vector3f* pos) const {
    pos->x = unk2.x * 0.333333f;
    pos->y = unk2.y * 0.333333f;
    pos->z = unk2.z * 0.333333f;

    pos->x = (unk.x * 0.6666667f) + pos->x;
    pos->y = (unk.y * 0.6666667f) + pos->y;
    pos->z = (unk.z * 0.6666667f) + pos->z;

    pos->x = pos->x + mStart.x;
    pos->y = pos->y + mStart.y;
    pos->z = pos->z + mStart.z;
}

void BezierCurve::calcEndPos(sead::Vector3f* pos) const {
    pos->x = mStart.x + unk.x;
    pos->y = mStart.y + unk.y;
    pos->z = mStart.z + unk.z;

    pos->x = pos->x + unk2.x;
    pos->y = pos->y + unk2.y;
    pos->z = pos->z + unk2.z;

    pos->x = pos->x + unk3.x;
    pos->y = pos->y + unk3.y;
    pos->z = pos->z + unk3.z;
}

}  // namespace al
