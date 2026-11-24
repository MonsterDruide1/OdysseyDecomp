#pragma once

#include <math/seadVector.h>

class GotogotonMark {
public:
    GotogotonMark();

    bool isMatch(const GotogotonMark* other) const;

    sead::Vector3f* getFrontDirPtr() { return &mFrontDir; }

    s32* getIdPtr() { return &mId; }

private:
    sead::Vector3f mFrontDir = sead::Vector3f::ez;
    s32 mId = 0;
};

static_assert(sizeof(GotogotonMark) == 0x10);
