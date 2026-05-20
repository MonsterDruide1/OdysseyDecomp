#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>

namespace al {
struct ActorInitInfo;
class DemoActorHolder;

namespace alDemoFunction {
DemoActorHolder* createDemoActorHolder(const char* demoName, const ActorInitInfo& initInfo,
                                       const sead::Matrix34f* mtx, s32 index, bool isCreateDummy);
DemoActorHolder* createDemoActorHolderForDemoScene(const char* demoName,
                                                   const ActorInitInfo& initInfo,
                                                   const sead::Matrix34f* mtx, s32 index);
}  // namespace alDemoFunction

}  // namespace al
