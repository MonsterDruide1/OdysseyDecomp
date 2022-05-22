#pragma once

#include <math/seadVector.h>

namespace al {

class LinearCurve {
public:
    LinearCurve();

    void set(const sead::Vector3f& start, const sead::Vector3f& end);
    void calcPos(sead::Vector3f* pos, float param) const;
    void calcVelocity(sead::Vector3f* vel, float param) const;
    float calcLength(float param_start, float param_end) const;
    float calcCurveParam(float param) const;
    float calcNearestParam(const sead::Vector3f& pos) const;
    float calcNearestLength(float* length, const sead::Vector3f& pos, float param) const;
    void calcNearestPos(sead::Vector3f* nearest, const sead::Vector3f& pos) const;
    void calcStartPos(sead::Vector3f* start) const;
    void calcEndPos(sead::Vector3f* end) const;

    float getLength() const { return mDistance; }

private:
    sead::Vector3f mStart = sead::Vector3f::zero;
    sead::Vector3f mDiff = sead::Vector3f::zero;
    float mDistance = 0;
};

}  // namespace al
