#pragma once

#include <basis/seadTypes.h>

namespace al {

enum class SePlayParamType {
    Invalid,
    Volume,
    Pitch,
    SeqTempoRatio,
    EffectSend,
    SeqLocalVariable,
    SeqGlobalVariable,
    LpfFreq,
    BiquadFilter,
};

class SePlayParam {
public:
    SePlayParam();

    void reset();
    void set(SePlayParamType type, f32 value, f32 value2);
    void setMul(SePlayParamType type, f32 value, f32 value2);

private:
    SePlayParamType mType;
    f32 mValue;
    f32 mValue2;
};

static_assert(sizeof(SePlayParam) == 0xC);

}  // namespace al
