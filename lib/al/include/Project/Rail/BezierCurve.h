#pragma once

#include <math/seadVector.h>

namespace al {

class BezierCurve {
public:
    BezierCurve();

    void set(const sead::Vector3f& start, const sead::Vector3f& startHandle,
             const sead::Vector3f& endHandle, const sead::Vector3f& end);
    float calcLength(float startParam, float endParam, int stepCount) const;
    void calcPos(sead::Vector3f* pos, float param) const;
    void calcVelocity(sead::Vector3f* vel, float param) const;
    float calcDeltaLength(float param) const;
    float calcCurveParam(float distance) const;
    float calcNearestParam(const sead::Vector3f& pos, float interval) const;
    float calcNearestLength(float* param, const sead::Vector3f& pos, float max,
                            float interval) const;
    void calcNearestPos(sead::Vector3f* nearest, const sead::Vector3f& pos, float interval) const;
    void calcStartPos(sead::Vector3f* pos) const;
    void calcCtrlPos1(sead::Vector3f* pos) const;
    void calcCtrlPos2(sead::Vector3f* pos) const;
    void calcEndPos(sead::Vector3f* pos) const;

    float getLength() const { return mDistance; }

private:
    sead::Vector3f mStart = sead::Vector3f::zero;
    sead::Vector3f unk = sead::Vector3f::zero;
    sead::Vector3f unk2 = sead::Vector3f::zero;
    sead::Vector3f unk3 = sead::Vector3f::zero;
    float mDistance = 0;
};

}  // namespace al
