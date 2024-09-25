#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;

bool isInWaterPos(const LiveActor* actor, const sead::Vector3f& pos);
bool isInIceWaterPos(const LiveActor* actor, const sead::Vector3f& pos);

}  // namespace al
