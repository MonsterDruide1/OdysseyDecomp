#include "Npc/SessionMayorNpc.h"

#include "Library/Base/StringUtil.h"
#include "Library/Demo/DemoFunction.h"
#include "Library/Event/EventFlowFunction.h"
#include "Library/Event/EventFlowUtil.h"
#include "Library/Joint/JointControllerKeeper.h"
#include "Library/Joint/JointSpringControllerHolder.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorParamHolderUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"

#include "Npc/NpcStateReactionParam.h"
#include "Npc/RandomWaitActionUpdater.h"
#include "Npc/SessionMayorParam.h"
#include "Npc/SessionMusicianLocalFunction.h"
#include "Npc/SessionMusicianManager.h"
#include "Npc/TalkNpcActionAnimInfo.h"
#include "System/GameDataFunction.h"
#include "Util/DemoUtil.h"
#include "Util/NpcActionUtil.h"
#include "Util/NpcAnimUtil.h"
#include "Util/NpcEventFlowUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_HOST_TYPE_IMPL(SessionMayorNpc, Wait);
NERVE_HOST_TYPE_IMPL(SessionMayorNpc, Reaction);

NERVES_MAKE_STRUCT(HostType, Wait, Reaction);
}  // namespace

static const char* sCameraNames[] = {
    "FocusDrum",
    "FocusBass",
    "FocusGuitar",
    "FocusTrumpet",
};

inline const char* getActionName(SessionEventProgress progress) {
    switch (progress) {
    case SessionEventProgress::Entry:
    case SessionEventProgress::Wait1stMusician:
    case SessionEventProgress::RequestGoToThePowerPlant:
    case SessionEventProgress::WaitThePowerPlantWorks:
        return "WaitTrouble";
    case SessionEventProgress::Wait2ndMusician:
    case SessionEventProgress::Wait3rdMusician:
        return "StandWait";
    case SessionEventProgress::Wait4thMusician:
    case SessionEventProgress::TheCeremonyIsReady:
    case SessionEventProgress::GoForTheCeremony:
        return "WaitSmile";
    default:
        return "StandWait";
    }
}

static inline SessionEventProgress getProgress(const char* eventName) {
    SessionEventProgress progress;
    for (s32 i = 0; i < SessionEventProgress::size(); i++) {
        if (al::isEqualString(SessionEventProgress::text(i), eventName)) {
            progress.setRelativeIndex(i);
            break;
        }
    }
    return progress;
}

inline void startEventFlow(al::EventFlowExecutor* eventFlow, SessionMayorNpc* actor) {
    GameDataHolderAccessor accessor(actor);
    rs::startEventFlow(eventFlow, GameDataFunction::getSessionEventProgress(accessor).text());
}

void SessionMayorNpc::init(const al::ActorInitInfo& initInfo) {
    al::initActorWithArchiveName(this, initInfo, "CityMayor", nullptr);

    mMayorParam = new SessionMayorParam(this);

    mTalkNpcActionAnimInfo = new TalkNpcActionAnimInfo();
    mTalkNpcActionAnimInfo->setWaitActionName("WaitTrouble");

    al::initNerve(this, &NrvHostType.Wait, 8);

    mNpcStateReaction = NpcStateReaction::create(this, nullptr);
    mNpcStateReaction->setStateReactionParam(new NpcStateReactionParam());
    mNpcStateReaction->setEnableCapReaction(true);

    al::initNerveState(this, mNpcStateReaction, &NrvHostType.Reaction, "リアクション");

    mPlacementId = al::createPlacementId(initInfo);

    mEventFlowExecutor = rs::initEventFlow(this, initInfo, "SessionMayorNpc", "MusicianEvent");
    rs::initEventMovementTurnSeparate(mEventFlowExecutor, initInfo);
    rs::initEventActionNameConverter(mEventFlowExecutor, mTalkNpcActionAnimInfo);
    al::initEventReceiver(mEventFlowExecutor, this);
    rs::initEventCharacterName(mEventFlowExecutor, initInfo, "Mayor");
    rs::initEventCameraTalk(mEventFlowExecutor, initInfo, "Default", -1.0);
    rs::initEventCameraTalk(mEventFlowExecutor, initInfo, "Outdoor", -1.0);

    mTalkNpcParam = rs::initTalkNpcParam(this, nullptr);

    rs::initEventParam(mEventFlowExecutor, mTalkNpcParam, nullptr);

    mTalkNpcActionAnimInfo->init(this, initInfo, mTalkNpcParam, nullptr);

    s32 childNum = al::calcLinkChildNum(initInfo, "SessionMusician");
    mMusicianArray.allocBuffer(childNum, nullptr);

    for (s32 i = 0; i < childNum; i++) {
        SessionMusicianNpc* musician =
            (SessionMusicianNpc*)al::createLinksActorFromFactory(initInfo, "SessionMusician", i);
        mMusicianArray.pushBack(musician);
    }

    startEventFlow(mEventFlowExecutor, this);

    SessionEventProgress progress = GameDataFunction::getSessionEventProgress(this);

    if (SessionEventProgress::Entry < progress)
        al::onStageSwitch(this, "EventStartSwitchOn");

    if (SessionEventProgress::Wait1stMusician < progress)
        al::onStageSwitch(this, "Join1stMusicianSwitchOn");

    if (SessionEventProgress::RequestGoToThePowerPlant < progress) {
        al::tryOnStageSwitch(this, "Join4thMusicianSwitchOn");
        if (SessionEventProgress::TheCeremonyIsReady > progress)
            al::tryOnStageSwitch(this, "SwitchWaitThePowerPlantWorksKeepOn");
    }
    if (SessionEventProgress::WaitThePowerPlantWorks < progress)
        al::onStageSwitch(this, "SwitchCeremonyIsReadyOn");

    al::initJointControllerKeeper(this, 8);
    mNpcJointLookAtController = rs::tryCreateAndAppendNpcJointLookAtController(this, mTalkNpcParam);

    mJointSpringControllerHolder = new al::JointSpringControllerHolder();
    mJointSpringControllerHolder->init(this, "InitJointSpringCtrl");

    SessionMusicianLocalFunction::tryCreateSessionMusicianManager(this);
    SessionMusicianLocalFunction::entrySessionMayorToManager(this);

    mDemoInfo = al::registDemoRequesterToAddDemoInfo(this, initInfo, 0);

    GameDataHolderAccessor accessor(this);
    const char* actionName = getActionName(GameDataFunction::getSessionEventProgress(accessor));
    mTalkNpcActionAnimInfo->setWaitActionName(actionName);

    mRandomWaitActionUpdater =
        new RandomWaitActionUpdater(this, initInfo, mTalkNpcParam, actionName, nullptr);
    mRandomWaitActionUpdater->setRandomOutbreakProbability(0.2);

    al::startVisAnim(this, "HatOn");

    makeActorAlive();
}

void SessionMayorNpc::movement() {
    if (mNpcJointLookAtController)
        rs::updateNpcJointLookAtController(mNpcJointLookAtController);

    al::LiveActor::movement();

    if (!rs::isActiveDemoScenarioCamera(this))
        rs::tryUpdateNpcEyeLineAnim(this, mTalkNpcParam);

    rs::syncActionCityMayorFace(this);
    rs::syncMtsAnimCityMayorFace(this);
    rs::trySwitchDepthToSelfShadow(this);

    if (al::isNerve(this, &NrvHostType.Reaction))
        return;

    mRandomWaitActionUpdater->update();
}

void SessionMayorNpc::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    rs::attackSensorNpcCommon(self, other);
}

bool SessionMayorNpc::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                 al::HitSensor* self) {
    if (rs::tryReceiveMsgPlayerDisregard(message, self, mTalkNpcParam))
        return true;
    if (rs::isMsgPlayerDisregardHomingAttack(message))
        return true;
    if (rs::isMsgPlayerDisregardTargetMarker(message))
        return true;

    if (!al::isNerve(this, &NrvHostType.Reaction) || !al::isNewNerve(this)) {
        bool result;
        if (rs::isInvalidTrampleSensor(self, mTalkNpcParam))
            result = mNpcStateReaction->receiveMsgWithoutTrample(message, other, self);
        else
            result = mNpcStateReaction->receiveMsg(message, other, self);

        if (result) {
            al::setNerve(this, &NrvHostType.Reaction);
            return true;
        }
    }

    return mNpcStateReaction->receiveMsgNoReaction(message, other, self);
}

static inline bool isCameraMusicianType(SessionMusicianType type) {
    return type >= SessionMusicianType::Drum && type <= SessionMusicianType::Trumpet;
}

static inline const char* getCameraName(SessionMusicianType type) {
    if (!isCameraMusicianType(type))
        return nullptr;
    return sCameraNames[type];
}

static inline sead::Vector3f* getCameraOffset(SessionMusicianType type, SessionMayorParam* param) {
    if (!isCameraMusicianType(type))
        return nullptr;
    sead::Vector3f* values[] = {
        &param->drummerCameraParams.offset, &param->bassistCameraParams.offset,
        &param->guitaristCameraParams.offset, &param->trumpeterCameraParams.offset};
    return values[type];
}

static inline f32 getCameraDistance(SessionMusicianType type, SessionMayorParam* param) {
    if (!isCameraMusicianType(type))
        return -1.0f;
    f32 values[] = {
        param->drummerCameraParams.distance->value, param->bassistCameraParams.distance->value,
        param->guitaristCameraParams.distance->value, param->trumpeterCameraParams.distance->value};
    return values[type];
}

static inline f32 getCameraHAngle(SessionMusicianType type, SessionMayorParam* param) {
    if (!isCameraMusicianType(type))
        return 45.0f;
    f32 values[] = {param->drummerCameraParams.horizontalAngle->value,
                    param->bassistCameraParams.horizontalAngle->value,
                    param->guitaristCameraParams.horizontalAngle->value,
                    param->trumpeterCameraParams.horizontalAngle->value};
    return al::wrapAngle(values[type]);
}

static inline f32 getCameraVAngle(SessionMusicianType type, SessionMayorParam* param) {
    if (!isCameraMusicianType(type))
        return 10.0f;
    f32 values[] = {param->drummerCameraParams.verticalAngle->value,
                    param->bassistCameraParams.verticalAngle->value,
                    param->guitaristCameraParams.verticalAngle->value,
                    param->trumpeterCameraParams.verticalAngle->value};
    return values[type];
}

void SessionMayorNpc::initIntroductionCamera(const al::ActorInitInfo& initInfo,
                                             sead::PtrArray<SessionMusicianNpc>* musicians) {
    for (auto it = musicians->begin(); it != musicians->end(); ++it) {
        SessionMusicianNpc* musician = &(*it);
        al::EventFlowExecutor* evenFlow = mEventFlowExecutor;
        SessionMayorParam* mayorParam = mMayorParam;
        SessionMusicianType type = SessionMusicianLocalFunction::getMusicianType(musician);
        const char* cameraName = getCameraName(type);
        if (cameraName && rs::isDefinedEventCamera(evenFlow, cameraName))
            rs::initEventCameraFixActor2(
                evenFlow, initInfo, cameraName, musician, getCameraOffset(type, mayorParam),
                getCameraDistance(type, mayorParam), getCameraHAngle(type, mayorParam),
                getCameraVAngle(type, mayorParam), true);
    }
}

bool SessionMayorNpc::tryAppearMemberMusicians() {
    if (mMusicianArray.size() == 0)
        return false;

    bool appeared = false;
    for (auto it = mMusicianArray.begin(); it != mMusicianArray.end(); ++it) {
        if (!al::isAlive(&(*it))) {
            SessionMusicianType musicianType =
                SessionMusicianLocalFunction::getMusicianType(&(*it));
            if (SessionMusicianLocalFunction::isSubscribed(this, musicianType)) {
                (&(*it))->appear();
                appeared = true;
            }
        }
    }

    if (appeared) {
        const char* progress = (GameDataFunction::getSessionEventProgress(this).text());
        rs::startEventFlow(mEventFlowExecutor, progress);
        if (al::isEqualString(progress, "Wait2ndMusician"))
            al::tryOnStageSwitch(this, "Join1stMusicianSwitchOn");

        return true;
    }

    return false;
}

bool SessionMayorNpc::receiveEvent(const al::EventFlowEventData* event) {
    if (al::isEventName(event, "StartCutSceneDemo") && rs::tryStartEventCutSceneDemo(this)) {
        rs::addDemoActor(SessionMusicianLocalFunction::getSessionMusicianManager(this), false);
        al::addDemoActorFromAddDemoInfo(this, mDemoInfo);
        return true;
    }

    if (al::isEventName(event, "EndCutSceneDemo")) {
        rs::endEventCutSceneDemo(this);
        return true;
    }

    if (al::isEventName(event, "StartIntroductionDemo")) {
        SessionMusicianLocalFunction::addDemoAllMusicians(this);
        return true;
    }

    if (al::isEventName(event, "ResetJointControl")) {
        mJointSpringControllerHolder->resetControlAll();

        return true;
    }

    SessionEventProgress progress = getProgress(al::getEventName(event));

    if (progress) {
        GameDataFunction::setSessionEventProgress(this, progress);

        mTalkNpcActionAnimInfo->setWaitActionName(getActionName(progress));

        if (progress == SessionEventProgress::WaitThePowerPlantWorks)
            al::tryOnStageSwitch(this, "Join4thMusicianSwitchOn");

        return true;
    }

    return false;
}

void SessionMayorNpc::exeWait() {
    tryAppearMemberMusicians();
    rs::updateEventFlow(mEventFlowExecutor);
    if (rs::isActiveEventDemo(this) && rs::isEqualEventDemoStartActor(this)) {
        al::invalidateClipping(this);
        return;
    }

    al::validateClipping(this);
}

void SessionMayorNpc::exeReaction() {
    if (rs::checkEnableStartEventAndCancelReaction(this, mTalkNpcParam)) {
        rs::updateEventFlow(mEventFlowExecutor);

        if (!al::isActionOneTime(this)) {
            al::setNerve(this, &NrvHostType.Wait);
            return;
        }
    }

    if (al::updateNerveStateAndNextNerve(this, &NrvHostType.Wait)) {
        GameDataHolderAccessor accessor(this);

        const char* animName = GameDataFunction::getSessionEventProgress(accessor) >
                                       SessionEventProgress::WaitThePowerPlantWorks ?
                                   "WaitSmile" :
                                   "WaitTrouble";

        al::startAction(this, animName);
    }
}
