#include "Util/NpcEventFlowUtil.h"

#include "Library/Area/TrafficAreaDirector.h"
#include "Library/Audio/IUseAudioKeeper.h"
#include "Library/Base/StringUtil.h"
#include "Library/Camera/CameraPoser.h"
#include "Library/Camera/CameraPoserFix.h"
#include "Library/Camera/CameraPoserFunction.h"
#include "Library/Camera/CameraTicket.h"
#include "Library/Camera/CameraUtil.h"
#include "Library/Event/BalloonOrderGroup.h"
#include "Library/Event/BalloonOrderGroupHolder.h"
#include "Library/Event/EventFlowChart.h"
#include "Library/Event/EventFlowChartInitInfo.h"
#include "Library/Event/EventFlowChoiceInfo.h"
#include "Library/Event/EventFlowDataHolder.h"
#include "Library/Event/EventFlowExecutor.h"
#include "Library/Event/EventFlowExecutorHolder.h"
#include "Library/Event/EventFlowFunction.h"
#include "Library/Event/EventFlowMovement.h"
#include "Library/Event/EventFlowNode.h"
#include "Library/Event/EventFlowUtil.h"
#include "Library/Event/EventFlowWatchParam.h"
#include "Library/Item/ItemUtil.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/LiveActor/LiveActorFunction.h"
#include "Library/Message/IUseMessageSystem.h"
#include "Library/Message/MessageHolder.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"
#include "Library/Play/Camera/CameraPoserLookBoard.h"
#include "Library/Play/Camera/CameraPoserTalk.h"
#include "Library/Scene/SceneObjUtil.h"

#include "Event/CostumePatternChecker.h"
#include "Event/EventDemoCtrl.h"
#include "Event/EventFlowScareCtrl.h"
#include "Event/EventFlowSceneExecuteCtrl.h"
#include "Npc/NpcEventBalloonInfo.h"
#include "Npc/NpcEventCtrlInfo.h"
#include "Npc/TalkNpcActionAnimInfo.h"
#include "Npc/TalkNpcParam.h"
#include "Npc/TalkNpcSceneEventSwitcher.h"
#include "Scene/ProjectEventFlowNodeFactory.h"
#include "Scene/SceneObjFactory.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolderAccessor.h"
#include "Util/DemoUtil.h"
#include "Util/PlayerDemoUtil.h"
#include "Util/PlayerUtil.h"
#include "Util/StageInputFunction.h"

namespace {

static const sead::Vector3f CameraOffsetZero = {0.0f, 0.0f, 0.0f};

class MessageSystemAccessor : public al::IUseMessageSystem {
public:
    MessageSystemAccessor() {}

    const al::MessageSystem* getMessageSystem() const override;

    void setMessageSystem(const al::MessageSystem* messageSystem) {
        mMessageSystem = messageSystem;
    }

private:
    const al::MessageSystem* mMessageSystem;
};

const al::IUseSceneObjHolder* getSceneObjHolder(const al::LiveActor* actor) {
    return actor;
}

al::IUseCamera* getCameraUser(al::LiveActor* actor) {
    return actor;
}

const al::IUseAudioKeeper* getAudioUser(const al::LiveActor* actor) {
    return actor;
}

NpcEventCtrlInfo* getNpcEventCtrlInfo(const al::LiveActor* actor) {
    return static_cast<NpcEventCtrlInfo*>(
        al::getSceneObj(getSceneObjHolder(actor), SceneObjID_NpcEventCtrlInfo));
}

NpcEventCtrlInfo* getNpcEventCtrlInfo(const al::EventFlowNode* node) {
    return getNpcEventCtrlInfo(node->getActor());
}

EventDemoCtrl* getEventDemoCtrl(const al::LiveActor* actor) {
    return static_cast<EventDemoCtrl*>(
        al::getSceneObj(getSceneObjHolder(actor), SceneObjID_EventDemoCtrl));
}

EventDemoCtrl* getEventDemoCtrl(const al::EventFlowNode* node) {
    return getEventDemoCtrl(node->getActor());
}

TalkNpcSceneEventSwitcher* getTalkNpcSceneEventSwitcher(const al::LiveActor* actor) {
    return static_cast<TalkNpcSceneEventSwitcher*>(
        al::getSceneObj(getSceneObjHolder(actor), SceneObjID_TalkNpcSceneEventSwitcher));
}

TalkNpcSceneEventSwitcher* getTalkNpcSceneEventSwitcher(const al::EventFlowNode* node) {
    return getTalkNpcSceneEventSwitcher(node->getActor());
}

EventFlowSceneExecuteCtrl* getSceneExecuteCtrl(const al::LiveActor* actor) {
    return getNpcEventCtrlInfo(actor)->getSceneExecuteCtrl();
}

EventFlowSceneExecuteCtrl* getSceneExecuteCtrl(const al::EventFlowNode* node) {
    return getSceneExecuteCtrl(node->getActor());
}

al::TrafficAreaDirector* tryGetTrafficAreaDirector(const al::LiveActor* actor) {
    return static_cast<al::TrafficAreaDirector*>(
        al::tryGetSceneObj(getSceneObjHolder(actor), SceneObjID_alTrafficAreaDirector));
}

void setInvalidUiCollisionCheckIfRequested(NpcEventBalloonInfo* balloonInfo,
                                           const al::IUseEventFlowData* user) {
    if (al::isInvalidUiCollisionCheck(user))
        balloonInfo->setInvalidUiCollisionCheck();
}

void requestShowBalloonMessage(al::EventFlowNode* node, const NpcEventBalloonInfo& info) {
    getNpcEventCtrlInfo(node)->requestShowBalloonMessage(info);
}

al::LiveActor* getTalkMessageActor(al::EventFlowNode* node) {
    const char* subActorName = node->getEventFlowDataHolder()->getTalkSubActorName();
    al::LiveActor* actor = node->getActor();
    if (subActorName != nullptr)
        return al::getSubActor(actor, node->getEventFlowDataHolder()->getTalkSubActorName());
    return actor;
}

void startOpenTalkMessage(al::EventFlowNode* node, const char16* message, bool isEnableButtonA,
                          bool isSystemMessage, s32 messageStyle) {
    NpcEventTalkInfo info(getTalkMessageActor(node), message,
                          node->getEventFlowDataHolder()->getMessageTagDataHolder());
    if (isEnableButtonA)
        info.setEnableButtonA();
    if (isSystemMessage)
        info.setSystemMessage(messageStyle);
    getNpcEventCtrlInfo(node)->requestShowTalkMessage(node, info);
}

void initEventMovementCommon(al::EventFlowExecutor* executor, al::EventFlowMovement* movement,
                             const al::ActorInitInfo& initInfo) {
    movement->setEventFlowDataHolder(executor->getEventFlowDataHolder());
    movement->init(initInfo);
    executor->initMovement(movement);
}

al::EventFlowExecutor* initEventFlowImpl(al::LiveActor* actor, const al::ActorInitInfo& initInfo,
                                         const char* eventFileName, const char* eventObjName,
                                         const char* eventFlowName, bool isInitScareCtrl,
                                         bool isEnableRunAway) {
    const char* objectName = eventObjName;
    if (objectName == nullptr)
        al::getObjectName(&objectName, initInfo);
    const char* flowName = eventFlowName == nullptr ? objectName : eventFlowName;

    al::EventFlowDataHolder* dataHolder = new al::EventFlowDataHolder();
    ProjectEventFlowNodeFactory nodeFactory;
    const char* chartObjectName = objectName;
    const al::IUseSceneObjHolder* sceneObjHolder = getSceneObjHolder(actor);
    al::SceneEventFlowMsg* sceneMsg = getNpcEventCtrlInfo(actor)->getSceneEventFlowMsg();

    al::EventFlowChartInitInfo chartInitInfo(
        actor, dataHolder, initInfo, eventFileName, chartObjectName, flowName, nodeFactory,
        sceneMsg, GameDataFunction::getCurrentStageName(GameDataHolderAccessor(sceneObjHolder)));
    al::EventFlowChart* chart = new al::EventFlowChart();
    chart->init(chartInitInfo);

    al::EventFlowExecutor* executor = new al::EventFlowExecutor();
    executor->init(actor, chart, dataHolder);

    if (isInitScareCtrl) {
        EventFlowScareCtrl* scareCtrl = new EventFlowScareCtrl();
        scareCtrl->init(actor, initInfo, dataHolder, objectName);
        executor->initScareCtrl(scareCtrl);
        if (!isEnableRunAway)
            scareCtrl->disableRunAway();
    }

    al::PlacementInfo placementInfo;
    if (al::tryGetLinksInfo(&placementInfo, initInfo, "BalloonOrderGroup")) {
        al::BalloonOrderGroup* group =
            getNpcEventCtrlInfo(actor)->getBalloonOrderGroupHolder()->tryFindGroup(placementInfo);
        if (group == nullptr) {
            group = new al::BalloonOrderGroup(placementInfo);
            getNpcEventCtrlInfo(actor)->getBalloonOrderGroupHolder()->registerGroup(group);
        }
        group->registerRequester(actor, executor, initInfo);
        dataHolder->setBalloonOrderGroup(group);
    }

    getNpcEventCtrlInfo(actor)->getEventFlowExecutorHolder()->registerExecutor(executor);
    return executor;
}

bool isFixCameraForNearestAt(al::CameraTicket* cameraTicket) {
    return al::isEqualString(cameraTicket->getPoser()->getName(), "固定") ||
           al::isEqualString(cameraTicket->getPoser()->getName(), "完全固定") ||
           al::isEqualString(cameraTicket->getPoser()->getName(), "出入口専用固定");
}

void setupFixCameraForNearestAt(al::CameraTicket* cameraTicket) {
    if (isFixCameraForNearestAt(cameraTicket)) {
        al::CameraPoserFix* poser = static_cast<al::CameraPoserFix*>(cameraTicket->getPoser());
        poser->setIsCalcNearestAtFromPreAt(true);
        alCameraPoserFunction::invalidateKeepDistanceNextCameraIfNoCollide(poser);
    }
}

s32 getBindPriority(const al::HitSensor* sensor) {
    if (sensor == nullptr)
        return -1;
    if (al::isSensorBindableGoal(sensor))
        return 7;
    if (al::isSensorBindableAllPlayer(sensor))
        return 6;
    if (al::isSensorBindableBubbleOutScreen(sensor))
        return 5;
    if (al::isSensorBindableKoura(sensor))
        return 4;
    if (al::isSensorBindableRouteDokan(sensor))
        return 3;
    if (al::isSensorBindableBubblePadInput(sensor))
        return 2;
    if (al::isSensorBindable(sensor))
        return 1;
    return -1;
}

}  // namespace

namespace rs {

al::EventFlowExecutor* initEventFlow(al::LiveActor* actor, const al::ActorInitInfo& initInfo,
                                     const char* eventObjName, const char* eventFlowName) {
    return initEventFlowImpl(actor, initInfo, nullptr, eventObjName, eventFlowName, true, true);
}

al::EventFlowExecutor* initEventFlowForRunAwayNpc(al::LiveActor* actor,
                                                  const al::ActorInitInfo& initInfo,
                                                  const char* eventObjName,
                                                  const char* eventFlowName) {
    return initEventFlowImpl(actor, initInfo, nullptr, eventObjName, eventFlowName, true, false);
}

al::EventFlowExecutor* initEventFlowForSystem(al::LiveActor* actor,
                                              const al::ActorInitInfo& initInfo,
                                              const char* eventFileName, const char* eventObjName,
                                              const char* eventFlowName) {
    return initEventFlowImpl(actor, initInfo, eventFileName, eventObjName, eventFlowName, false,
                             true);
}

al::EventFlowExecutor* initEventFlowSuffix(al::LiveActor* actor, const al::ActorInitInfo& initInfo,
                                           const char* eventFileName, const char* eventObjName,
                                           const char* eventFlowName) {
    return initEventFlowImpl(actor, initInfo, eventFileName, eventObjName, eventFlowName, true,
                             true);
}

al::EventFlowExecutor* initEventFlowFromPlacementInfo(al::LiveActor* actor,
                                                      const al::ActorInitInfo& initInfo,
                                                      const char* eventObjName) {
    const char* eventName = al::getStringArg(initInfo, "EventName");
    return initEventFlowImpl(actor, initInfo, nullptr, eventObjName, eventName, true, true);
}

al::EventFlowExecutor* initCommonEventFlow(al::LiveActor* actor, const al::ActorInitInfo& initInfo,
                                           const char* eventFlowName) {
    return initEventFlowImpl(actor, initInfo, nullptr, "Common", eventFlowName, true, true);
}

al::EventFlowExecutor* createCommonEventFlowFromPlacementInfo(al::LiveActor* actor,
                                                              const al::ActorInitInfo& initInfo) {
    const char* eventName = "None";
    al::tryGetStringArg(&eventName, initInfo, "EventName");
    return initEventFlowImpl(actor, initInfo, nullptr, "Common", eventName, true, true);
}

void initEventCharacterName(al::EventFlowExecutor* executor, const al::ActorInitInfo& initInfo,
                            const char* name) {
    MessageSystemAccessor accessor;
    const al::LayoutInitInfo& layoutInfo = al::getLayoutInitInfo(initInfo);
    accessor.setMessageSystem(layoutInfo.getMessageSystem());
    if (al::isExistLabelInSystemMessage(&accessor, "CharacterName", name)) {
        const char16* message = al::getSystemMessageString(&accessor, "CharacterName", name);
        executor->getEventFlowDataHolder()->initCharacterName(message);
    }
}

void makeEventCharacterName(sead::WBufferedSafeString* out, const al::ActorInitInfo& initInfo,
                            const char* name) {
    MessageSystemAccessor accessor;
    const al::LayoutInitInfo& layoutInfo = al::getLayoutInitInfo(initInfo);
    accessor.setMessageSystem(layoutInfo.getMessageSystem());
    const char16* message = al::getSystemMessageString(&accessor, "CharacterName", name);
    al::copyMessageWithTag(out->getBuffer(), out->getBufferSize(), message);
}

void initEventParam(al::EventFlowExecutor* executor, const TalkNpcParam* param,
                    const char* suffix) {
    al::EventFlowDataHolder* dataHolder = executor->getEventFlowDataHolder();
    const void* actorParam = suffix != nullptr ? param->findEventFlowActorSuffixParam(suffix) :
                                                 param->getEventFlowActorParam();
    dataHolder->setEventFlowActorParam(actorParam);
    executor->getEventFlowDataHolder()->setEventFlowActor(param->getEventFlowActor());
}

bool tryInitItemKeeperByEvent(al::LiveActor* actor, const al::ActorInitInfo& initInfo,
                              const al::EventFlowExecutor* executor) {
    if (executor->getEventFlowDataHolder()->getItemTypeCount() < 1)
        return false;
    actor->initItemKeeper(executor->getEventFlowDataHolder()->getItemTypeCount());
    for (s64 i = 0; i < executor->getEventFlowDataHolder()->getItemTypeCount(); i++) {
        const char* itemType = executor->getEventFlowDataHolder()->getItemType(i);
        al::addItem(actor, initInfo, itemType, itemType, nullptr, -1, false);
    }
    return true;
}

void startEventFlow(al::EventFlowExecutor* executor, const char* entryName) {
    executor->start(entryName);
}

bool updateEventFlow(al::EventFlowExecutor* executor) {
    executor->execute();
    return executor->isEnd();
}

void stopEventFlow(al::EventFlowExecutor* executor) {
    executor->stopMovement();
}

void restartEventFlow(al::EventFlowExecutor* executor) {
    executor->restartMovement();
}

void initEventMovementRail(al::EventFlowExecutor* executor, const al::ActorInitInfo& initInfo) {
    initEventMovementCommon(executor, new al::EventFlowMovementRail(executor->getActor()),
                            initInfo);
}

void initEventMovement(al::EventFlowExecutor* executor, al::EventFlowMovement* movement,
                       const al::ActorInitInfo& initInfo) {
    initEventMovementCommon(executor, movement, initInfo);
}

void initEventMovementTurnSeparate(al::EventFlowExecutor* executor,
                                   const al::ActorInitInfo& initInfo) {
    initEventMovementCommon(executor, new al::EventFlowMovementTurnSeparate(executor->getActor()),
                            initInfo);
}

void initEventMovementWait(al::EventFlowExecutor* executor, const al::ActorInitInfo& initInfo) {
    initEventMovementCommon(executor, new al::EventFlowMovementWait(executor->getActor()),
                            initInfo);
}

void initEventMovementWander(al::EventFlowExecutor* executor, const al::ActorInitInfo& initInfo) {
    initEventMovementCommon(executor, new al::EventFlowMovementWander(executor->getActor()),
                            initInfo);
}

bool isDefinedEventCamera(const al::EventFlowExecutor* executor, const char* name) {
    return executor->getEventFlowDataHolder()->isExistCameraInfo(name);
}

void initEventActionNameConverter(al::EventFlowExecutor* executor,
                                  const al::IEventFlowActionNameConverter* converter) {
    executor->getEventFlowDataHolder()->setEventActionNameConverter(converter);
}

void initEventQueryJudge(al::EventFlowExecutor* executor,
                         const al::IEventFlowQueryJudge* queryJudge) {
    executor->getEventFlowDataHolder()->setEventQueryJudge(queryJudge);
}

void initEventMessageTagDataHolder(al::EventFlowExecutor* executor,
                                   const al::MessageTagDataHolder* messageTagDataHolder) {
    executor->getEventFlowDataHolder()->setMessageTagDataHolder(messageTagDataHolder);
}

void initEventCameraObject(al::EventFlowExecutor* flowExecutor, const al::ActorInitInfo& initInfo,
                           const char* name) {
    al::CameraTicket* cameraTicket =
        al::initObjectCamera(flowExecutor->getActor(), initInfo, name, "会話用2点間");
    alCameraFunction::initPriorityDemoTalk(cameraTicket);
    alCameraFunction::validateKeepPreSelfPoseNextCamera(cameraTicket);
    alCameraFunction::validateCameraInterpoleEaseOut(cameraTicket);

    if (al::isEqualString(cameraTicket->getPoser()->getName(), "固定") ||
        al::isEqualString(cameraTicket->getPoser()->getName(), "完全固定") ||
        al::isEqualString(cameraTicket->getPoser()->getName(), "出入口専用固定")) {
        al::CameraPoserFix* poser = static_cast<al::CameraPoserFix*>(cameraTicket->getPoser());
        poser->setIsCalcNearestAtFromPreAt(true);
        alCameraPoserFunction::invalidateKeepDistanceNextCameraIfNoCollide(poser);
    }

    flowExecutor->getEventFlowDataHolder()->initCamera(name, cameraTicket);
}

void initEventCameraObjectAfterKeepPose(al::EventFlowExecutor* executor,
                                        const al::ActorInitInfo& initInfo, const char* name) {
    al::CameraTicket* cameraTicket =
        al::initObjectCamera(getCameraUser(executor->getActor()), initInfo, name, "会話用2点間");
    alCameraFunction::initPriorityDemoTalk(cameraTicket);
    alCameraFunction::validateCameraInterpoleEaseOut(cameraTicket);
    setupFixCameraForNearestAt(cameraTicket);
    executor->getEventFlowDataHolder()->initCamera(name, cameraTicket);
}

void initEventCameraTalk(al::EventFlowExecutor* executor, const al::ActorInitInfo& initInfo,
                         const char* name, f32 minDistance) {
    al::LiveActor* actor = executor->getActor();
    al::CameraPoserTalk* poser = new al::CameraPoserTalk("会話用2点間");
    if (minDistance > 0.0f)
        poser->setMinDistance(minDistance);
    al::CameraTicket* ticket =
        alCameraFunction::initCamera(poser, getCameraUser(actor), initInfo, name, 10);
    alCameraFunction::validateCameraInterpoleEaseOut(ticket);
    executor->getEventFlowDataHolder()->initCamera(name, ticket);
}

void initEventCameraTalk2(al::EventFlowExecutor* executor, const al::ActorInitInfo& initInfo,
                          const char* name) {
    al::LiveActor* actor = executor->getActor();
    al::CameraPoserTalk* poser = new al::CameraPoserTalk("会話用2点間");
    al::CameraTicket* ticket =
        alCameraFunction::initCamera(poser, getCameraUser(actor), initInfo, name, 10);
    alCameraFunction::initPriorityDemo(ticket);
    alCameraFunction::validateCameraInterpoleEaseOut(ticket);
    executor->getEventFlowDataHolder()->initCamera(name, ticket);
}

void initEventCameraLookBoard(al::EventFlowExecutor* executor, const al::ActorInitInfo& initInfo,
                              const sead::Vector3f& offset, const char* name) {
    al::CameraPoserLookBoard* poser = new al::CameraPoserLookBoard("看板");
    poser->setLookAtOffset(offset);
    al::CameraTicket* ticket = alCameraFunction::initCamera(
        poser, getCameraUser(executor->getActor()), initInfo, name, 10);
    alCameraFunction::validateKeepPreSelfPoseNextCamera(ticket);
    alCameraFunction::validateCameraInterpoleEaseOut(ticket);
    executor->getEventFlowDataHolder()->initCamera(name, ticket);
}

void initEventCameraAnim(al::EventFlowExecutor* executor, const al::LiveActor* actor,
                         const al::ActorInitInfo& initInfo, const char* name) {
    al::CameraTicket* ticket = al::initDemoAnimCamera(actor, initInfo, name);
    alCameraFunction::validateCameraInterpoleEaseOut(ticket);
    executor->getEventFlowDataHolder()->initCamera(name, ticket);
}

void initEventCameraFixActor(al::EventFlowExecutor* executor, const al::ActorInitInfo& initInfo,
                             const char* name, const al::LiveActor* target,
                             const sead::Vector3f* offset, f32 distance, f32 angleH, f32 angleV) {
    const al::LiveActor* executorActor = executor->getActor();
    const sead::Vector3f* defaultOffset = &CameraOffsetZero;
    al::CameraTicket* ticket =
        al::initFixActorCamera(target == nullptr ? executorActor : target, initInfo, name,
                               *(offset == nullptr ? defaultOffset : offset),
                               distance < 0.0f ? 500.0f : distance, angleH, angleV, false);
    alCameraFunction::initPriorityDemoTalk(ticket);
    alCameraFunction::validateCameraInterpoleEaseOut(ticket);
    executor->getEventFlowDataHolder()->initCamera(name, ticket);
}

void initEventCameraFixActor2(al::EventFlowExecutor* executor, const al::ActorInitInfo& initInfo,
                              const char* name, const al::LiveActor* target,
                              const sead::Vector3f* offset, f32 distance, f32 angleH, f32 angleV,
                              bool isUseDistance) {
    const al::LiveActor* executorActor = executor->getActor();
    const sead::Vector3f* defaultOffset = &CameraOffsetZero;
    al::CameraTicket* ticket =
        al::initFixActorCamera(target == nullptr ? executorActor : target, initInfo, name,
                               *(offset == nullptr ? defaultOffset : offset),
                               distance < 0.0f ? 500.0f : distance, angleH, angleV, isUseDistance);
    alCameraFunction::initPriorityDemo(ticket);
    alCameraFunction::validateCameraInterpoleEaseOut(ticket);
    executor->getEventFlowDataHolder()->initCamera(name, ticket);
}

void initEventCameraFixActorAutoAroundFront(al::EventFlowExecutor* executor,
                                            const al::ActorInitInfo& initInfo, const char* name,
                                            const al::LiveActor* target,
                                            const sead::Vector3f* offset, f32 distance, f32 angleH,
                                            f32 angleV, bool isUseDistance) {
    const al::LiveActor* executorActor = executor->getActor();
    const sead::Vector3f* defaultOffset = &CameraOffsetZero;
    al::CameraTicket* ticket =
        al::initFixTalkCamera(target == nullptr ? executorActor : target, initInfo, name,
                              *(offset == nullptr ? defaultOffset : offset),
                              distance < 0.0f ? 500.0f : distance, angleH, angleV, isUseDistance);
    alCameraFunction::initPriorityDemoTalk(ticket);
    alCameraFunction::validateCameraInterpoleEaseOut(ticket);
    executor->getEventFlowDataHolder()->initCamera(name, ticket);
}

void initEventCameraFixActorAutoAroundFront2(al::EventFlowExecutor* executor,
                                             const al::ActorInitInfo& initInfo, const char* name,
                                             const al::LiveActor* target,
                                             const sead::Vector3f* offset, f32 distance, f32 angleH,
                                             f32 angleV) {
    const al::LiveActor* executorActor = executor->getActor();
    const sead::Vector3f* defaultOffset = &CameraOffsetZero;
    al::CameraTicket* ticket =
        al::initFixTalkCamera(target == nullptr ? executorActor : target, initInfo, name,
                              *(offset == nullptr ? defaultOffset : offset),
                              distance < 0.0f ? 500.0f : distance, angleH, angleV, false);
    alCameraFunction::initPriorityDemo(ticket);
    alCameraFunction::validateCameraInterpoleEaseOut(ticket);
    executor->getEventFlowDataHolder()->initCamera(name, ticket);
}

u32 checkMatchPatternCurrentCostume(bool* isCapMatch, bool* isClothMatch,
                                    const al::LiveActor* actor, const char* pattern) {
    CostumePatternChecker* checker = getNpcEventCtrlInfo(actor)->getCostumePatternChecker();
    GameDataHolderAccessor capAccessor(getSceneObjHolder(actor));
    const char* capName = GameDataFunction::getCurrentCapTypeName(capAccessor);
    GameDataHolderAccessor costumeAccessor(getSceneObjHolder(actor));
    const char* costumeName = GameDataFunction::getCurrentCostumeTypeName(costumeAccessor);
    return checker->checkMatchCostume(isCapMatch, isClothMatch, capName, costumeName, pattern);
}

bool checkMatchPatternCurrentCostumePair(const al::LiveActor* actor, const char* pattern) {
    CostumePatternChecker* checker = getNpcEventCtrlInfo(actor)->getCostumePatternChecker();
    GameDataHolderAccessor capAccessor(getSceneObjHolder(actor));
    const char* capName = GameDataFunction::getCurrentCapTypeName(capAccessor);
    GameDataHolderAccessor costumeAccessor(getSceneObjHolder(actor));
    const char* costumeName = GameDataFunction::getCurrentCostumeTypeName(costumeAccessor);
    return checker->checkMatchCostumePair(capName, costumeName, pattern);
}

bool isSuccessNpcEventBalloonMessage(const al::LiveActor* actor) {
    return getNpcEventCtrlInfo(actor)->isSuccessBalloonMessage(actor);
}

bool isPlayingNpcEventBalloonMessageVoice(const al::LiveActor* actor) {
    return getNpcEventCtrlInfo(actor)->isPlayingBalloonMessageVoice();
}

void requestNpcEventBalloonMessage(al::EventFlowNode* node, const char16* message,
                                   const NpcEventBalloonRequestInfo& requestInfo) {
    NpcEventBalloonInfo info;
    info.setupForMessageBalloon(node->getActor(), message,
                                node->getEventFlowDataHolder()->getMessageTagDataHolder());
    info.setCommonParam(requestInfo);
    setInvalidUiCollisionCheckIfRequested(&info, node);
    requestShowBalloonMessage(node, info);
}

void requestNpcEventTalkBalloonMessage(al::EventFlowNode* node, const char* iconName,
                                       const NpcEventBalloonRequestInfo& requestInfo) {
    if (al::isEqualString(iconName, "Action"))
        return;
    NpcEventBalloonInfo info;
    info.setupForEmotionIconBalloon(node->getActor(), iconName);
    info.setCommonParam(requestInfo);
    setInvalidUiCollisionCheckIfRequested(&info, node);
    requestShowBalloonMessage(node, info);
}

void requestNpcEventTalkBalloonMessageEntranceCamera(
    al::EventFlowNode* node, const char* iconName, const NpcEventBalloonRequestInfo& requestInfo) {
    NpcEventBalloonInfo info;
    info.setupForEmotionIconBalloon(node->getActor(), iconName);
    info.setCommonParam(requestInfo);
    setInvalidUiCollisionCheckIfRequested(&info, node);
    info.setEntranceCamera();
    requestShowBalloonMessage(node, info);
}

void requestNpcEventTalkBalloonMessageWithEnableButtonA(
    al::EventFlowNode* node, const char* iconName, const NpcEventBalloonRequestInfo& requestInfo) {
    NpcEventBalloonInfo info;
    info.setupForTalkIconBalloon(node->getActor(), iconName, true);
    info.setCommonParam(requestInfo);
    setInvalidUiCollisionCheckIfRequested(&info, node);
    requestShowBalloonMessage(node, info);
}

void requestNpcEventTalkBalloonMessageWithDisableButtonA(
    al::EventFlowNode* node, const char* iconName, const NpcEventBalloonRequestInfo& requestInfo) {
    NpcEventBalloonInfo info;
    info.setupForTalkIconBalloon(node->getActor(), iconName, false);
    info.setCommonParam(requestInfo);
    setInvalidUiCollisionCheckIfRequested(&info, node);
    requestShowBalloonMessage(node, info);
}

void requestNpcEventActionBalloonMessage(al::LiveActor* actor, bool isEnableButtonA,
                                         const NpcEventBalloonRequestInfo& requestInfo) {
    NpcEventBalloonInfo info;
    info.setupForTalkIconBalloon(actor, "Action", isEnableButtonA);
    info.setCommonParam(requestInfo);
    getNpcEventCtrlInfo(actor)->requestShowBalloonMessage(info);
}

void setEventBalloonFilterOnlyMiniGame(const al::LiveActor* actor) {
    getNpcEventCtrlInfo(actor)->setBalloonFilterOnlyMiniGame(actor);
}

void resetEventBalloonFilter(const al::LiveActor* actor) {
    getNpcEventCtrlInfo(actor)->resetBalloonFilter(actor);
}

void startChoiceEvent(al::EventFlowNode* node, al::EventFlowChoiceInfo* choiceInfo) {
    getNpcEventCtrlInfo(node)->startChoice(node, choiceInfo);
}

bool isSuccessLockEventTalkDemo(const al::EventFlowNode* node) {
    return getEventDemoCtrl(node)->isSuccessLockTalkDemo(node->getActor());
}

bool tryLockStartEventTalkDemo(al::EventFlowNode* node) {
    return getEventDemoCtrl(node)->tryLockStartTalkDemo(node->getActor());
}

bool tryLockStartEventTalkDemoWithoutBalloon(al::EventFlowNode* node) {
    return getEventDemoCtrl(node)->tryLockStartTalkDemoWithoutBalloon(node->getActor());
}

bool tryStartEventTalkDemo(al::EventFlowNode* node) {
    return getEventDemoCtrl(node)->tryStartTalkDemo(node->getActor());
}

bool tryStartEventTalkOnlyRequesterDemo(al::EventFlowNode* node) {
    return getEventDemoCtrl(node)->tryStartTalkOnlyRequesterDemo(node->getActor());
}

bool tryStartEventTalkKeepHackDemo(al::EventFlowNode* node) {
    return getEventDemoCtrl(node)->tryStartTalkKeepHackDemo(node->getActor());
}

bool tryStartEventTalkUseCoinDemo(al::EventFlowNode* node) {
    return getEventDemoCtrl(node)->tryStartTalkUseCoinDemo(node->getActor());
}

bool tryStartEventNormalDemo(al::EventFlowNode* node) {
    return getEventDemoCtrl(node)->tryStartNormalDemo(node->getActor());
}

bool tryStartEventKeepBindDemo(al::LiveActor* actor) {
    return getEventDemoCtrl(actor)->tryStartKeepBindDemo(actor);
}

bool tryStartEventCutSceneDemo(al::LiveActor* actor) {
    return getEventDemoCtrl(actor)->tryStartCutSceneDemo(actor);
}

bool tryStartEventCutSceneKeepHackDemo(al::LiveActor* actor) {
    return getEventDemoCtrl(actor)->tryStartCutSceneKeepHackDemo(actor);
}

bool tryStartEventCutSceneTalkOnlyRequesterDemo(al::LiveActor* actor) {
    return getEventDemoCtrl(actor)->tryStartCutSceneTalkOnlyRequesterDemo(actor);
}

void endEventCutSceneDemo(al::LiveActor* actor) {
    getEventDemoCtrl(actor)->endCutSceneDemo(actor);
}

void endEventCutSceneTalkOnlyRequeterDemo(al::LiveActor* actor) {
    getEventDemoCtrl(actor)->endCutSceneTalkOnlyRequesterDemo(actor);
}

void endEventCutSceneDemoBySkip(al::LiveActor* actor) {
    getEventDemoCtrl(actor)->endCutSceneDemoBySkip(actor);
}

void endEventCutSceneDemoOrTryEndEventCutSceneDemoBySkip(al::LiveActor* actor) {
    if (isWaitDemoSkipEnd(actor))
        endEventCutSceneDemoBySkip(actor);
    else
        endEventCutSceneDemo(actor);
}

void endEventCutSceneTalkOnlyRequeterDemoOrTryEndEventCutSceneDemoBySkip(al::LiveActor* actor) {
    if (isWaitDemoSkipEnd(actor))
        endEventCutSceneDemoBySkip(actor);
    else
        endEventCutSceneTalkOnlyRequeterDemo(actor);
}

void requestEndEventDemo(al::LiveActor* actor) {
    getEventDemoCtrl(actor)->requestEndDemo(actor);
}

void requestEndEventDemo(al::EventFlowNode* node) {
    getEventDemoCtrl(node)->requestEndDemo(node->getActor());
}

bool isActiveEventDemo(const al::EventFlowNode* node) {
    return getEventDemoCtrl(node)->isActiveDemo();
}

bool isActiveEventDemo(const al::LiveActor* actor) {
    return getEventDemoCtrl(actor)->isActiveDemo();
}

bool isEqualEventDemoStartActor(const al::LiveActor* actor) {
    return getEventDemoCtrl(actor)->isDemoStartActor(actor);
}

bool tryHideDemoPlayerIfRequested(al::LiveActor* actor, al::EventFlowExecutor* executor) {
    if (!executor->getEventFlowDataHolder()->getRequestInfo()->isRequestHideDemoPlayer())
        return false;
    executor->getEventFlowDataHolder()->getRequestInfo()->resetRequestHideDemoPlayer();
    if (executor->getEventFlowDataHolder()->getRequestInfo()->isDemoPlayerHidden())
        return false;
    hideDemoPlayer(actor);
    startActionDemoPlayer(actor, "Wait");
    executor->getEventFlowDataHolder()->getRequestInfo()->setDemoPlayerHidden(true);
    return true;
}

bool tryShowDemoPlayerIfRequested(al::LiveActor* actor, al::EventFlowExecutor* executor) {
    if (!executor->getEventFlowDataHolder()->getRequestInfo()->isRequestShowDemoPlayer())
        return false;
    executor->getEventFlowDataHolder()->getRequestInfo()->resetRequestShowDemoPlayer();
    if (!executor->getEventFlowDataHolder()->getRequestInfo()->isDemoPlayerHidden())
        return false;
    showDemoPlayer(actor);
    executor->getEventFlowDataHolder()->getRequestInfo()->setDemoPlayerHidden(false);
    return true;
}

bool tryStartDemoPlayerActionIfRequested(al::LiveActor* actor, al::EventFlowExecutor* executor) {
    if (executor->getEventFlowDataHolder()->getRequestInfo()->getRequestedDemoPlayerActionName() ==
        nullptr)
        return false;
    startActionDemoPlayer(
        actor,
        executor->getEventFlowDataHolder()->getRequestInfo()->getRequestedDemoPlayerActionName());
    executor->getEventFlowDataHolder()->getRequestInfo()->clearRequestedDemoPlayerActionName();
    return true;
}

void swapEventCharacterName(al::EventFlowExecutor* executor,
                            const sead::WBufferedSafeString* name) {
    executor->getEventFlowDataHolder()->swapCharacterName(name);
}

void resetEventCharacterName(al::EventFlowExecutor* executor) {
    executor->getEventFlowDataHolder()->resetCharacterName();
}

void startOpenNpcDemoEventTalkMessage(al::EventFlowNode* node, const char16* message) {
    startOpenTalkMessage(node, message, false, false, 0);
}

void startOpenNpcDemoEventTalkMessageWithButtonA(al::EventFlowNode* node, const char16* message) {
    startOpenTalkMessage(node, message, true, false, 0);
}

void startOpenNpcDemoEventSystemMessageWithButtonA(al::EventFlowNode* node, const char16* message,
                                                   s32 messageStyle) {
    startOpenTalkMessage(node, message, true, true, messageStyle);
}

void startCloseNpcDemoEventTalkMessage(al::LiveActor* actor) {
    getNpcEventCtrlInfo(actor)->requestCloseTalkMessage(actor);
}

bool isOpenWaitNpcDemoEventTalkMessage(const al::LiveActor* actor) {
    return getNpcEventCtrlInfo(actor)->isOpenWaitTalkMessage();
}

bool isPlayingTextPaneAnimEventTalkMessage(const al::LiveActor* actor) {
    return getNpcEventCtrlInfo(actor)->isPlayingTextPaneAnim();
}

bool isEndNpcDemoEventTalkMessage(const al::EventFlowNode* node) {
    if (!getNpcEventCtrlInfo(node)->isTalkMessageClosed())
        return false;
    return getNpcEventCtrlInfo(node)->isTalkMessageRequestEnd();
}

bool isCloseNpcDemoEventTalkMessage(const al::LiveActor* actor) {
    return getNpcEventCtrlInfo(actor)->isCloseTalk();
}

s64 getEventTalkWindowMessageStyle(const al::EventFlowNode* node) {
    return getNpcEventCtrlInfo(node)->getEventTalkWindowMessageStyle();
}

bool isCloseEventWipeFadeBlack(const al::EventFlowNode* node) {
    return getNpcEventCtrlInfo(node)->isCloseWipeFadeBlack();
}

bool isOpenEventWipeFadeBlack(const al::EventFlowNode* node) {
    return getNpcEventCtrlInfo(node)->isOpenWipeFadeBlack();
}

void requestCloseEventWipeFadeBlack(al::EventFlowNode* node, s32 step) {
    getNpcEventCtrlInfo(node)->requestCloseWipeFadeBlack(node, step);
}

void requestOpenEventWipeFadeBlack(al::EventFlowNode* node, s32 step) {
    getNpcEventCtrlInfo(node)->requestOpenWipeFadeBlack(node, step);
}

void calcPlayerWatchTrans(sead::Vector3f* out, const al::LiveActor* actor,
                          const TalkNpcParam* param) {
    const al::EventFlowWatchParam* watchParam = param->getEventFlowWatchParam();
    if (watchParam != nullptr) {
        watchParam->calcWatchTrans(out, actor);
        return;
    }
    const sead::Vector3f& trans = al::getTrans(actor);
    out->set(trans);
}

bool isPlayerInWater(const al::EventFlowNode* node) {
    return getNpcEventCtrlInfo(node)->isPlayerInWater();
}

bool isNpcScareTiming(const al::EventFlowExecutor* executor) {
    return al::isScare(executor);
}

bool isExistNpcLookPos(const al::LiveActor* actor) {
    return getNpcEventCtrlInfo(actor)->isExistNpcLookPos();
}

bool isExistNpcLookPos(const al::EventFlowExecutor* executor) {
    return getNpcEventCtrlInfo(executor->getActor())->isExistNpcLookPos();
}

const sead::Vector3f& getNpcLookPos(const al::LiveActor* actor) {
    return getNpcEventCtrlInfo(actor)->getNpcLookPos();
}

const sead::Vector3f& getNpcLookPos(const al::EventFlowExecutor* executor) {
    return getNpcEventCtrlInfo(executor->getActor())->getNpcLookPos();
}

bool isExistTrafficAreaDirector(const al::LiveActor* actor) {
    return tryGetTrafficAreaDirector(actor) != nullptr;
}

bool tryPermitEnterTrafficNpcAndSyncDrawClipping(al::LiveActor* actor) {
    al::TrafficAreaDirector* director = tryGetTrafficAreaDirector(actor);
    if (director == nullptr)
        return true;
    return director->tryPermitEnterNpcAndSyncDrawClipping(actor);
}

bool tryPermitEnterTrafficCar(const al::LiveActor* actor, const sead::Vector3f& position) {
    al::TrafficAreaDirector* director = tryGetTrafficAreaDirector(actor);
    if (director == nullptr)
        return true;
    return director->tryPermitEnterCar(position);
}

void requestSwitchTalkNpcEventAfterDoorSnow(al::LiveActor* actor, s32 eventId) {
    switch (eventId) {
    case 1:
        getTalkNpcSceneEventSwitcher(actor)->requestAfterDoorSnow(1);
        break;
    case 2:
        getTalkNpcSceneEventSwitcher(actor)->requestAfterDoorSnow(2);
        break;
    case 3:
        getTalkNpcSceneEventSwitcher(actor)->requestAfterDoorSnow(3);
        break;
    case 4:
        getTalkNpcSceneEventSwitcher(actor)->requestAfterDoorSnow(4);
        break;
    }
}

void requestSwitchTalkNpcEventVolleyBall(al::LiveActor* actor, s32 shineIndex) {
    if (shineIndex == 1)
        getTalkNpcSceneEventSwitcher(actor)->requestVolleyBall("VolleyBallAppearShine1");
    else if (shineIndex == 2)
        getTalkNpcSceneEventSwitcher(actor)->requestVolleyBall("VolleyBallAppearShine2");
}

void requestSwitchTalkNpcEventJumpingRope(al::LiveActor* actor, s32 shineIndex) {
    if (shineIndex == 1)
        getTalkNpcSceneEventSwitcher(actor)->requestJumpingRope("JumpingRopeAppearShine1");
    else if (shineIndex == 2)
        getTalkNpcSceneEventSwitcher(actor)->requestJumpingRope("JumpingRopeAppearShine2");
}

void requestSwitchTalkNpcEventRadicon(al::LiveActor* actor, s32 shineIndex) {
    if (shineIndex == 1)
        getTalkNpcSceneEventSwitcher(actor)->requestRadicon("RadiconAppearShine1");
    else if (shineIndex == 2)
        getTalkNpcSceneEventSwitcher(actor)->requestRadicon("RadiconAppearShine2");
}

bool isEventAfterDoorSnow1(const al::EventFlowNode* node) {
    return getTalkNpcSceneEventSwitcher(node)->getAfterDoorSnowEventId() == 1;
}

bool isEventAfterDoorSnow2(const al::EventFlowNode* node) {
    return getTalkNpcSceneEventSwitcher(node)->getAfterDoorSnowEventId() == 2;
}

bool isEventAfterDoorSnow3(const al::EventFlowNode* node) {
    return getTalkNpcSceneEventSwitcher(node)->getAfterDoorSnowEventId() == 3;
}

bool isEventAfterDoorSnow4(const al::EventFlowNode* node) {
    return getTalkNpcSceneEventSwitcher(node)->getAfterDoorSnowEventId() == 4;
}

const char* tryGetTalkNpcVolleyBallEntryName(const al::LiveActor* actor) {
    return getTalkNpcSceneEventSwitcher(actor)->getVolleyBallEntryName();
}

const char* tryGetTalkNpcJumpingRopeEntryName(const al::LiveActor* actor) {
    return getTalkNpcSceneEventSwitcher(actor)->getJumpingRopeEntryName();
}

const char* tryGetTalkNpcRadiconEntryName(const al::LiveActor* actor) {
    return getTalkNpcSceneEventSwitcher(actor)->getRadiconEntryName();
}

void requestEventGetShineDirect(al::EventFlowNode* node, Shine* shine) {
    getSceneExecuteCtrl(node)->requestEventGetShineDirect(node->getActor(), shine);
}

void requestEventOpenBgmList(al::EventFlowNode* node) {
    getSceneExecuteCtrl(node)->requestEventOpenBgmList(node->getActor());
}

void requestEventOpenShineList(al::LiveActor* actor) {
    getSceneExecuteCtrl(actor)->requestEventOpenShineList(actor);
}

void requestEventGetAchievement(al::LiveActor* actor, const char* achievementName) {
    getSceneExecuteCtrl(actor)->requestEventGetAchievement(actor, achievementName);
}

bool checkEndSceneExecuteAndResetRequest(al::EventFlowNode* node) {
    return getSceneExecuteCtrl(node)->checkEndSceneExecuteAndReset(node->getActor());
}

bool checkEndSceneExecuteAndResetRequest(al::LiveActor* actor) {
    return getSceneExecuteCtrl(actor)->checkEndSceneExecuteAndReset(actor);
}

bool isExecuteSceneEvent(const al::LiveActor* actor) {
    return getSceneExecuteCtrl(actor)->isExecuteScene();
}

bool checkTriggerDecideWithRequestIcon(al::LiveActor* actor, const sead::Vector3f& offset,
                                       f32 distance) {
    NpcEventBalloonRequestInfo requestInfo;
    requestInfo.localOffset.set(offset);
    if (distance > 0.0f)
        requestInfo.collisionCheckOffsetRadius = distance;

    bool result = false;
    if (isPlayerEnableTalkGround(actor)) {
        requestNpcEventActionBalloonMessage(actor, true, requestInfo);
        if (isTriggerUiDecide(getSceneObjHolder(actor)) &&
            getNpcEventCtrlInfo(actor)->isSuccessBalloonMessage(actor)) {
            return true;
        }
    }
    return result;
}

void skipEventDemo(al::EventFlowExecutor* executor) {
    if (al::isHideDemoPlayer(executor))
        showDemoPlayer(executor->getActor());
    if (!getNpcEventCtrlInfo(executor->getActor())->isCloseTalk()) {
        al::LiveActor* actor = executor->getActor();
        getNpcEventCtrlInfo(actor)->requestCloseTalkMessage(actor);
    }
    executor->getEventFlowDataHolder()->endAllEventCamera(getCameraUser(executor->getActor()));
    executor->clearCurrentNodeAndName();
    al::notifyDemoSkipToDemoSyncedProc(getAudioUser(executor->getActor()));
}

}  // namespace rs

namespace TalkNpcFunction {

bool receiveEventChangeWaitAction(TalkNpcActionAnimInfo* actionAnimInfo,
                                  const al::EventFlowEventData* eventData,
                                  const TalkNpcParam* param) {
    if (!al::isEventName(eventData, "ChangeWaitAction"))
        return false;
    const char* actionName = al::getEventDataParamString(eventData, "ActionName");
    actionAnimInfo->changeWaitActionName(actionName, param);
    return true;
}

}  // namespace TalkNpcFunction

namespace {

const al::MessageSystem* MessageSystemAccessor::getMessageSystem() const {
    return mMessageSystem;
}

}  // namespace

namespace rs {

bool isGreaterBindPriority(const al::HitSensor* lhs, const al::HitSensor* rhs) {
    if (lhs == rhs)
        return false;
    if (rhs == nullptr)
        return true;
    if (al::isSensorBindable(lhs) && al::isSensorBindable(rhs))
        return false;
    if (al::isSensorBindableAllPlayer(lhs) && al::isSensorBindableAllPlayer(rhs))
        return false;
    return getBindPriority(rhs) <= getBindPriority(lhs);
}

}  // namespace rs
