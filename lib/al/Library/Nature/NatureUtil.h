#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
class WaterSurfaceFinder;

bool isInWaterPos(const LiveActor* actor, const sead::Vector3f& pos);
bool isInWater(const LiveActor* actor);
bool isInIceWaterPos(const LiveActor* actor, const sead::Vector3f& pos);
bool tryAddRippleMiddle(LiveActor* actor);

bool calcFindWaterSurface(sead::Vector3f*, sead::Vector3f*, const LiveActor*, const sead::Vector3f&,
                          const sead::Vector3f&, f32);
bool calcFindWaterSurfaceFlat(sead::Vector3f*, sead::Vector3f*, const LiveActor*,
                              const sead::Vector3f&, const sead::Vector3f&, f32);
bool calcFindWaterSurfaceDisplacement(sead::Vector3f*, sead::Vector3f*, const LiveActor*,
                                      const sead::Vector3f&, const sead::Vector3f&, f32);
bool calcFindWaterSurfaceOverGround(sead::Vector3f*, sead::Vector3f*, const LiveActor*,
                                    const sead::Vector3f&, const sead::Vector3f&, f32);
void approachWaterSurfaceSpringDumper(LiveActor*, const WaterSurfaceFinder*, f32, f32, f32, f32,
                                      f32);
}  // namespace al
