#pragma once

#include <math/seadVector.h>

namespace al {

class LinearCurve {
public:
    LinearCurve();

    void set(const sead::Vector3f& start, const sead::Vector3f& end);
    void calcPos(sead::Vector3f* pos, f32 param) const;
    void calcVelocity(sead::Vector3f* vel, f32 param) const;
    f32 calcLength(f32 param_start, f32 param_end) const;
    f32 calcCurveParam(f32 param) const;
    f32 calcNearestParam(const sead::Vector3f& pos) const;
    f32 calcNearestLength(f32* length, const sead::Vector3f& pos, f32 param) const;
    void calcNearestPos(sead::Vector3f* nearest, const sead::Vector3f& pos) const;
    void calcStartPos(sead::Vector3f* start) const;
    void calcEndPos(sead::Vector3f* end) const;

    f32 getLength() const { return mDistance; }

private:
    sead::Vector3f mStart = sead::Vector3f::zero;
    sead::Vector3f mDiff = sead::Vector3f::zero;
    f32 mDistance = 0;
};

}  // namespace al
