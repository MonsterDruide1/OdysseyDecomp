#pragma once

#include <basis/seadTypes.h>

namespace al {
struct ActorInitInfo;
class EventFlowExecutor;
class IEventFlowActionNameConverter;
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
void initEventMovementTurnSeparate(al::EventFlowExecutor*, const al::ActorInitInfo&);
void initEventActionNameConverter(al::EventFlowExecutor*, const al::IEventFlowActionNameConverter*);
void initEventCameraTalk(al::EventFlowExecutor*, const al::ActorInitInfo&, const char*, f32);
void initEventCharacterName(al::EventFlowExecutor*, const al::ActorInitInfo&, const char*);
void initEventCameraObject(al::EventFlowExecutor* flowExecutor, const al::ActorInitInfo& initInfo,
                           const char* name);
void initEventCameraObjectAfterKeepPose(al::EventFlowExecutor* flowExecutor,
                                        const al::ActorInitInfo& initInfo, const char* name);
void setEventBalloonFilterOnlyMiniGame(const al::LiveActor*);
void resetEventBalloonFilter(const al::LiveActor*);
void requestSwitchTalkNpcEventVolleyBall(al::LiveActor*, s32);
bool tryStartEventCutSceneKeepHackDemo(al::LiveActor*);
bool isActiveEventDemo(const al::LiveActor*);
bool isEqualEventDemoStartActor(const al::LiveActor*);
}  // namespace rs
