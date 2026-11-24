#pragma once

#include <math/seadVector.h>

class GotogotonMark {
public:
    GotogotonMark();

    bool isMatch(const GotogotonMark* other) const;

    sead::Vector3f mFrontDir = {0.0f, 0.0f, 1.0f};
    s32 mId = 0;
};
