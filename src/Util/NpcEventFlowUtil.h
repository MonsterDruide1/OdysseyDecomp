#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>

namespace al {
struct ActorInitInfo;
class EventFlowChoiceInfo;
class EventFlowEventData;
class EventFlowExecutor;
class EventFlowMovement;
class EventFlowNode;
class HitSensor;
class IEventFlowActionNameConverter;
class IEventFlowQueryJudge;
class LiveActor;
class MessageTagDataHolder;
}  // namespace al
struct NpcEventBalloonRequestInfo;
class Shine;
class TalkNpcActionAnimInfo;
class TalkNpcParam;

namespace rs {
al::EventFlowExecutor* initEventFlow(al::LiveActor*, const al::ActorInitInfo&, const char*,
                                     const char*);
al::EventFlowExecutor* initEventFlowForRunAwayNpc(al::LiveActor*, const al::ActorInitInfo&,
                                                  const char*, const char*);
al::EventFlowExecutor* initEventFlowForSystem(al::LiveActor*, const al::ActorInitInfo&, const char*,
                                              const char*, const char*);
al::EventFlowExecutor* initEventFlowSuffix(al::LiveActor*, const al::ActorInitInfo&, const char*,
                                           const char*, const char*);
al::EventFlowExecutor* initEventFlowFromPlacementInfo(al::LiveActor*, const al::ActorInitInfo&,
                                                      const char*);
al::EventFlowExecutor* initCommonEventFlow(al::LiveActor*, const al::ActorInitInfo&, const char*);
al::EventFlowExecutor* createCommonEventFlowFromPlacementInfo(al::LiveActor*,
                                                              const al::ActorInitInfo&);
void initEventCharacterName(al::EventFlowExecutor*, const al::ActorInitInfo&, const char*);
void makeEventCharacterName(sead::WBufferedSafeString*, const al::ActorInitInfo&, const char*);
void initEventParam(al::EventFlowExecutor*, const TalkNpcParam*, const char*);
bool tryInitItemKeeperByEvent(al::LiveActor*, const al::ActorInitInfo&,
                              const al::EventFlowExecutor*);
void startEventFlow(al::EventFlowExecutor*, const char*);
bool updateEventFlow(al::EventFlowExecutor*);
void stopEventFlow(al::EventFlowExecutor*);
void restartEventFlow(al::EventFlowExecutor*);
void initEventMovementRail(al::EventFlowExecutor*, const al::ActorInitInfo&);
void initEventMovement(al::EventFlowExecutor*, al::EventFlowMovement*, const al::ActorInitInfo&);
void initEventMovementTurnSeparate(al::EventFlowExecutor*, const al::ActorInitInfo&);
void initEventMovementWait(al::EventFlowExecutor*, const al::ActorInitInfo&);
void initEventMovementWander(al::EventFlowExecutor*, const al::ActorInitInfo&);
bool isDefinedEventCamera(const al::EventFlowExecutor*, const char*);
void initEventActionNameConverter(al::EventFlowExecutor*, const al::IEventFlowActionNameConverter*);
void initEventQueryJudge(al::EventFlowExecutor*, const al::IEventFlowQueryJudge*);
void initEventMessageTagDataHolder(al::EventFlowExecutor*, const al::MessageTagDataHolder*);
void initEventCameraObject(al::EventFlowExecutor* flowExecutor, const al::ActorInitInfo& initInfo,
                           const char* name);
void initEventCameraObjectAfterKeepPose(al::EventFlowExecutor* flowExecutor,
                                        const al::ActorInitInfo& initInfo, const char* name);
void initEventCameraTalk(al::EventFlowExecutor*, const al::ActorInitInfo&, const char*, f32);
void initEventCameraTalk2(al::EventFlowExecutor*, const al::ActorInitInfo&, const char*);
void initEventCameraLookBoard(al::EventFlowExecutor*, const al::ActorInitInfo&,
                              const sead::Vector3f&, const char*);
void initEventCameraAnim(al::EventFlowExecutor*, const al::LiveActor*, const al::ActorInitInfo&,
                         const char*);
void initEventCameraFixActor(al::EventFlowExecutor*, const al::ActorInitInfo&, const char*,
                             const al::LiveActor*, const sead::Vector3f*, f32, f32, f32);
void initEventCameraFixActor2(al::EventFlowExecutor*, const al::ActorInitInfo&, const char*,
                              const al::LiveActor*, const sead::Vector3f*, f32, f32, f32, bool);
void initEventCameraFixActorAutoAroundFront(al::EventFlowExecutor*, const al::ActorInitInfo&,
                                            const char*, const al::LiveActor*,
                                            const sead::Vector3f*, f32, f32, f32, bool);
void initEventCameraFixActorAutoAroundFront2(al::EventFlowExecutor*, const al::ActorInitInfo&,
                                             const char*, const al::LiveActor*,
                                             const sead::Vector3f*, f32, f32, f32);
u32 checkMatchPatternCurrentCostume(bool*, bool*, const al::LiveActor*, const char*);
bool checkMatchPatternCurrentCostumePair(const al::LiveActor*, const char*);
bool isSuccessNpcEventBalloonMessage(const al::LiveActor*);
bool isPlayingNpcEventBalloonMessageVoice(const al::LiveActor*);
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
void requestNpcEventActionBalloonMessage(al::LiveActor*, bool, const NpcEventBalloonRequestInfo&);
void setEventBalloonFilterOnlyMiniGame(const al::LiveActor*);
void resetEventBalloonFilter(const al::LiveActor*);
void startChoiceEvent(al::EventFlowNode*, al::EventFlowChoiceInfo*);
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
void endEventCutSceneDemoOrTryEndEventCutSceneDemoBySkip(al::LiveActor*);
void endEventCutSceneTalkOnlyRequeterDemoOrTryEndEventCutSceneDemoBySkip(al::LiveActor*);
void requestEndEventDemo(al::LiveActor*);
void requestEndEventDemo(al::EventFlowNode*);
bool isActiveEventDemo(const al::EventFlowNode*);
bool isActiveEventDemo(const al::LiveActor*);
bool isEqualEventDemoStartActor(const al::LiveActor*);
bool tryHideDemoPlayerIfRequested(al::LiveActor*, al::EventFlowExecutor*);
bool tryShowDemoPlayerIfRequested(al::LiveActor*, al::EventFlowExecutor*);
bool tryStartDemoPlayerActionIfRequested(al::LiveActor*, al::EventFlowExecutor*);
void swapEventCharacterName(al::EventFlowExecutor*, const sead::WBufferedSafeString*);
void resetEventCharacterName(al::EventFlowExecutor*);
void startOpenNpcDemoEventTalkMessage(al::EventFlowNode*, const char16*);
void startOpenNpcDemoEventTalkMessageWithButtonA(al::EventFlowNode*, const char16*);
void startOpenNpcDemoEventSystemMessageWithButtonA(al::EventFlowNode*, const char16*, s32);
void startCloseNpcDemoEventTalkMessage(al::LiveActor*);
bool isOpenWaitNpcDemoEventTalkMessage(const al::LiveActor*);
bool isPlayingTextPaneAnimEventTalkMessage(const al::LiveActor*);
bool isEndNpcDemoEventTalkMessage(const al::EventFlowNode*);
bool isCloseNpcDemoEventTalkMessage(const al::LiveActor*);
s64 getEventTalkWindowMessageStyle(const al::EventFlowNode*);
bool isCloseEventWipeFadeBlack(const al::EventFlowNode*);
bool isOpenEventWipeFadeBlack(const al::EventFlowNode*);
void requestCloseEventWipeFadeBlack(al::EventFlowNode*, s32);
void requestOpenEventWipeFadeBlack(al::EventFlowNode*, s32);
void calcPlayerWatchTrans(sead::Vector3f*, const al::LiveActor*, const TalkNpcParam*);
bool isPlayerInWater(const al::EventFlowNode*);
bool isNpcScareTiming(const al::EventFlowExecutor*);
bool isExistNpcLookPos(const al::LiveActor*);
bool isExistNpcLookPos(const al::EventFlowExecutor*);
const sead::Vector3f& getNpcLookPos(const al::LiveActor*);
const sead::Vector3f& getNpcLookPos(const al::EventFlowExecutor*);
bool isExistTrafficAreaDirector(const al::LiveActor*);
bool tryPermitEnterTrafficNpcAndSyncDrawClipping(al::LiveActor*);
bool tryPermitEnterTrafficCar(const al::LiveActor*, const sead::Vector3f&);
void requestSwitchTalkNpcEventAfterDoorSnow(al::LiveActor*, s32);
void requestSwitchTalkNpcEventVolleyBall(al::LiveActor*, s32);
void requestSwitchTalkNpcEventJumpingRope(al::LiveActor*, s32);
void requestSwitchTalkNpcEventRadicon(al::LiveActor*, s32);
bool isEventAfterDoorSnow1(const al::EventFlowNode*);
bool isEventAfterDoorSnow2(const al::EventFlowNode*);
bool isEventAfterDoorSnow3(const al::EventFlowNode*);
bool isEventAfterDoorSnow4(const al::EventFlowNode*);
const char* tryGetTalkNpcVolleyBallEntryName(const al::LiveActor*);
const char* tryGetTalkNpcJumpingRopeEntryName(const al::LiveActor*);
const char* tryGetTalkNpcRadiconEntryName(const al::LiveActor*);
void requestEventGetShineDirect(al::EventFlowNode*, Shine*);
void requestEventOpenBgmList(al::EventFlowNode*);
void requestEventOpenShineList(al::LiveActor*);
void requestEventGetAchievement(al::LiveActor*, const char*);
bool checkEndSceneExecuteAndResetRequest(al::EventFlowNode*);
bool checkEndSceneExecuteAndResetRequest(al::LiveActor*);
bool isExecuteSceneEvent(const al::LiveActor*);
bool checkTriggerDecideWithRequestIcon(al::LiveActor*, const sead::Vector3f&, f32);
void skipEventDemo(al::EventFlowExecutor*);
bool isGreaterBindPriority(const al::HitSensor*, const al::HitSensor*);
}  // namespace rs

namespace TalkNpcFunction {
bool receiveEventChangeWaitAction(TalkNpcActionAnimInfo*, const al::EventFlowEventData*,
                                  const TalkNpcParam*);
}  // namespace TalkNpcFunction
