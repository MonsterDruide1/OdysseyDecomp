#pragma once

#include <math/seadVector.h>

namespace al {

struct CameraPoseInfo {
    sead::Vector3f pos;
    sead::Vector3f at;
    sead::Vector3f up;
};

static_assert(sizeof(CameraPoseInfo) == 0x24);

}  // namespace al
