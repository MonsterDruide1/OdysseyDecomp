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
bool isDefinedEventCamera(const al::EventFlowExecutor*, const char*);
void initEventCameraObject(al::EventFlowExecutor*, const al::ActorInitInfo&, const char*);
void initEventCameraObjectAfterKeepPose(al::EventFlowExecutor*, const al::ActorInitInfo&,
                                        const char*);
void initEventMessageTagDataHolder(al::EventFlowExecutor*, const al::MessageTagDataHolder*);
void setEventBalloonFilterOnlyMiniGame(const al::LiveActor*);
void resetEventBalloonFilter(const al::LiveActor*);
bool tryStartEventCutSceneDemo(al::LiveActor*);
void endEventCutSceneDemoOrTryEndEventCutSceneDemoBySkip(al::LiveActor*);
void tryHideDemoPlayerIfRequested(al::LiveActor*, al::EventFlowExecutor*);
void tryShowDemoPlayerIfRequested(al::LiveActor*, al::EventFlowExecutor*);
void tryStartDemoPlayerActionIfRequested(al::LiveActor*, al::EventFlowExecutor*);
bool isPlayingTextPaneAnimEventTalkMessage(const al::LiveActor*);
bool isCloseNpcDemoEventTalkMessage(const al::LiveActor*);
void requestSwitchTalkNpcEventVolleyBall(al::LiveActor*, s32);
void skipEventDemo(al::EventFlowExecutor*);
}  // namespace rs
