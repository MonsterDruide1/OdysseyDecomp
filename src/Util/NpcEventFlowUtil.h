#pragma once

namespace al {
struct ActorInitInfo;
class EventFlowExecutor;
class LiveActor;
class MessageTagDataHolder;
}  // namespace al

namespace rs {
al::EventFlowExecutor* initEventFlow(al::LiveActor*, const al::ActorInitInfo&, const char*,
                                     const char*);
void startEventFlow(al::EventFlowExecutor*, const char*);
bool updateEventFlow(al::EventFlowExecutor*);
void initEventMessageTagDataHolder(al::EventFlowExecutor*, const al::MessageTagDataHolder*);
void initEventCameraObject(al::EventFlowExecutor* flowExecutor, const al::ActorInitInfo& initInfo,
                           const char* name);
void initEventCameraObjectAfterKeepPose(al::EventFlowExecutor* flowExecutor,
                                        const al::ActorInitInfo& initInfo, const char* name);
}  // namespace rs
