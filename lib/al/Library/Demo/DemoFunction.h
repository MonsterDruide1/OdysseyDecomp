#pragma once

#include <basis/seadTypes.h>

namespace al {
class AddDemoInfo;
struct ActorInitInfo;
class DemoActorHolder;
class LiveActor;
class Scene;

AddDemoInfo* registDemoRequesterToAddDemoInfo(const LiveActor* actor, const ActorInitInfo& initInfo,
                                              s32 index);
void registActorToDemoInfo(LiveActor* actor, const ActorInitInfo& initInfo);
void addDemoActorFromAddDemoInfo(const LiveActor* actor, const AddDemoInfo* info);
void addDemoActorFromDemoActorHolder(const LiveActor* actor, const DemoActorHolder* holder);
void addDemoActorFromDemoActorHolder(const Scene* scene, const DemoActorHolder* holder);
void setDemoInfoDemoName(const LiveActor* actor, const char* name);
void killForceBeforeDemo(LiveActor* actor);
void prepareSkip(LiveActor* actor, s32);
void invalidateLODWithSubActor(LiveActor*);
bool isActiveDemo(const Scene*);

}  // namespace al

namespace alDemoUtilTmp {
bool isActiveDemo(const al::LiveActor*);
}
