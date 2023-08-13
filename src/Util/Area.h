#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
}  // namespace al

namespace rs {
bool isInChangeStageArea(const al::LiveActor*, const sead::Vector3f*);
}  // namespace rs
