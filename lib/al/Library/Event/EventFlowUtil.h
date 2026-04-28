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
    void requestDemoAction(const char* actionName);
    void requestDemoCamera(const char* cameraName);

    const char* getRequestedDemoPlayerActionName() const { return mDemoPlayerActionName; }

    void clearRequestedDemoPlayerActionName() { mDemoPlayerActionName = nullptr; }

    bool isDemoPlayerHidden() const { return mIsDemoPlayerHidden; }

    void setDemoPlayerHidden(bool isHidden) { mIsDemoPlayerHidden = isHidden; }

    bool isRequestHideDemoPlayer() const { return mIsRequestHideDemoPlayer; }

    void resetRequestHideDemoPlayer() { mIsRequestHideDemoPlayer = false; }

    bool isRequestShowDemoPlayer() const { return mIsRequestShowDemoPlayer; }

    void resetRequestShowDemoPlayer() { mIsRequestShowDemoPlayer = false; }

private:
    bool _0 = false;
    bool _1 = false;
    char _2[6] = {};
    const char* mDemoActionName = nullptr;
    const char* _10 = nullptr;
    const char* mDemoPlayerActionName = nullptr;
    const char* mDemoCameraName = nullptr;
    void* _28 = nullptr;
    bool mIsDemoPlayerHidden = false;
    bool mIsRequestHideDemoPlayer = false;
    bool mIsRequestShowDemoPlayer = false;
    char _33[5] = {};
};

static_assert(sizeof(EventFlowRequestInfo) == 0x38);

bool isNodeName(const EventFlowNode*, const char*);
const char16* getScareMessage(const EventFlowNode*);
void startEventCamera(EventFlowNode*, const char*, s32);
void startEventAnimCamera(EventFlowNode*, const char*, const char*, s32);
void endEventCamera(EventFlowNode*, const char*, s32, bool);
bool tryEndEventCameraIfPlaying(EventFlowNode*, const char*, s32, bool);
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
bool tryGetScareEnemyPos(sead::Vector3f*, const IUseEventFlowData*);
void invalidateUiCollisionCheck(EventFlowExecutor*);
void onExistLookAtJointCtrl(EventFlowExecutor*);
void setTalkSubActorName(EventFlowExecutor*, const char*);
void resetTalkSubActorName(EventFlowExecutor*);
void startEventAction(LiveActor*, const IUseEventFlowData*, const char*);
void makeEventActionName(sead::BufferedSafeStringBase<char>*, const IUseEventFlowData*,
                         const char*);
void startEventActionAtRandomFrame(LiveActor*, const IUseEventFlowData*, const char*);
bool tryStartEventActionIfNotPlaying(LiveActor*, const IUseEventFlowData*, const char*);
bool isPlayingEventAction(const LiveActor*, const IUseEventFlowData*, const char*);
bool isExistEventAction(const LiveActor*, const IUseEventFlowData*, const char*);
bool isOneTimeEventAction(const LiveActor*, const IUseEventFlowData*, const char*);
f32 getEventActionFrameMax(const LiveActor*, const IUseEventFlowData*, const char*);
f32 getParamMoveSpeed(const IUseEventFlowData*);
f32 getParamTurnSpeedDegree(const IUseEventFlowData*);
const sead::Vector3f& getRecordActorFront(const IUseEventFlowData*);
void recordActorFront(IUseEventFlowData*, const sead::Vector3f&);
bool checkInsideTerritoryPos(const IUseEventFlowData*, const LiveActor*, const sead::Vector3f&,
                             f32);
bool checkInsideEmotionTerritoryPos(const IUseEventFlowData*, const LiveActor*,
                                    const sead::Vector3f&);
bool checkInsideEmotionLowPriorityTerritoryPos(const IUseEventFlowData*, const LiveActor*,
                                               const sead::Vector3f&);
bool checkInsideTalkTerritoryPos(const IUseEventFlowData*, const LiveActor*, const sead::Vector3f&);
void calcBalloonOffset(sead::Vector3f*, const IUseEventFlowData*, const LiveActor*);
void setBalloonLocalOffset(EventFlowExecutor*, const sead::Vector3f&);
f32 getBalloonCollisionCheckOffsetRadius(const IUseEventFlowData*);
bool isDisableOpenBalloonByOrderGroup(const LiveActor*, const IUseEventFlowData*);
bool isEnableForceOpenBalloonByOrderGroup(const LiveActor*, const IUseEventFlowData*);
bool isWaitAtPointMovement(const EventFlowExecutor*);
bool isStopMovement(const IUseEventFlowData*);
bool isRequestEventDemoAction(const EventFlowExecutor*);
bool isPlayingEventDemoAction(const EventFlowExecutor*);
const char* getEventDemoActionName(const EventFlowExecutor*);
bool isExistEventDemoTalkAction(const EventFlowExecutor*);
const char* getEventDemoTalkActionName(const EventFlowExecutor*);
bool isResetEventDemoPlayerDynamics(const EventFlowExecutor*);
void startEventDemoAction(EventFlowExecutor*);
void endEventDemoAction(EventFlowExecutor*);
bool isRequestEventDemoCamera(const EventFlowExecutor*);
const char* getEventDemoCameraAnimName(const EventFlowExecutor*);
void clearEventDemoCameraRequest(EventFlowExecutor*);
bool isHideDemoPlayer(const EventFlowExecutor*);
bool isExistEventEntry(const EventFlowExecutor*, const char*);
bool isCurrentEventEntry(const EventFlowExecutor*, const char*);
bool isEventName(const EventFlowEventData*, const char*, ...);
const char* getEventName(const EventFlowEventData*);
const char* getEventDataParamString(const EventFlowEventData*, const char*);
bool isEventDataParamBool(const EventFlowEventData*, const char*);
}  // namespace al

namespace alEventFlowFunction {
bool isReceiveCommandCloseTalkMessageLayout(const al::SceneEventFlowMsg*);
void clearSceneEventFlowMsg(al::SceneEventFlowMsg*);
}  // namespace alEventFlowFunction
