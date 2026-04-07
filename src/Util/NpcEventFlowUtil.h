#pragma once

#include <basis/seadTypes.h>

namespace al {
struct ActorInitInfo;
class EventFlowChoiceInfo;
class EventFlowNode;
class EventFlowExecutor;
class LiveActor;
class MessageTagDataHolder;
}  // namespace al

class Shine;

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
void startChoiceEvent(al::EventFlowNode* node, al::EventFlowChoiceInfo* info);
void startCloseNpcDemoEventTalkMessage(al::LiveActor*);
bool isCloseNpcDemoEventTalkMessage(const al::LiveActor*);
const char* getEventTalkWindowMessageStyle(const al::EventFlowNode* node);
void checkMatchPatternCurrentCostume(bool*, bool*, const al::LiveActor*, const char*);
bool checkMatchPatternCurrentCostumePair(const al::LiveActor*, const char*);
void requestEventGetShineDirect(al::EventFlowNode*, Shine*);
void startOpenNpcDemoEventSystemMessageWithButtonA(al::EventFlowNode*, const char16*, s32);
void startOpenNpcDemoEventTalkMessage(al::EventFlowNode*, const char16*);
void startOpenNpcDemoEventTalkMessageWithButtonA(al::EventFlowNode*, const char16*);
bool isEndNpcDemoEventTalkMessage(const al::EventFlowNode*);
void requestEventOpenBgmList(al::EventFlowNode*);
bool checkEndSceneExecuteAndResetRequest(al::EventFlowNode*);
bool isSuccessLockEventTalkDemo(const al::EventFlowNode*);
bool tryLockStartEventTalkDemo(al::EventFlowNode*);
bool tryLockStartEventTalkDemoWithoutBalloon(al::EventFlowNode*);
bool tryStartEventTalkDemo(al::EventFlowNode*);
bool tryStartEventTalkOnlyRequesterDemo(al::EventFlowNode*);
bool tryStartEventTalkKeepHackDemo(al::EventFlowNode*);
bool tryStartEventTalkUseCoinDemo(al::EventFlowNode*);
bool tryStartEventNormalDemo(al::EventFlowNode*);
bool tryStartEventKeepBindDemo(al::LiveActor*);
bool tryStartEventCutSceneDemo(al::LiveActor*);
bool tryStartEventCutSceneKeepHackDemo(al::LiveActor*);
bool tryStartEventCutSceneTalkOnlyRequesterDemo(al::LiveActor*);
void endEventCutSceneDemo(al::LiveActor*);
void endEventCutSceneTalkOnlyRequeterDemo(al::LiveActor*);
void endEventCutSceneDemoBySkip(al::LiveActor*);
void requestEndEventDemo(al::LiveActor*);
void requestEndEventDemo(al::EventFlowNode*);
bool isActiveEventDemo(const al::EventFlowNode*);
bool isActiveEventDemo(const al::LiveActor*);
bool isEqualEventDemoStartActor(const al::LiveActor*);
bool isCloseEventWipeFadeBlack(const al::EventFlowNode*);
bool isOpenEventWipeFadeBlack(const al::EventFlowNode*);
void requestCloseEventWipeFadeBlack(al::EventFlowNode*, s32);
void requestOpenEventWipeFadeBlack(al::EventFlowNode*, s32);
void requestSwitchTalkNpcEventVolleyBall(al::LiveActor*, s32);
bool isEventAfterDoorSnow1(const al::EventFlowNode*);
bool isEventAfterDoorSnow2(const al::EventFlowNode*);
bool isEventAfterDoorSnow3(const al::EventFlowNode*);
bool isEventAfterDoorSnow4(const al::EventFlowNode*);
}  // namespace rs
