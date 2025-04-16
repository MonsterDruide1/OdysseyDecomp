#pragma once

namespace al {
class ByamlIter;

class InOutParam {
public:
    InOutParam();
    InOutParam(const InOutParam& src);
    InOutParam(f32 inMin, f32 inMax, f32 outMin, f32 outMax);
    void init(const ByamlIter& iter);
    f32 calcLeapValue(f32 value) const;
    f32 calcEaseInValue(f32 value) const;
    f32 calcEaseOutValue(f32 value) const;
    f32 calcEaseInOutValue(f32 value) const;
    f32 calcSqrtValue(f32 value) const;
    f32 calcSquareValue(f32 value) const;

private:
    f32 mInMin = 0.0f;
    f32 mInMax = 0.0f;
    f32 mOutMin = 0.0f;
    f32 mOutMax = 0.0f;
};
}  // namespace al
