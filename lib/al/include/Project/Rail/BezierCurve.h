#pragma once

#include <math/seadVector.h>

namespace al {

class BezierCurve {
public:
    BezierCurve();

    void set(const sead::Vector3f& start, const sead::Vector3f& startHandle,
             const sead::Vector3f& endHandle, const sead::Vector3f& end);
    f32 calcLength(f32 startParam, f32 endParam, s32 stepCount) const;
    void calcPos(sead::Vector3f* pos, f32 param) const;
    void calcVelocity(sead::Vector3f* vel, f32 param) const;
    f32 calcDeltaLength(f32 param) const;
    f32 calcCurveParam(f32 distance) const;
    f32 calcNearestParam(const sead::Vector3f& pos, f32 interval) const;
    f32 calcNearestLength(f32* param, const sead::Vector3f& pos, f32 max, f32 interval) const;
    void calcNearestPos(sead::Vector3f* nearest, const sead::Vector3f& pos, f32 interval) const;
    void calcStartPos(sead::Vector3f* pos) const;
    void calcCtrlPos1(sead::Vector3f* pos) const;
    void calcCtrlPos2(sead::Vector3f* pos) const;
    void calcEndPos(sead::Vector3f* pos) const;

    f32 getLength() const { return mDistance; }

private:
    sead::Vector3f mStart = sead::Vector3f::zero;
    sead::Vector3f unk = sead::Vector3f::zero;
    sead::Vector3f unk2 = sead::Vector3f::zero;
    sead::Vector3f unk3 = sead::Vector3f::zero;
    f32 mDistance = 0;
};

}  // namespace al
