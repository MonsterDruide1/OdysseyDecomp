#include "Library/Event/EventFlowUtil.h"

#include <math/seadQuat.h>

#include "Library/Base/StringUtil.h"
#include "Library/Event/BalloonOrderGroup.h"
#include "Library/Event/EventFlowActorParamHolder.h"
#include "Library/Event/EventFlowDataHolder.h"
#include "Library/Event/EventFlowExecutor.h"
#include "Library/Event/EventFlowNode.h"
#include "Library/Event/EventFlowScareCtrlBase.h"
#include "Library/Event/EventFlowWatchParam.h"
#include "Library/Event/SceneEventFlowMsg.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathUtil.h"
#include "Library/Yaml/ByamlUtil.h"
#include "Project/Event/EventFlowActorTerritory.h"
#include "Project/Event/EventFlowEventData.h"

namespace {
const char16 EmptyScareMessage[] = {0};
}

namespace al {
EventFlowRequestInfo::EventFlowRequestInfo() {
    mIsDemoActionStarted = false;
    mIsResetDemoPlayerDynamics = false;
    mIsDemoCameraStarted = false;
    mDemoActionName = nullptr;
    mIsRequestShowDemoPlayer = false;
    mIsHideDemoPlayer = false;
    mIsRequestHideDemoPlayer = false;
    mDemoCameraAnimName = nullptr;
    mDemoTalkActionName = nullptr;
    mPlayerDemoActionName = nullptr;
}

void EventFlowRequestInfo::reset() {
    mIsDemoActionStarted = false;
    mIsResetDemoPlayerDynamics = false;
    mIsDemoCameraStarted = false;
    mIsHideDemoPlayer = false;
    mIsRequestHideDemoPlayer = false;
    mDemoCameraAnimName = nullptr;
    mDemoActionName = nullptr;
    mDemoTalkActionName = nullptr;
}

__attribute__((noinline)) void EventFlowRequestInfo::requestDemoAction(const char* actionName) {
    mDemoActionName = actionName;
}

__attribute__((noinline)) void EventFlowRequestInfo::requestDemoCamera(const char* cameraName) {
    mDemoCameraAnimName = cameraName;
}

bool isNodeName(const EventFlowNode* node, const char* nodeName) {
    return isEqualString(node->getName(), nodeName);
}

const char16* getScareMessage(const EventFlowNode* node) {
    const IUseEventFlowData* eventFlowUser = node;
    if (eventFlowUser->getEventFlowDataHolder()->getScareCtrl() == nullptr)
        return EmptyScareMessage;
    return eventFlowUser->getEventFlowDataHolder()->getScareCtrl()->getScareMessage();
}

void startEventCamera(EventFlowNode* node, const char* cameraName, s32 priority) {
    IUseEventFlowData* eventFlowUser = node;
    EventFlowDataHolder* holder = eventFlowUser->getEventFlowDataHolder();
    LiveActor* actor = node->getActor();
    IUseCamera* cameraUser = actor != nullptr ? static_cast<IUseCamera*>(actor) : nullptr;
    holder->startEventCamera(cameraUser, cameraName, priority);
}

void startEventAnimCamera(EventFlowNode* node, const char* cameraName, const char* animName,
                          s32 priority) {
    IUseEventFlowData* eventFlowUser = node;
    EventFlowDataHolder* holder = eventFlowUser->getEventFlowDataHolder();
    LiveActor* actor = node->getActor();
    IUseCamera* cameraUser = actor != nullptr ? static_cast<IUseCamera*>(actor) : nullptr;
    holder->startEventAnimCamera(cameraUser, cameraName, animName, priority);
}

void endEventCamera(EventFlowNode* node, const char* cameraName, s32 priority, bool isInterpole) {
    IUseEventFlowData* eventFlowUser = node;
    EventFlowDataHolder* holder = eventFlowUser->getEventFlowDataHolder();
    LiveActor* actor = node->getActor();
    IUseCamera* cameraUser = actor != nullptr ? static_cast<IUseCamera*>(actor) : nullptr;
    holder->endEventCamera(cameraUser, cameraName, priority, isInterpole);
}

bool tryEndEventCameraIfPlaying(EventFlowNode* node, const char* cameraName, s32 priority,
                                bool isInterpole) {
    IUseEventFlowData* eventFlowUser = node;
    EventFlowDataHolder* holder = eventFlowUser->getEventFlowDataHolder();
    LiveActor* actor = node->getActor();
    IUseCamera* cameraUser = actor != nullptr ? static_cast<IUseCamera*>(actor) : nullptr;
    return holder->tryEndEventCameraIfPlaying(cameraUser, cameraName, priority, isInterpole);
}

bool isEndInterpoleEventCamera(const EventFlowNode* node, const char* cameraName) {
    const IUseEventFlowData* eventFlowUser = node;
    const EventFlowDataHolder* holder = eventFlowUser->getEventFlowDataHolder();
    const LiveActor* actor = node->getActor();
    const IUseCamera* cameraUser =
        actor != nullptr ? static_cast<const IUseCamera*>(actor) : nullptr;
    return holder->isEndInterpoleCamera(cameraUser, cameraName);
}

bool isPlayingEventAnimCamera(const EventFlowNode* node, const char* cameraName) {
    const IUseEventFlowData* eventFlowUser = node;
    return eventFlowUser->getEventFlowDataHolder()->isPlayingEventAnimCamera(cameraName);
}

void requestDemoAction(EventFlowNode* node, const char* actionName, bool isResetPlayerDynamics) {
    IUseEventFlowData* eventFlowUser = node;
    eventFlowUser->getEventFlowDataHolder()->getRequestInfo()->requestDemoAction(actionName);
    if (isResetPlayerDynamics)
        eventFlowUser->getEventFlowDataHolder()->getRequestInfo()->setResetDemoPlayerDynamics();
}

void requestPlayerDemoAction(EventFlowNode* node, const char* actionName) {
    IUseEventFlowData* eventFlowUser = node;
    eventFlowUser->getEventFlowDataHolder()->getRequestInfo()->setPlayerDemoActionName(actionName);
}

bool isEndDemoAction(const EventFlowNode* node) {
    const IUseEventFlowData* eventFlowUser = node;
    return eventFlowUser->getEventFlowDataHolder()->getRequestInfo()->getDemoActionName() ==
           nullptr;
}

void setDemoTalkAction(EventFlowNode* node, const char* actionName) {
    IUseEventFlowData* eventFlowUser = node;
    eventFlowUser->getEventFlowDataHolder()->getRequestInfo()->setDemoTalkActionName(actionName);
}

void resetDemoTalkAction(EventFlowNode* node) {
    IUseEventFlowData* eventFlowUser = node;
    eventFlowUser->getEventFlowDataHolder()->getRequestInfo()->resetDemoTalkActionName();
}

void requestDemoCamera(EventFlowNode* node, const char* cameraName) {
    IUseEventFlowData* eventFlowUser = node;
    eventFlowUser->getEventFlowDataHolder()->getRequestInfo()->requestDemoCamera(cameraName);
}

bool isEndRequestDemoCamera(const EventFlowNode* node) {
    const IUseEventFlowData* eventFlowUser = node;
    return eventFlowUser->getEventFlowDataHolder()->getRequestInfo()->getDemoCameraAnimName() ==
           nullptr;
}

bool isHideDemoPlayer(const EventFlowNode* node) {
    const IUseEventFlowData* eventFlowUser = node;
    return eventFlowUser->getEventFlowDataHolder()->getRequestInfo()->isHideDemoPlayer();
}

void requestHideDemoPlayer(EventFlowNode* node) {
    IUseEventFlowData* eventFlowUser = node;
    eventFlowUser->getEventFlowDataHolder()->getRequestInfo()->requestHideDemoPlayer();
}

void requestShowDemoPlayer(EventFlowNode* node) {
    IUseEventFlowData* eventFlowUser = node;
    eventFlowUser->getEventFlowDataHolder()->getRequestInfo()->requestShowDemoPlayer();
}

void calcPlayerWatchTrans(sead::Vector3f* outWatchTrans, const EventFlowNode* node) {
    const IUseEventFlowData* eventFlowUser = node;
    const EventFlowWatchParam* watchParam =
        eventFlowUser->getEventFlowDataHolder()->getWatchParam();
    const LiveActor* actor = node->getActor();

    if (watchParam != nullptr) {
        watchParam->calcWatchTrans(outWatchTrans, actor);
        return;
    }

    const sead::Vector3f& actorTrans = getTrans(actor);
    outWatchTrans->set(actorTrans);
}

void requestCommandCloseTalkMessageLayout(EventFlowNode* node) {
    node->getSceneEventFlowMsg()->requestCommand("CloseTalkMessageLayout");
}

bool isActive(const EventFlowExecutor* executor) {
    return executor->getCurrentEntryName() != nullptr && executor->getCurrentNode() != nullptr;
}

bool isScare(const IUseEventFlowData* user) {
    if (user->getEventFlowDataHolder()->getScareCtrl() == nullptr)
        return false;
    return user->getEventFlowDataHolder()->getScareCtrl()->isScare();
}

bool tryGetScareEnemyPos(sead::Vector3f* outScareEnemyPos, const IUseEventFlowData* user) {
    if (user->getEventFlowDataHolder()->getScareCtrl() == nullptr)
        return false;
    return user->getEventFlowDataHolder()->getScareCtrl()->tryGetScareEnemyPos(outScareEnemyPos);
}

void invalidateUiCollisionCheck(EventFlowExecutor* executor) {
    executor->getState()->isInvalidUiCollisionCheck = true;
}

void onExistLookAtJointCtrl(EventFlowExecutor* executor) {
    executor->getState()->hasLookAtJointCtrl = true;
}

void setTalkSubActorName(EventFlowExecutor* executor, const char* actorName) {
    IUseEventFlowData* eventFlowUser = executor;
    eventFlowUser->getEventFlowDataHolder()->setTalkSubActorName(actorName);
}

void resetTalkSubActorName(EventFlowExecutor* executor) {
    IUseEventFlowData* eventFlowUser = executor;
    eventFlowUser->getEventFlowDataHolder()->clearTalkSubActorName();
}

void startEventAction(LiveActor* actor, const IUseEventFlowData* user, const char* actionName) {
    StringTmp<128> eventActionName("");
    user->getEventFlowDataHolder()->convertActionName(&eventActionName, actionName);
    startAction(actor, eventActionName.cstr());
}

void makeEventActionName(sead::BufferedSafeStringBase<char>* outActionName,
                         const IUseEventFlowData* user, const char* actionName) {
    user->getEventFlowDataHolder()->convertActionName(outActionName, actionName);
}

void startEventActionAtRandomFrame(LiveActor* actor, const IUseEventFlowData* user,
                                   const char* actionName) {
    StringTmp<128> eventActionName("");
    user->getEventFlowDataHolder()->convertActionName(&eventActionName, actionName);
    startAction(actor, eventActionName.cstr());

    f32 randomFrame = getRandom(0.0f, getSklAnimFrameMax(actor, 0));
    setSklAnimFrame(actor, randomFrame, 0);

    if (isVisAnimPlayingForAction(actor)) {
        if (getVisAnimFrameMaxForAction(actor) > 0.0f) {
            f32 visAnimFrameMax = getVisAnimFrameMaxForAction(actor);
            setVisAnimFrameForAction(actor,
                                     modf(randomFrame + visAnimFrameMax, visAnimFrameMax) + 0.0f);
        }
    }
}

bool tryStartEventActionIfNotPlaying(LiveActor* actor, const IUseEventFlowData* user,
                                     const char* actionName) {
    StringTmp<128> eventActionName("");
    user->getEventFlowDataHolder()->convertActionName(&eventActionName, actionName);
    return tryStartActionIfNotPlaying(actor, eventActionName.cstr());
}

bool isPlayingEventAction(const LiveActor* actor, const IUseEventFlowData* user,
                          const char* actionName) {
    StringTmp<128> eventActionName("");
    user->getEventFlowDataHolder()->convertActionName(&eventActionName, actionName);
    return isActionPlaying(actor, eventActionName.cstr());
}

bool isExistEventAction(const LiveActor* actor, const IUseEventFlowData* user,
                        const char* actionName) {
    StringTmp<128> eventActionName("");
    user->getEventFlowDataHolder()->convertActionName(&eventActionName, actionName);
    return isExistAction(actor, eventActionName.cstr());
}

bool isOneTimeEventAction(const LiveActor* actor, const IUseEventFlowData* user,
                          const char* actionName) {
    StringTmp<128> eventActionName("");
    user->getEventFlowDataHolder()->convertActionName(&eventActionName, actionName);
    return isActionOneTime(actor, eventActionName.cstr());
}

f32 getEventActionFrameMax(const LiveActor* actor, const IUseEventFlowData* user,
                           const char* actionName) {
    StringTmp<128> eventActionName("");
    user->getEventFlowDataHolder()->convertActionName(&eventActionName, actionName);
    return getActionFrameMax(actor, eventActionName.cstr());
}

f32 getParamMoveSpeed(const IUseEventFlowData* user) {
    return user->getEventFlowDataHolder()->getDefaultActorParam()->moveSpeed;
}

f32 getParamTurnSpeedDegree(const IUseEventFlowData* user) {
    return user->getEventFlowDataHolder()->getDefaultActorParam()->turnSpeedDegree;
}

const sead::Vector3f& getRecordActorFront(const IUseEventFlowData* user) {
    return user->getEventFlowDataHolder()->getActorFront();
}

void recordActorFront(IUseEventFlowData* user, const sead::Vector3f& front) {
    user->getEventFlowDataHolder()->setActorFront(front);
}

bool checkInsideTerritoryPos(const IUseEventFlowData* user, const LiveActor* actor,
                             const sead::Vector3f& position, f32 radiusOffset) {
    sead::Vector3f territoryPosition = {0.0f, 0.0f, 0.0f};
    const EventFlowDataHolder* holder = user->getEventFlowDataHolder();
    multVecPose(&territoryPosition, actor, holder->getTerritory()->getLocalOffset());

    sead::Vector3f distance = territoryPosition - position;
    return distance.length() <
           user->getEventFlowDataHolder()->getTerritory()->getRadius() + radiusOffset;
}

bool checkInsideEmotionTerritoryPos(const IUseEventFlowData* user, const LiveActor* actor,
                                    const sead::Vector3f& position) {
    sead::Vector3f territoryPosition = {0.0f, 0.0f, 0.0f};
    const EventFlowDataHolder* holder = user->getEventFlowDataHolder();
    multVecPose(&territoryPosition, actor, holder->getTerritory()->getLocalOffset());

    sead::Vector3f distance = territoryPosition - position;
    return distance.length() < user->getEventFlowDataHolder()->getTerritory()->getEmotionRadius();
}

bool checkInsideEmotionLowPriorityTerritoryPos(const IUseEventFlowData* user,
                                               const LiveActor* actor,
                                               const sead::Vector3f& position) {
    sead::Vector3f territoryPosition = {0.0f, 0.0f, 0.0f};
    const EventFlowDataHolder* holder = user->getEventFlowDataHolder();
    multVecPose(&territoryPosition, actor, holder->getTerritory()->getLocalOffset());

    sead::Vector3f distance = territoryPosition - position;
    return distance.length() <
           user->getEventFlowDataHolder()->getTerritory()->getEmotionLowPriorityRadius();
}

bool checkInsideTalkTerritoryPos(const IUseEventFlowData* user, const LiveActor* actor,
                                 const sead::Vector3f& position) {
    sead::Vector3f territoryPosition = {0.0f, 0.0f, 0.0f};
    const EventFlowDataHolder* holder = user->getEventFlowDataHolder();
    multVecPose(&territoryPosition, actor, holder->getTerritory()->getLocalOffset());

    sead::Vector3f distance = territoryPosition - position;
    return distance.length() < user->getEventFlowDataHolder()->getTerritory()->getTalkableRadius();
}

void calcBalloonOffset(sead::Vector3f* outOffset, const IUseEventFlowData* user,
                       const LiveActor* actor) {
    sead::Quatf actorQuat = sead::Quatf::unit;
    calcQuat(&actorQuat, actor);

    const EventFlowActorTerritory* territory = user->getEventFlowDataHolder()->getTerritory();
    outOffset->setRotated(actorQuat, territory->getBalloonLocalOffset());
}

void setBalloonLocalOffset(EventFlowExecutor* executor, const sead::Vector3f& localOffset) {
    IUseEventFlowData* eventFlowUser = executor;
    eventFlowUser->getEventFlowDataHolder()->getTerritory()->setBalloonLocalOffset(localOffset);
}

f32 getBalloonCollisionCheckOffsetRadius(const IUseEventFlowData* user) {
    return user->getEventFlowDataHolder()->getTerritory()->getBalloonCollisionCheckOffsetRadius();
}

bool isDisableOpenBalloonByOrderGroup(const LiveActor* actor, const IUseEventFlowData* user) {
    if (user->getEventFlowDataHolder()->getBalloonOrderGroup() == nullptr)
        return false;
    return !user->getEventFlowDataHolder()->getBalloonOrderGroup()->isEnableAppearBalloon(actor);
}

bool isEnableForceOpenBalloonByOrderGroup(const LiveActor* actor, const IUseEventFlowData* user) {
    if (user->getEventFlowDataHolder()->getBalloonOrderGroup() == nullptr)
        return false;
    return user->getEventFlowDataHolder()->getBalloonOrderGroup()->isEnableAppearBalloon(actor);
}

bool isWaitAtPointMovement(const EventFlowExecutor* executor) {
    return executor->isWaitAtPointMovement();
}

bool isStopMovement(const IUseEventFlowData* user) {
    if (user->getEventFlowDataHolder()->isStopMovementByNode())
        return true;
    return user->getEventFlowDataHolder()->getExecutorState()->isStopMovement;
}

bool isRequestEventDemoAction(const EventFlowExecutor* executor) {
    const IUseEventFlowData* eventFlowUser = executor;
    return eventFlowUser->getEventFlowDataHolder()->getRequestInfo()->isRequestDemoAction();
}

bool isPlayingEventDemoAction(const EventFlowExecutor* executor) {
    const IUseEventFlowData* eventFlowUser = executor;
    return eventFlowUser->getEventFlowDataHolder()->getRequestInfo()->isPlayingDemoAction();
}

const char* getEventDemoActionName(const EventFlowExecutor* executor) {
    const IUseEventFlowData* eventFlowUser = executor;
    return eventFlowUser->getEventFlowDataHolder()->getRequestInfo()->getDemoActionName();
}

bool isExistEventDemoTalkAction(const EventFlowExecutor* executor) {
    const IUseEventFlowData* eventFlowUser = executor;
    return eventFlowUser->getEventFlowDataHolder()->getRequestInfo()->isExistDemoTalkAction();
}

const char* getEventDemoTalkActionName(const EventFlowExecutor* executor) {
    const IUseEventFlowData* eventFlowUser = executor;
    return eventFlowUser->getEventFlowDataHolder()->getRequestInfo()->getDemoTalkActionName();
}

bool isResetEventDemoPlayerDynamics(const EventFlowExecutor* executor) {
    const IUseEventFlowData* eventFlowUser = executor;
    return eventFlowUser->getEventFlowDataHolder()->getRequestInfo()->isResetDemoPlayerDynamics();
}

void startEventDemoAction(EventFlowExecutor* executor) {
    IUseEventFlowData* eventFlowUser = executor;
    eventFlowUser->getEventFlowDataHolder()->getRequestInfo()->startDemoAction();
}

void endEventDemoAction(EventFlowExecutor* executor) {
    IUseEventFlowData* eventFlowUser = executor;
    eventFlowUser->getEventFlowDataHolder()->getRequestInfo()->endDemoAction();
}

bool isRequestEventDemoCamera(const EventFlowExecutor* executor) {
    const IUseEventFlowData* eventFlowUser = executor;
    return eventFlowUser->getEventFlowDataHolder()->getRequestInfo()->isRequestDemoCamera();
}

const char* getEventDemoCameraAnimName(const EventFlowExecutor* executor) {
    const IUseEventFlowData* eventFlowUser = executor;
    return eventFlowUser->getEventFlowDataHolder()->getRequestInfo()->getDemoCameraAnimName();
}

void clearEventDemoCameraRequest(EventFlowExecutor* executor) {
    IUseEventFlowData* eventFlowUser = executor;
    eventFlowUser->getEventFlowDataHolder()->getRequestInfo()->clearDemoCameraRequest();
}

bool isHideDemoPlayer(const EventFlowExecutor* executor) {
    const IUseEventFlowData* eventFlowUser = executor;
    return eventFlowUser->getEventFlowDataHolder()->getRequestInfo()->isHideDemoPlayer();
}

bool isExistEventEntry(const EventFlowExecutor* executor, const char* entryName) {
    return executor->isExistEntry(entryName);
}

bool isCurrentEventEntry(const EventFlowExecutor* executor, const char* entryName) {
    const char* currentEntryName = executor->getCurrentEntryName();
    if (currentEntryName != nullptr && executor->getCurrentNode() != nullptr)
        return isEqualString(currentEntryName, entryName);
    return false;
}

bool isEventName(const EventFlowEventData* eventData, const char* format, ...) {
    std::va_list args;
    va_start(args, format);

    StringTmp<256> eventName("");
    eventName.formatV(format, args);
    va_end(args);

    return isEqualString(eventData->getName(), eventName.cstr());
}

const char* getEventName(const EventFlowEventData* eventData) {
    return eventData->getName();
}

const char* getEventDataParamString(const EventFlowEventData* eventData, const char* key) {
    return getByamlKeyString(eventData->getByamlIter(), key);
}

bool isEventDataParamBool(const EventFlowEventData* eventData, const char* key) {
    return getByamlKeyBool(eventData->getByamlIter(), key);
}
}  // namespace al

namespace alEventFlowFunction {
bool isReceiveCommandCloseTalkMessageLayout(const al::SceneEventFlowMsg* sceneEventFlowMsg) {
    return sceneEventFlowMsg->isReceiveCommand("CloseTalkMessageLayout");
}

void clearSceneEventFlowMsg(al::SceneEventFlowMsg* sceneEventFlowMsg) {
    sceneEventFlowMsg->clearCommand();
}
}  // namespace alEventFlowFunction
