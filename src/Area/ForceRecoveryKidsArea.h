#pragma once

#include <math/seadVector.h>

#include "Library/Area/AreaObj.h"

class ForceRecoveryKidsArea : public al::AreaObj {
public:
    ForceRecoveryKidsArea(const char* name);

    void init(const al::AreaInitInfo& areaInitInfo) override;

private:
    sead::Vector3f mTargetPos;
    sead::Vector3f mTargetUp;
};
