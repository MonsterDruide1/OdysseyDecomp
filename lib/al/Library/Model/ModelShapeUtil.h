#pragma once

#include <math/seadMatrix.h>

namespace al {
class ModelKeeper;

void setModelProjMtx0(const ModelKeeper*, const sead::Matrix44f&);

}  // namespace al
