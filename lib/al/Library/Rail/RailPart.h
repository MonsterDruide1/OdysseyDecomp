#pragma once

#include <math/seadVector.h>

namespace al {
class BezierCurve;
class LinearCurve;

class RailPart {
public:
    RailPart();
    void init(const sead::Vector3f& start, const sead::Vector3f& startHandle,
              const sead::Vector3f& endHandle, const sead::Vector3f& end);
    void calcPos(sead::Vector3f* pos, f32 param) const;
    void calcVelocity(sead::Vector3f* vel, f32 param) const;
    void calcDir(sead::Vector3f* dir, f32 param) const;
    void calcStartPos(sead::Vector3f* pos) const;
    void calcEndPos(sead::Vector3f* pos) const;
    f32 calcLength(f32 startParam, f32 endParam, s32 stepCount) const;
    f32 calcCurveParam(f32 param) const;
    f32 calcNearestParam(const sead::Vector3f& pos, f32 interval) const;
    void calcNearestPos(sead::Vector3f* nearest, const sead::Vector3f& pos, f32 interval) const;
    f32 calcNearestLength(f32* param, const sead::Vector3f& pos, f32 max, f32 interval) const;
    f32 getPartLength() const;

    void setTotalDistance(f32 len) { mTotalDistance = len; }

    f32 getTotalDistance() const { return mTotalDistance; }

    bool isBezierCurve() const { return mBezierCurve != nullptr; }

private:
    BezierCurve* mBezierCurve = nullptr;
    LinearCurve* mLinearCurve = nullptr;
    f32 mTotalDistance = 0;
};

}  // namespace al
