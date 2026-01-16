#include "Library/Math/RateParam.h"

#include "Library/Math/MathUtil.h"

namespace al {

RateParamV3f::RateParamV3f() = default;

void RateParamV3f::setParam(const sead::Vector3f& startParam, const sead::Vector3f& endParam) {
    mStartParam.set(startParam);
    mEndParam.set(endParam);
}

void RateParamV3f::getParam(sead::Vector3f* startParam, sead::Vector3f* endParam) const {
    startParam->set(mStartParam);
    endParam->set(mEndParam);
}

void RateParamV3f::calcLerpValue(sead::Vector3f* outVec, f32 t) const {
    lerpVec(outVec, mStartParam, mEndParam, t);
}

void RateParamV3f::calcEaseInValue(sead::Vector3f* outVec, f32 t) const {
    calcLerpValue(outVec, easeIn(t));
}

void RateParamV3f::calcEaseOutValue(sead::Vector3f* outVec, f32 t) const {
    calcLerpValue(outVec, easeOut(t));
}

void RateParamV3f::calcEaseInOutValue(sead::Vector3f* outVec, f32 t) const {
    calcLerpValue(outVec, easeInOut(t));
}

RateParamF32::RateParamF32() = default;

void RateParamF32::setParam(f32 startParam, f32 endParam) {
    mStartParam = startParam;
    mEndParam = endParam;
}

f32 RateParamF32::calcLerpValue(f32 t) const {
    return lerpValue(mStartParam, mEndParam, t);
}

f32 RateParamF32::calcEaseInValue(f32 t) const {
    return calcLerpValue(easeIn(t));
}

f32 RateParamF32::calcEaseOutValue(f32 t) const {
    return calcLerpValue(easeOut(t));
}

f32 RateParamF32::calcEaseInOutValue(f32 t) const {
    return calcLerpValue(easeInOut(t));
}

}  // namespace al
