#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;

bool isInWaterPos(const LiveActor* actor, const sead::Vector3f& pos);
bool isInWater(const LiveActor* actor);
bool isInIceWaterPos(const LiveActor* actor, const sead::Vector3f& pos);

bool calcFindWaterSurface(sead::Vector3f*, sead::Vector3f*, const LiveActor*, const sead::Vector3f&,
                          const sead::Vector3f&, f32);

}  // namespace al
