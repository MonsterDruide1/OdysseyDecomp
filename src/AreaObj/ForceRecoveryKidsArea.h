#pragma once

#include "al/include/Library/Area/AreaObj.h"
#include "math/seadVectorFwd.h"

class ForceRecoveryKidsArea : public al::AreaObj {
public:
    ForceRecoveryKidsArea(const char* name);

    void init(const al::AreaInitInfo& areaInitInfo) override;

private:
    sead::Vector3f mTrans;
    sead::Vector3f mYRot;
};
