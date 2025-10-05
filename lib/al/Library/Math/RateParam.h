#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {

class RateParamV3f {
public:
    RateParamV3f();
    void setParam(const sead::Vector3f& startParam, const sead::Vector3f& endParam);
    void getParam(sead::Vector3f* startParam, sead::Vector3f* endParam) const;
    void calcLerpValue(sead::Vector3f* outVec, f32 t) const;
    void calcEaseInValue(sead::Vector3f* outVec, f32 t) const;
    void calcEaseOutValue(sead::Vector3f* outVec, f32 t) const;
    void calcEaseInOutValue(sead::Vector3f* outVec, f32 t) const;

private:
    sead::Vector3f mStartParam = sead::Vector3f::zero;
    sead::Vector3f mEndParam = sead::Vector3f::zero;
};

class RateParamF32 {
public:
    RateParamF32();
    void setParam(f32 startParam, f32 endParam);
    f32 calcLerpValue(f32 t) const;
    f32 calcEaseInValue(f32 t) const;
    f32 calcEaseOutValue(f32 t) const;
    f32 calcEaseInOutValue(f32 t) const;

private:
    f32 mStartParam = 0.0f;
    f32 mEndParam = 0.0f;
};

}  // namespace al
