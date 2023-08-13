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
    void calcPos(sead::Vector3f*, float) const;
    void calcVelocity(sead::Vector3f*, float) const;
    void calcDir(sead::Vector3f*, float) const;
    void calcStartPos(sead::Vector3f*) const;
    void calcEndPos(sead::Vector3f*) const;
    float calcLength(float, float, int) const;
    float calcCurveParam(float) const;
    float calcNearestParam(const sead::Vector3f&, float) const;
    void calcNearestPos(sead::Vector3f*, const sead::Vector3f&, float) const;
    float calcNearestLength(float*, const sead::Vector3f&, float, float) const;
    float getPartLength() const;

    void setTotalDistance(float len) { mTotalDistance = len; }
    float getTotalDistance() const { return mTotalDistance; }
    bool isBezierCurve() const { return mBezierCurve != nullptr; }

private:
    BezierCurve* mBezierCurve = nullptr;
    LinearCurve* mLinearCurve = nullptr;
    float mTotalDistance = 0;
};

}  // namespace al
