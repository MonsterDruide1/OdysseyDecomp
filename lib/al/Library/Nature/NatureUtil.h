#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;

bool isInWaterPos(const LiveActor*, const sead::Vector3f&);
bool isInIceWaterPos(const LiveActor*, const sead::Vector3f&);

}  // namespace al
