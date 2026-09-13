#pragma once

#include <basis/seadTypes.h>

namespace al {
class FireSurfaceFinder;
class LiveActor;
class WaterSurfaceFinder;
}  // namespace al

namespace LifeUpItemFunction {
bool updateSurfaceFinder(al::WaterSurfaceFinder* surfaceFinder, const al::LiveActor* actor,
                         f32 distance);
bool updateSurfaceFinder(al::FireSurfaceFinder* surfaceFinder, const al::LiveActor* actor,
                         f32 distance);
void updateVelocityInWaterAndFire(al::LiveActor* actor, al::WaterSurfaceFinder* waterSurfaceFinder,
                                  al::FireSurfaceFinder* fireSurfaceFinder, f32 surfaceHeight,
                                  f32 gravityAccel);
}  // namespace LifeUpItemFunction
