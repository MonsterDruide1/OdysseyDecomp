#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>

namespace al {
class ModelKeeper;
}

namespace alModelJointFunction {
void setDirectJointMtx(al::ModelKeeper* modelKeeper, s32 jointIndex,
                       const sead::Matrix34f& jointMtx);
}  // namespace alModelJointFunction
