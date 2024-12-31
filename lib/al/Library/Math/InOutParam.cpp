#include "Library/Math/InOutParam.h"

#include "Library/Math/MathUtil.h"
#include "Library/Yaml/ByamlIter.h"

namespace al {

InOutParam::InOutParam() = default;

InOutParam::InOutParam(const InOutParam& src)
    : mInMin(src.mInMin), mInMax(src.mInMax), mOutMin(src.mOutMin), mOutMax(src.mOutMax) {}

InOutParam::InOutParam(f32 inMin, f32 inMax, f32 outMin, f32 outMax)
    : mInMin(inMin), mInMax(inMax), mOutMin(outMin), mOutMax(outMax) {}

void InOutParam::init(const ByamlIter& iter) {
    iter.tryGetFloatByKey(&mInMin, "InMin");
    iter.tryGetFloatByKey(&mInMax, "InMax");
    iter.tryGetFloatByKey(&mOutMin, "OutMin");
    iter.tryGetFloatByKey(&mOutMax, "OutMax");
}

f32 InOutParam::calcLeapValue(f32 value) const {
    return lerpValue(value, mInMin, mInMax, mOutMin, mOutMax);
}

f32 InOutParam::calcEaseInValue(f32 value) const {
    f32 lerp = lerpValue(value, mInMin, mInMax, 0.0f, 1.0f);
    f32 eased = easeIn(lerp);
    return mOutMin + eased * (mOutMax - mOutMin);
}

f32 InOutParam::calcEaseOutValue(f32 value) const {
    f32 lerp = lerpValue(value, mInMin, mInMax, 0.0f, 1.0f);
    f32 eased = easeOut(lerp);
    return mOutMin + eased * (mOutMax - mOutMin);
}

f32 InOutParam::calcEaseInOutValue(f32 value) const {
    f32 lerp = lerpValue(value, mInMin, mInMax, 0.0f, 1.0f);
    f32 eased = easeInOut(lerp);
    return mOutMin + eased * (mOutMax - mOutMin);
}

f32 InOutParam::calcSqrtValue(f32 value) const {
    f32 lerp = lerpValue(value, mInMin, mInMax, 0.0f, 1.0f);
    f32 eased = sead::Mathf::sqrt(lerp);
    return mOutMin + eased * (mOutMax - mOutMin);
}

f32 InOutParam::calcSquareValue(f32 value) const {
    f32 lerp = lerpValue(value, mInMin, mInMax, 0.0f, 1.0f);
    f32 eased = sead::Mathf::square(lerp);
    return mOutMin + eased * (mOutMax - mOutMin);
}

}  // namespace al
