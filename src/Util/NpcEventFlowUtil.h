#pragma once

#include <prim/seadSafeString.h>

namespace al {
class ActorInitInfo;
class EventFlowExecutor;
class LiveActor;
class MessageTagDataHolder;
}  // namespace al

namespace rs {
al::EventFlowExecutor* initEventFlow(al::LiveActor*, const al::ActorInitInfo&, const char*,
                                     const char*);
void startEventFlow(al::EventFlowExecutor*, const char*);
void updateEventFlow(al::EventFlowExecutor*);
void initEventMessageTagDataHolder(al::EventFlowExecutor*, const al::MessageTagDataHolder*);
}  // namespace rs
