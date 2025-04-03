#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
struct ActorInitInfo;
class LiveActor;
}  // namespace al

class WaterSurfaceShadow;

namespace rs {

f32 setShadowDropLength(al::LiveActor*, const al::ActorInitInfo&, const char*);
f32 calcShadowDropClippingCenter(sead::Vector3f*, const al::LiveActor*, f32, f32);
void setShadowDropOffset(al::LiveActor*, const char*, f32);
WaterSurfaceShadow* tryCreateWaterSurfaceShadow(const al::ActorInitInfo&, const char*);
WaterSurfaceShadow* tryCreateWaterSurfaceCoinShadow(const al::ActorInitInfo&);
void tryUpdateWaterSurfaceCoinShadow(WaterSurfaceShadow*, al::LiveActor*, f32);
void initHackShadow(al::LiveActor*);
void setupHackShadow(al::LiveActor*);
void endHackShadow(al::LiveActor*);

}  // namespace rs
