#include "Library/Se/Project/SePlayParam.h"

namespace al {

SePlayParam::SePlayParam() {
    reset();
}

void SePlayParam::reset() {
    mType = SePlayParamType::Invalid;
    mValue = 1.0f;
    mValue2 = 0.0f;
}

void SePlayParam::set(SePlayParamType type, f32 value, f32 value2) {
    mType = type;
    mValue = value;
    mValue2 = value2;
}

void SePlayParam::setMul(SePlayParamType type, f32 value, f32 value2) {
    f32 mul = mValue * value;
    mType = type;
    mValue2 = value2;
    mValue = mul;
}

}  // namespace al
