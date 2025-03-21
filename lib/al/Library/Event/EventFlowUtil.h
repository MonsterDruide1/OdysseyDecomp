#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>

namespace al {
class EventFlowNode;
class EventFlowEventData;
class SceneEventFlowMsg;
class LiveActor;
class EventFlowExecutor;
class IUseEventFlowData;

class EventFlowRequestInfo {
public:
    EventFlowRequestInfo();
    void reset();
    void requestDemoAction(const char*);
    void requestDemoCamera(const char*);
};

bool isNodeName(const EventFlowNode*, const char*);
void getScareMessage(const EventFlowNode*);
void startEventCamera(EventFlowNode*, const char*, s32);
void startEventAnimCamera(EventFlowNode*, const char*, const char*, s32);
void endEventCamera(EventFlowNode*, const char*, s32, bool);
void tryEndEventCameraIfPlaying(EventFlowNode*, const char*, s32, bool);
bool isEndInterpoleEventCamera(const EventFlowNode*, const char*);
bool isPlayingEventAnimCamera(const EventFlowNode*, const char*);
void requestDemoAction(EventFlowNode*, const char*, bool);
void requestPlayerDemoAction(EventFlowNode*, const char*);
bool isEndDemoAction(const EventFlowNode*);
void setDemoTalkAction(EventFlowNode*, const char*);
void resetDemoTalkAction(EventFlowNode*);
void requestDemoCamera(EventFlowNode*, const char*);
bool isEndRequestDemoCamera(const EventFlowNode*);
bool isHideDemoPlayer(const EventFlowNode*);
void requestHideDemoPlayer(EventFlowNode*);
void requestShowDemoPlayer(EventFlowNode*);
void calcPlayerWatchTrans(sead::Vector3f*, const EventFlowNode*);
void requestCommandCloseTalkMessageLayout(EventFlowNode*);
bool isActive(const EventFlowExecutor*);
bool isScare(const IUseEventFlowData*);
void tryGetScareEnemyPos(sead::Vector3f*, const IUseEventFlowData*);
void invalidateUiCollisionCheck(EventFlowExecutor*);
void onExistLookAtJointCtrl(EventFlowExecutor*);
void setTalkSubActorName(EventFlowExecutor*, const char*);
void resetTalkSubActorName(EventFlowExecutor*);
void startEventAction(LiveActor*, const IUseEventFlowData*, const char*);
void makeEventActionName(sead::BufferedSafeStringBase<char>*, const IUseEventFlowData*,
                         const char*);
void startEventActionAtRandomFrame(LiveActor*, const IUseEventFlowData*, const char*);
void tryStartEventActionIfNotPlaying(LiveActor*, const IUseEventFlowData*, const char*);
bool isPlayingEventAction(const LiveActor*, const IUseEventFlowData*, const char*);
bool isExistEventAction(const LiveActor*, const IUseEventFlowData*, const char*);
bool isOneTimeEventAction(const LiveActor*, const IUseEventFlowData*, const char*);
void getEventActionFrameMax(const LiveActor*, const IUseEventFlowData*, const char*);
void getParamMoveSpeed(const IUseEventFlowData*);
void getParamTurnSpeedDegree(const IUseEventFlowData*);
void getRecordActorFront(const IUseEventFlowData*);
void recordActorFront(IUseEventFlowData*, const sead::Vector3f&);
void checkInsideTerritoryPos(const IUseEventFlowData*, const LiveActor*, const sead::Vector3f&,
                             f32);
void checkInsideEmotionTerritoryPos(const IUseEventFlowData*, const LiveActor*,
                                    const sead::Vector3f&);
void checkInsideEmotionLowPriorityTerritoryPos(const IUseEventFlowData*, const LiveActor*,
                                               const sead::Vector3f&);
void checkInsideTalkTerritoryPos(const IUseEventFlowData*, const LiveActor*, const sead::Vector3f&);
void calcBalloonOffset(sead::Vector3f*, const IUseEventFlowData*, const LiveActor*);
void setBalloonLocalOffset(EventFlowExecutor*, const sead::Vector3f&);
void getBalloonCollisionCheckOffsetRadius(const IUseEventFlowData*);
bool isDisableOpenBalloonByOrderGroup(const LiveActor*, const IUseEventFlowData*);
bool isEnableForceOpenBalloonByOrderGroup(const LiveActor*, const IUseEventFlowData*);
bool isWaitAtPointMovement(const EventFlowExecutor*);
bool isStopMovement(const IUseEventFlowData*);
bool isRequestEventDemoAction(const EventFlowExecutor*);
bool isPlayingEventDemoAction(const EventFlowExecutor*);
void getEventDemoActionName(const EventFlowExecutor*);
bool isExistEventDemoTalkAction(const EventFlowExecutor*);
void getEventDemoTalkActionName(const EventFlowExecutor*);
bool isResetEventDemoPlayerDynamics(const EventFlowExecutor*);
void startEventDemoAction(EventFlowExecutor*);
void endEventDemoAction(EventFlowExecutor*);
bool isRequestEventDemoCamera(const EventFlowExecutor*);
void getEventDemoCameraAnimName(const EventFlowExecutor*);
void clearEventDemoCameraRequest(EventFlowExecutor*);
bool isHideDemoPlayer(const EventFlowExecutor*);
bool isExistEventEntry(const EventFlowExecutor*, const char*);
bool isCurrentEventEntry(const EventFlowExecutor*, const char*);
bool isEventName(const EventFlowEventData*, const char*, ...);
void getEventName(const EventFlowEventData*);
void getEventDataParamString(const EventFlowEventData*, const char*);
bool isEventDataParamBool(const EventFlowEventData*, const char*);
}  // namespace al

namespace alEventFlowFunction {
bool isReceiveCommandCloseTalkMessageLayout(const al::SceneEventFlowMsg*);
void clearSceneEventFlowMsg(al::SceneEventFlowMsg*);
}  // namespace alEventFlowFunction
