#pragma once

#include <basis/seadTypes.h>

namespace al {
class ByamlIter;

class InOutParam {
public:
    InOutParam();
    InOutParam(const InOutParam& src);
    InOutParam(f32 inMin, f32 inMax, f32 outMin, f32 outMax);
    void init(const ByamlIter& iter);
    f32 calcLeapValue(f32 value);
    f32 calcEaseInValue(f32 value);
    f32 calcEaseOutValue(f32 value);
    f32 calcEaseInOutValue(f32 value);
    f32 calcSqrtValue(f32 value);
    f32 calcSquareValue(f32 value);

private:
    f32 mInMin;
    f32 mInMax;
    f32 mOutMin;
    f32 mOutMax;
};
}  // namespace al
