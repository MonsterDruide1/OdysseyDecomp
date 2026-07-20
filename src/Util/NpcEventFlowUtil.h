#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>

namespace al {
struct ActorInitInfo;
class EventFlowExecutor;
class LiveActor;
class MessageTagDataHolder;
class EventFlowMovement;
class IEventFlowActionNameConverter;
class IEventFlowQueryJudge;
class EventFlowNode;
struct EventFlowChoiceInfo;
class EventFlowEventData;
}  // namespace al

class TalkNpcParam;
struct NpcEventBalloonRequestInfo;
class Shine;
struct TalkNpcActionAnimInfo;

namespace rs {
al::EventFlowExecutor* initEventFlow(al::LiveActor* actor, const al::ActorInitInfo& initInfo,
                                     const char*, const char*);
al::EventFlowExecutor* initEventFlowForRunAwayNpc(al::LiveActor* actor,
                                                  const al::ActorInitInfo& initInfo, const char*,
                                                  const char*);
al::EventFlowExecutor* initEventFlowForSystem(al::LiveActor* actor,
                                              const al::ActorInitInfo& initInfo, const char*,
                                              const char*, const char*);
al::EventFlowExecutor* initEventFlowSuffix(al::LiveActor* actor, const al::ActorInitInfo& initInfo,
                                           const char*, const char*, const char*);
al::EventFlowExecutor* initEventFlowFromPlacementInfo(al::LiveActor* actor,
                                                      const al::ActorInitInfo& initInfo,
                                                      const char*);
al::EventFlowExecutor* initCommonEventFlow(al::LiveActor* actor, const al::ActorInitInfo& initInfo,
                                           const char*);
al::EventFlowExecutor* createCommonEventFlowFromPlacementInfo(al::LiveActor* actor,
                                                              const al::ActorInitInfo& initInfo);
void initEventCharacterName(al::EventFlowExecutor* executor, const al::ActorInitInfo& initInfo,
                            const char*);
void makeEventCharacterName(sead::WBufferedSafeString*, const al::ActorInitInfo& initInfo,
                            const char*);
void initEventParam(al::EventFlowExecutor* executor, const TalkNpcParam*, const char*);
bool tryInitItemKeeperByEvent(al::LiveActor* actor, const al::ActorInitInfo& initInfo,
                              const al::EventFlowExecutor*);
void startEventFlow(al::EventFlowExecutor* executor, const char*);
bool updateEventFlow(al::EventFlowExecutor*);
void stopEventFlow(al::EventFlowExecutor*);
void restartEventFlow(al::EventFlowExecutor*);
void initEventMovementRail(al::EventFlowExecutor* executor, const al::ActorInitInfo& initInfo);
void initEventMovement(al::EventFlowExecutor* executor, al::EventFlowMovement*,
                       const al::ActorInitInfo& initInfo);
void initEventMovementTurnSeparate(al::EventFlowExecutor* executor,
                                   const al::ActorInitInfo& initInfo);
void initEventMovementWait(al::EventFlowExecutor* executor, const al::ActorInitInfo& initInfo);
void initEventMovementWander(al::EventFlowExecutor* executor, const al::ActorInitInfo& initInfo);
void initEventCameraObject(al::EventFlowExecutor* executor, const al::ActorInitInfo& initInfo,
                           const char* name);
void initEventCameraObjectAfterKeepPose(al::EventFlowExecutor* executor,
                                        const al::ActorInitInfo& initInfo, const char*);
void initEventCameraTalk(al::EventFlowExecutor* executor, const al::ActorInitInfo& initInfo,
                         const char*, f32);
void initEventCameraTalk2(al::EventFlowExecutor* executor, const al::ActorInitInfo& initInfo,
                          const char*);
void initEventCameraLookBoard(al::EventFlowExecutor* executor, const al::ActorInitInfo& initInfo,
                              const sead::Vector3f&, const char*);
void initEventCameraAnim(al::EventFlowExecutor* executor, const al::LiveActor* actor,
                         const al::ActorInitInfo& initInfo, const char*);
void initEventCameraFixActor(al::EventFlowExecutor* executor, const al::ActorInitInfo& initInfo,
                             const char*, const al::LiveActor* actor, const sead::Vector3f*, f32,
                             f32, f32);
void initEventCameraFixActor2(al::EventFlowExecutor* executor, const al::ActorInitInfo& initInfo,
                              const char*, const al::LiveActor* actor, const sead::Vector3f*, f32,
                              f32, f32, bool);
void initEventCameraFixActorAutoAroundFront(al::EventFlowExecutor* executor,
                                            const al::ActorInitInfo& initInfo, const char*,
                                            const al::LiveActor* actor, const sead::Vector3f*, f32,
                                            f32, f32, bool);
void initEventCameraFixActorAutoAroundFront2(al::EventFlowExecutor* executor,
                                             const al::ActorInitInfo& initInfo, const char*,
                                             const al::LiveActor* actor, const sead::Vector3f*, f32,
                                             f32, f32);
bool isDefinedEventCamera(const al::EventFlowExecutor* executor, const char*);
void initEventActionNameConverter(al::EventFlowExecutor* executor,
                                  const al::IEventFlowActionNameConverter*);
void initEventQueryJudge(al::EventFlowExecutor* executor, const al::IEventFlowQueryJudge*);
void initEventMessageTagDataHolder(al::EventFlowExecutor* executor,
                                   const al::MessageTagDataHolder*);
void checkMatchPatternCurrentCostume(bool*, bool*, const al::LiveActor* actor, const char*);
bool checkMatchPatternCurrentCostumePair(const al::LiveActor* actor, const char*);
bool isSuccessNpcEventBalloonMessage(const al::LiveActor* actor);
bool isPlayingNpcEventBalloonMessageVoice(const al::LiveActor* actor);
void requestNpcEventBalloonMessage(al::EventFlowNode*, const char16*,
                                   const NpcEventBalloonRequestInfo&);
void requestNpcEventTalkBalloonMessage(al::EventFlowNode*, const char*,
                                       const NpcEventBalloonRequestInfo&);
void requestNpcEventTalkBalloonMessageEntranceCamera(al::EventFlowNode*, const char*,
                                                     const NpcEventBalloonRequestInfo&);
void requestNpcEventTalkBalloonMessageWithEnableButtonA(al::EventFlowNode*, const char*,
                                                        const NpcEventBalloonRequestInfo&);
void requestNpcEventTalkBalloonMessageWithDisableButtonA(al::EventFlowNode*, const char*,
                                                         const NpcEventBalloonRequestInfo&);
void requestNpcEventActionBalloonMessage(al::LiveActor* actor, bool,
                                         const NpcEventBalloonRequestInfo&);
void setEventBalloonFilterOnlyMiniGame(const al::LiveActor* actor);
void resetEventBalloonFilter(const al::LiveActor* actor);
void startChoiceEvent(al::EventFlowNode*, al::EventFlowChoiceInfo*);
bool isSuccessLockEventTalkDemo(const al::EventFlowNode*);
bool tryLockStartEventTalkDemo(al::EventFlowNode*);
bool tryLockStartEventTalkDemoWithoutBalloon(al::EventFlowNode*);
bool tryStartEventTalkDemo(al::EventFlowNode*);
bool tryStartEventTalkOnlyRequesterDemo(al::EventFlowNode*);
bool tryStartEventTalkKeepHackDemo(al::EventFlowNode*);
bool tryStartEventTalkUseCoinDemo(al::EventFlowNode*);
bool tryStartEventNormalDemo(al::EventFlowNode*);
bool tryStartEventKeepBindDemo(al::LiveActor* actor);
bool tryStartEventCutSceneDemo(al::LiveActor* actor);
bool tryStartEventCutSceneKeepHackDemo(al::LiveActor* actor);
bool tryStartEventCutSceneTalkOnlyRequesterDemo(al::LiveActor* actor);
void endEventCutSceneDemo(al::LiveActor* actor);
void endEventCutSceneTalkOnlyRequeterDemo(al::LiveActor* actor);
void endEventCutSceneDemoBySkip(al::LiveActor* actor);
void endEventCutSceneDemoOrTryEndEventCutSceneDemoBySkip(al::LiveActor* actor);
void endEventCutSceneTalkOnlyRequeterDemoOrTryEndEventCutSceneDemoBySkip(al::LiveActor* actor);
void requestEndEventDemo(al::LiveActor* actor);
void requestEndEventDemo(al::EventFlowNode*);
bool isActiveEventDemo(const al::EventFlowNode*);
bool isActiveEventDemo(const al::LiveActor* actor);
bool isEqualEventDemoStartActor(const al::LiveActor* actor);
bool tryHideDemoPlayerIfRequested(al::LiveActor* actor, al::EventFlowExecutor*);
bool tryShowDemoPlayerIfRequested(al::LiveActor* actor, al::EventFlowExecutor*);
bool tryStartDemoPlayerActionIfRequested(al::LiveActor* actor, al::EventFlowExecutor*);
void swapEventCharacterName(al::EventFlowExecutor* executor, const sead::WBufferedSafeString*);
void resetEventCharacterName(al::EventFlowExecutor*);
void startOpenNpcDemoEventTalkMessage(al::EventFlowNode*, const char16*);
void startOpenNpcDemoEventTalkMessageWithButtonA(al::EventFlowNode*, const char16*);
void startOpenNpcDemoEventSystemMessageWithButtonA(al::EventFlowNode*, const char16*, s32);
void startCloseNpcDemoEventTalkMessage(al::LiveActor* actor);
bool isOpenWaitNpcDemoEventTalkMessage(const al::LiveActor* actor);
bool isPlayingTextPaneAnimEventTalkMessage(const al::LiveActor* actor);
bool isEndNpcDemoEventTalkMessage(const al::EventFlowNode*);
bool isCloseNpcDemoEventTalkMessage(const al::LiveActor* actor);
const char* getEventTalkWindowMessageStyle(const al::EventFlowNode*);
bool isCloseEventWipeFadeBlack(const al::EventFlowNode*);
bool isOpenEventWipeFadeBlack(const al::EventFlowNode*);
void requestCloseEventWipeFadeBlack(al::EventFlowNode*, s32);
void requestOpenEventWipeFadeBlack(al::EventFlowNode*, s32);
void calcPlayerWatchTrans(sead::Vector3f*, const al::LiveActor* actor, const TalkNpcParam*);
bool isPlayerInWater(const al::EventFlowNode*);
bool isNpcScareTiming(const al::EventFlowExecutor*);
bool isExistNpcLookPos(const al::LiveActor* actor);
bool isExistNpcLookPos(const al::EventFlowExecutor*);
sead::Vector3f* getNpcLookPos(const al::LiveActor* actor);
sead::Vector3f* getNpcLookPos(const al::EventFlowExecutor*);
bool isExistTrafficAreaDirector(const al::LiveActor* actor);
bool tryPermitEnterTrafficNpcAndSyncDrawClipping(al::LiveActor* actor);
bool tryPermitEnterTrafficCar(const al::LiveActor* actor, const sead::Vector3f&);
void requestSwitchTalkNpcEventAfterDoorSnow(al::LiveActor* actor, s32 doorIndex);
void requestSwitchTalkNpcEventVolleyBall(al::LiveActor* actor, s32);
void requestSwitchTalkNpcEventJumpingRope(al::LiveActor* actor, s32);
void requestSwitchTalkNpcEventRadicon(al::LiveActor* actor, s32);
bool isEventAfterDoorSnow1(const al::EventFlowNode*);
bool isEventAfterDoorSnow2(const al::EventFlowNode*);
bool isEventAfterDoorSnow3(const al::EventFlowNode*);
bool isEventAfterDoorSnow4(const al::EventFlowNode*);
const char* tryGetTalkNpcVolleyBallEntryName(const al::LiveActor* actor);
const char* tryGetTalkNpcJumpingRopeEntryName(const al::LiveActor* actor);
const char* tryGetTalkNpcRadiconEntryName(const al::LiveActor* actor);
void requestEventGetShineDirect(al::EventFlowNode*, Shine*);
void requestEventOpenBgmList(al::EventFlowNode*);
void requestEventOpenShineList(al::LiveActor* actor);
void requestEventGetAchievement(al::LiveActor* actor, const char*);
bool checkEndSceneExecuteAndResetRequest(al::EventFlowNode*);
bool checkEndSceneExecuteAndResetRequest(al::LiveActor* actor);
bool isExecuteSceneEvent(const al::LiveActor* actor);
bool checkTriggerDecideWithRequestIcon(al::LiveActor* actor, const sead::Vector3f&, f32);
void skipEventDemo(al::EventFlowExecutor*);
}  // namespace rs

namespace TalkNpcFunction {
bool receiveEventChangeWaitAction(TalkNpcActionAnimInfo*, const al::EventFlowEventData*,
                                  const TalkNpcParam*);
}
