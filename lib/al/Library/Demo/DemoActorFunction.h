#pragma once

#include <basis/seadTypes.h>

namespace sead {
template <typename T>
class Matrix34;
using Matrix34f = Matrix34<f32>;
}  // namespace sead

namespace al {
struct ActorInitInfo;
class DemoActorHolder;

namespace alDemoFunction {
DemoActorHolder* createDemoActorHolder(const char* demoName, const ActorInitInfo& info,
                                       const sead::Matrix34f* baseMtx, s32 placementIndex,
                                       bool isDemoScene);
DemoActorHolder* createDemoActorHolderForDemoScene(const char* demoName, const ActorInitInfo& info,
                                                   const sead::Matrix34f* baseMtx,
                                                   s32 placementIndex);
}  // namespace alDemoFunction
}  // namespace al
