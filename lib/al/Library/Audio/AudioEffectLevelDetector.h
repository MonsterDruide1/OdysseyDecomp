#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class AudioEffectLevelDetector{
public:
    const sead::Vector2f& getLevel() const { return mLevel; }

private:
    char filler_68[0x30];
    sead::Vector2f mLevel;
    char filler_a0[0x20];
};
}  // namespace al
