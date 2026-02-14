#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
struct ActorInitInfo;
class LiveActor;
}  // namespace al

class WaterSurfaceShadow;

namespace rs {

f32 setShadowDropLength(al::LiveActor* actor, const al::ActorInitInfo& initInfo,
                        const char* maskName);
void calcShadowDropClippingCenter(sead::Vector3f* vec, const al::LiveActor* actor, f32 f, f32 f2);
void setShadowDropOffset(al::LiveActor* actor, const char* maskName, f32 distance);
WaterSurfaceShadow* tryCreateWaterSurfaceShadow(const al::ActorInitInfo& initInfo,
                                                const char* archiveName);
WaterSurfaceShadow* tryCreateWaterSurfaceCoinShadow(const al::ActorInitInfo& initInfo);
void tryUpdateWaterSurfaceCoinShadow(WaterSurfaceShadow* shadow, al::LiveActor* actor,
                                     f32 distance);
void initHackShadow(al::LiveActor* actor);
void setupHackShadow(al::LiveActor* actor);
void endHackShadow(al::LiveActor* actor);

}  // namespace rs
