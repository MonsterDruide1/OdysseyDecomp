#pragma once

#include <math/seadVector.h>

#include "Library/Area/AreaObj.h"

class ForceRecoveryKidsArea : public al::AreaObj {
public:
    ForceRecoveryKidsArea(const char* name);

    void init(const al::AreaInitInfo& info) override;

private:
    sead::Vector3f mRecoveryPos = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mRecoveryUp;
};
