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

    bool isRequestDemoAction() const { return !mIsDemoActionStarted && mDemoActionName != nullptr; }

    bool isPlayingDemoAction() const { return mIsDemoActionStarted; }

    const char* getDemoActionName() const { return mDemoActionName; }

    bool isExistDemoTalkAction() const { return mDemoTalkActionName != nullptr; }

    const char* getDemoTalkActionName() const { return mDemoTalkActionName; }

    void setDemoTalkActionName(const char* actionName) { mDemoTalkActionName = actionName; }

    void resetDemoTalkActionName() { mDemoTalkActionName = nullptr; }

    void setPlayerDemoActionName(const char* actionName) { mPlayerDemoActionName = actionName; }

    const char* getPlayerDemoActionName() const { return mPlayerDemoActionName; }

    bool isRequestDemoCamera() const {
        return !mIsDemoCameraStarted && mDemoCameraAnimName != nullptr;
    }

    const char* getDemoCameraAnimName() const { return mDemoCameraAnimName; }

    void clearDemoCameraRequest() { mDemoCameraAnimName = nullptr; }

    bool isHideDemoPlayer() const { return mIsHideDemoPlayer; }

    void requestHideDemoPlayer() { mIsRequestHideDemoPlayer = true; }

    void requestShowDemoPlayer() { mIsRequestShowDemoPlayer = true; }

    void setResetDemoPlayerDynamics() { mIsResetDemoPlayerDynamics = true; }

    bool isResetDemoPlayerDynamics() const { return mIsResetDemoPlayerDynamics; }

    void startDemoAction() { mIsDemoActionStarted = true; }

    void endDemoAction() {
        mIsDemoActionStarted = false;
        mDemoActionName = nullptr;
    }

private:
    bool mIsDemoActionStarted;
    bool mIsResetDemoPlayerDynamics;
    u16 _02;
    u32 _04;
    const char* mDemoActionName;
    const char* mDemoTalkActionName;
    const char* mPlayerDemoActionName;
    bool mIsDemoCameraStarted;
    u8 _21;
    u16 _22;
    u32 _24;
    const char* mDemoCameraAnimName;
    bool mIsHideDemoPlayer;
    bool mIsRequestHideDemoPlayer;
    bool mIsRequestShowDemoPlayer;
    u8 _33;
    u32 _34;
};

bool isNodeName(const EventFlowNode* node, const char* nodeName);
const char16* getScareMessage(const EventFlowNode* node);
void startEventCamera(EventFlowNode* node, const char* cameraName, s32 priority);
void startEventAnimCamera(EventFlowNode* node, const char* cameraName, const char* animName,
                          s32 priority);
void endEventCamera(EventFlowNode* node, const char* cameraName, s32 priority, bool isInterpole);
bool tryEndEventCameraIfPlaying(EventFlowNode* node, const char* cameraName, s32 priority,
                                bool isInterpole);
bool isEndInterpoleEventCamera(const EventFlowNode* node, const char* cameraName);
bool isPlayingEventAnimCamera(const EventFlowNode* node, const char* cameraName);
void requestDemoAction(EventFlowNode* node, const char* actionName, bool isResetPlayerDynamics);
void requestPlayerDemoAction(EventFlowNode* node, const char* actionName);
bool isEndDemoAction(const EventFlowNode* node);
void setDemoTalkAction(EventFlowNode* node, const char* actionName);
void resetDemoTalkAction(EventFlowNode* node);
void requestDemoCamera(EventFlowNode* node, const char* cameraName);
bool isEndRequestDemoCamera(const EventFlowNode* node);
bool isHideDemoPlayer(const EventFlowNode* node);
void requestHideDemoPlayer(EventFlowNode* node);
void requestShowDemoPlayer(EventFlowNode* node);
void calcPlayerWatchTrans(sead::Vector3f* outWatchTrans, const EventFlowNode* node);
void requestCommandCloseTalkMessageLayout(EventFlowNode* node);
bool isActive(const EventFlowExecutor* executor);
bool isScare(const IUseEventFlowData* user);
bool tryGetScareEnemyPos(sead::Vector3f* outScareEnemyPos, const IUseEventFlowData* user);
void invalidateUiCollisionCheck(EventFlowExecutor* executor);
void onExistLookAtJointCtrl(EventFlowExecutor* executor);
void setTalkSubActorName(EventFlowExecutor* executor, const char* actorName);
void resetTalkSubActorName(EventFlowExecutor* executor);
void startEventAction(LiveActor* actor, const IUseEventFlowData* user, const char* actionName);
void makeEventActionName(sead::BufferedSafeStringBase<char>* outActionName,
                         const IUseEventFlowData* user, const char* actionName);
void startEventActionAtRandomFrame(LiveActor* actor, const IUseEventFlowData* user,
                                   const char* actionName);
bool tryStartEventActionIfNotPlaying(LiveActor* actor, const IUseEventFlowData* user,
                                     const char* actionName);
bool isPlayingEventAction(const LiveActor* actor, const IUseEventFlowData* user,
                          const char* actionName);
bool isExistEventAction(const LiveActor* actor, const IUseEventFlowData* user,
                        const char* actionName);
bool isOneTimeEventAction(const LiveActor* actor, const IUseEventFlowData* user,
                          const char* actionName);
f32 getEventActionFrameMax(const LiveActor* actor, const IUseEventFlowData* user,
                           const char* actionName);
f32 getParamMoveSpeed(const IUseEventFlowData* user);
f32 getParamTurnSpeedDegree(const IUseEventFlowData* user);
const sead::Vector3f& getRecordActorFront(const IUseEventFlowData* user);
void recordActorFront(IUseEventFlowData* user, const sead::Vector3f& front);
bool checkInsideTerritoryPos(const IUseEventFlowData* user, const LiveActor* actor,
                             const sead::Vector3f& position, f32 radiusOffset);
bool checkInsideEmotionTerritoryPos(const IUseEventFlowData* user, const LiveActor* actor,
                                    const sead::Vector3f& position);
bool checkInsideEmotionLowPriorityTerritoryPos(const IUseEventFlowData* user,
                                               const LiveActor* actor,
                                               const sead::Vector3f& position);
bool checkInsideTalkTerritoryPos(const IUseEventFlowData* user, const LiveActor* actor,
                                 const sead::Vector3f& position);
void calcBalloonOffset(sead::Vector3f* outOffset, const IUseEventFlowData* user,
                       const LiveActor* actor);
void setBalloonLocalOffset(EventFlowExecutor* executor, const sead::Vector3f& localOffset);
f32 getBalloonCollisionCheckOffsetRadius(const IUseEventFlowData* user);
bool isDisableOpenBalloonByOrderGroup(const LiveActor* actor, const IUseEventFlowData* user);
bool isEnableForceOpenBalloonByOrderGroup(const LiveActor* actor, const IUseEventFlowData* user);
bool isWaitAtPointMovement(const EventFlowExecutor* executor);
bool isStopMovement(const IUseEventFlowData* user);
bool isRequestEventDemoAction(const EventFlowExecutor* executor);
bool isPlayingEventDemoAction(const EventFlowExecutor* executor);
const char* getEventDemoActionName(const EventFlowExecutor* executor);
bool isExistEventDemoTalkAction(const EventFlowExecutor* executor);
const char* getEventDemoTalkActionName(const EventFlowExecutor* executor);
bool isResetEventDemoPlayerDynamics(const EventFlowExecutor* executor);
void startEventDemoAction(EventFlowExecutor* executor);
void endEventDemoAction(EventFlowExecutor* executor);
bool isRequestEventDemoCamera(const EventFlowExecutor* executor);
const char* getEventDemoCameraAnimName(const EventFlowExecutor* executor);
void clearEventDemoCameraRequest(EventFlowExecutor* executor);
bool isHideDemoPlayer(const EventFlowExecutor* executor);
bool isExistEventEntry(const EventFlowExecutor* executor, const char* entryName);
bool isCurrentEventEntry(const EventFlowExecutor* executor, const char* entryName);
bool isEventName(const EventFlowEventData* eventData, const char* format, ...);
const char* getEventName(const EventFlowEventData* eventData);
const char* getEventDataParamString(const EventFlowEventData* eventData, const char* key);
bool isEventDataParamBool(const EventFlowEventData* eventData, const char* key);
}  // namespace al

namespace alEventFlowFunction {
bool isReceiveCommandCloseTalkMessageLayout(const al::SceneEventFlowMsg* sceneEventFlowMsg);
void clearSceneEventFlowMsg(al::SceneEventFlowMsg* sceneEventFlowMsg);
}  // namespace alEventFlowFunction

static_assert(sizeof(al::EventFlowRequestInfo) == 0x38);
