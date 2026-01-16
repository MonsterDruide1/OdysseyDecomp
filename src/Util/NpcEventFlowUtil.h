#pragma once

#include <basis/seadTypes.h>

namespace al {
struct ActorInitInfo;
class EventFlowExecutor;
class LiveActor;
class MessageTagDataHolder;
}  // namespace al

namespace rs {
al::EventFlowExecutor* initEventFlow(al::LiveActor*, const al::ActorInitInfo&, const char*,
                                     const char*);
al::EventFlowExecutor* initEventFlowSuffix(al::LiveActor*, const al::ActorInitInfo&, const char*,
                                           const char*, const char*);
void startEventFlow(al::EventFlowExecutor*, const char*);
bool updateEventFlow(al::EventFlowExecutor*);
void initEventMessageTagDataHolder(al::EventFlowExecutor*, const al::MessageTagDataHolder*);
void initEventCameraObject(al::EventFlowExecutor* flowExecutor, const al::ActorInitInfo& initInfo,
                           const char* name);
void initEventCameraObjectAfterKeepPose(al::EventFlowExecutor* flowExecutor,
                                        const al::ActorInitInfo& initInfo, const char* name);
void setEventBalloonFilterOnlyMiniGame(const al::LiveActor*);
void resetEventBalloonFilter(const al::LiveActor*);
void requestSwitchTalkNpcEventVolleyBall(al::LiveActor*, s32);
}  // namespace rs
