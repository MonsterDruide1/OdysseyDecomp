#pragma once

#include <math/seadVector.h>

namespace al {
class BezierCurve;
class LinearCurve;

class RailPart {
public:
    RailPart();
    void init(const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&,
              const sead::Vector3f&);
    void calcPos(sead::Vector3f*, f32) const;
    void calcVelocity(sead::Vector3f*, f32) const;
    void calcDir(sead::Vector3f*, f32) const;
    void calcStartPos(sead::Vector3f*) const;
    void calcEndPos(sead::Vector3f*) const;
    f32 calcLength(f32, f32, s32) const;
    f32 calcCurveParam(f32) const;
    f32 calcNearestParam(const sead::Vector3f&, f32) const;
    void calcNearestPos(sead::Vector3f*, const sead::Vector3f&, f32) const;
    f32 calcNearestLength(f32*, const sead::Vector3f&, f32, f32) const;
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
