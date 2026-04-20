#include "Event/EventDemoCtrl.h"

#include "Util/DemoUtil.h"
#include "Util/NpcEventFlowUtil.h"

namespace {
enum EventDemoType {
    EventDemoType_None = 0,
    EventDemoType_Normal = 1,
    EventDemoType_Talk = 2,
    EventDemoType_TalkOnlyRequester = 3,
    EventDemoType_TalkKeepHack = 4,
    EventDemoType_TalkUseCoin = 5,
    EventDemoType_KeepBind = 6,
    EventDemoType_CutScene = 7,
    EventDemoType_CutSceneKeepHack = 8,
    EventDemoType_CutSceneTalkOnlyRequester = 9,
};

void endEventDemo(EventDemoCtrlInfo* info) {
    switch (info->demoType) {
    case EventDemoType_TalkOnlyRequester:
    case EventDemoType_CutSceneTalkOnlyRequester:
        rs::requestEndDemoNormalTalk(info->demoStartActor);
        break;

    case EventDemoType_TalkKeepHack:
    case EventDemoType_CutSceneKeepHack:
        rs::requestEndDemoKeepHackTalk(info->demoStartActor);
        break;

    case EventDemoType_Normal:
        rs::requestEndDemoNormal(info->demoStartActor);
        break;

    case EventDemoType_Talk:
    default:
        rs::requestEndDemoWithPlayerKeepCarryTalk(info->demoStartActor);
        break;

    case EventDemoType_TalkUseCoin:
        rs::requestEndDemoWithPlayerUseCoinTalk(info->demoStartActor);
        break;

    case EventDemoType_KeepBind:
        rs::requestEndDemoWithPlayerKeepBindTalk(info->demoStartActor);
        break;

    case EventDemoType_CutScene:
        rs::requestEndDemoWithPlayerCinemaFrameTalk(info->demoStartActor);
        break;
    }

    info->demoType = 0;
    info->demoStartActor = nullptr;
    info->isRequestEndDemo = 0;
}
}  // namespace

EventDemoCtrl::EventDemoCtrl() {
    EventDemoCtrlInfo* info = new EventDemoCtrlInfo;

    info->demoType = 0;
    info->isRequestEndDemo = 0;
    info->isDemoSkipStart = 0;
    info->demoStartActor = nullptr;
    info->lockTalkActor = nullptr;
    mEventDemoInfo = info;
}

bool EventDemoCtrl::isSuccessLockTalkDemo(const al::LiveActor* actor) const {
    return mEventDemoInfo->lockTalkActor == actor;
}

bool EventDemoCtrl::tryLockStartTalkDemo(al::LiveActor* actor) {
    EventDemoCtrlInfo* info = mEventDemoInfo;

    if (!rs::isSuccessNpcEventBalloonMessage(actor))
        return false;

    if (rs::isActiveDemo(actor))
        return false;

    if (info->demoType != 0)
        return false;

    info->lockTalkActor = actor;
    return true;
}

bool EventDemoCtrl::tryLockStartTalkDemoWithoutBalloon(al::LiveActor* actor) {
    EventDemoCtrlInfo* info = mEventDemoInfo;

    if (rs::isActiveDemo(actor))
        return false;

    if (info->demoType != 0)
        return false;

    info->lockTalkActor = actor;
    return true;
}

bool EventDemoCtrl::tryStartTalkDemo(al::LiveActor* actor) {
    EventDemoCtrlInfo* info = mEventDemoInfo;

    if (!rs::requestStartDemoWithPlayerKeepCarryTalk(actor, true))
        return false;

    info->demoType = EventDemoType_Talk;
    info->demoStartActor = actor;
    info->lockTalkActor = nullptr;
    info->isRequestEndDemo = 0;
    info->isDemoSkipStart = 0;
    return true;
}

bool EventDemoCtrl::tryStartTalkOnlyRequesterDemo(al::LiveActor* actor) {
    EventDemoCtrlInfo* info = mEventDemoInfo;

    if (!rs::requestStartDemoNormalTalk(actor, true))
        return false;

    info->demoType = EventDemoType_TalkOnlyRequester;
    info->demoStartActor = actor;
    info->lockTalkActor = nullptr;
    info->isRequestEndDemo = 0;
    info->isDemoSkipStart = 0;
    return true;
}

bool EventDemoCtrl::tryStartTalkKeepHackDemo(al::LiveActor* actor) {
    EventDemoCtrlInfo* info = mEventDemoInfo;

    if (!rs::requestStartDemoKeepHackTalk(actor, true))
        return false;

    info->demoType = EventDemoType_TalkKeepHack;
    info->demoStartActor = actor;
    info->lockTalkActor = nullptr;
    info->isRequestEndDemo = 0;
    info->isDemoSkipStart = 0;
    return true;
}

bool EventDemoCtrl::tryStartTalkUseCoinDemo(al::LiveActor* actor) {
    EventDemoCtrlInfo* info = mEventDemoInfo;

    if (!rs::requestStartDemoWithPlayerUseCoinTalk(actor, true))
        return false;

    info->demoType = EventDemoType_TalkUseCoin;
    info->demoStartActor = actor;
    info->lockTalkActor = nullptr;
    info->isRequestEndDemo = 0;
    info->isDemoSkipStart = 0;
    return true;
}

bool EventDemoCtrl::tryStartNormalDemo(al::LiveActor* actor) {
    EventDemoCtrlInfo* info = mEventDemoInfo;

    if (info->lockTalkActor != nullptr)
        return false;

    if (rs::isActiveDemo(actor))
        return false;

    if (info->demoType != 0)
        return false;

    bool isSuccess = true;
    if (!rs::requestStartDemoNormal(actor, true)) {
        isSuccess = false;
    } else {
        info->demoType = EventDemoType_Normal;
        info->demoStartActor = actor;
        info->lockTalkActor = nullptr;
        info->isRequestEndDemo = 0;
        info->isDemoSkipStart = 0;
    }

    return isSuccess;
}

bool EventDemoCtrl::tryStartKeepBindDemo(al::LiveActor* actor) {
    EventDemoCtrlInfo* info = mEventDemoInfo;

    if (info->lockTalkActor != nullptr)
        return false;

    if (rs::isActiveDemo(actor))
        return false;

    if (info->demoType != 0)
        return false;

    if (!rs::requestStartDemoWithPlayerKeepBindTalk(actor, true))
        return false;

    info->demoType = EventDemoType_KeepBind;
    info->demoStartActor = actor;
    info->lockTalkActor = nullptr;
    info->isRequestEndDemo = 0;
    info->isDemoSkipStart = 0;
    return true;
}

bool EventDemoCtrl::tryStartCutSceneDemo(al::LiveActor* actor) {
    EventDemoCtrlInfo* info = mEventDemoInfo;

    if (info->lockTalkActor != nullptr)
        return false;

    if (rs::isActiveDemo(actor))
        return false;

    if (info->demoType != 0)
        return false;

    if (!rs::requestStartDemoWithPlayerCinemaFrameTalk(actor, true))
        return false;

    info->demoType = EventDemoType_CutScene;
    info->demoStartActor = actor;
    info->lockTalkActor = nullptr;
    info->isRequestEndDemo = 0;
    info->isDemoSkipStart = 0;
    return true;
}

bool EventDemoCtrl::tryStartCutSceneKeepHackDemo(al::LiveActor* actor) {
    EventDemoCtrlInfo* info = mEventDemoInfo;

    if (info->lockTalkActor != nullptr)
        return false;

    if (rs::isActiveDemo(actor))
        return false;

    if (info->demoType != 0)
        return false;

    if (!rs::requestStartDemoKeepHackTalk(actor, true))
        return false;

    info->demoType = EventDemoType_CutSceneKeepHack;
    info->demoStartActor = actor;
    info->lockTalkActor = nullptr;
    info->isRequestEndDemo = 0;
    info->isDemoSkipStart = 0;
    return true;
}

bool EventDemoCtrl::tryStartCutSceneTalkOnlyRequesterDemo(al::LiveActor* actor) {
    EventDemoCtrlInfo* info = mEventDemoInfo;

    if (info->lockTalkActor != nullptr)
        return false;

    if (rs::isActiveDemo(actor))
        return false;

    if (info->demoType != 0)
        return false;

    if (!rs::requestStartDemoNormalTalk(actor, true))
        return false;

    info->demoType = EventDemoType_CutSceneTalkOnlyRequester;
    info->demoStartActor = actor;
    info->lockTalkActor = nullptr;
    info->isRequestEndDemo = 0;
    info->isDemoSkipStart = 0;
    return true;
}

void EventDemoCtrl::requestEndDemo(al::LiveActor* actor) {
    mEventDemoInfo->isRequestEndDemo = 1;
}

void EventDemoCtrl::endCutSceneDemo(al::LiveActor* actor) {
    mEventDemoInfo->isRequestEndDemo = 1;
}

void EventDemoCtrl::endCutSceneTalkOnlyRequesterDemo(al::LiveActor* actor) {
    mEventDemoInfo->isRequestEndDemo = 1;
}

void EventDemoCtrl::endCutSceneDemoBySkip(al::LiveActor* actor) {
    EventDemoCtrlInfo* info = mEventDemoInfo;
    endEventDemo(info);
    info->isDemoSkipStart = 0;
}

bool EventDemoCtrl::isActiveDemo() const {
    return mEventDemoInfo->demoType != 0;
}

bool EventDemoCtrl::isActiveDemoWithPlayer() const {
    return mEventDemoInfo->demoType == EventDemoType_Talk;
}

bool EventDemoCtrl::isRequestEndDemo() const {
    return mEventDemoInfo->isRequestEndDemo != 0;
}

al::LiveActor* EventDemoCtrl::getDemoStartActor() const {
    return mEventDemoInfo->demoStartActor;
}

void EventDemoCtrl::endDemo() {
    endEventDemo(mEventDemoInfo);
}

bool EventDemoCtrl::isDemoStartActor(const al::LiveActor* actor) const {
    return mEventDemoInfo->demoStartActor == actor;
}

void EventDemoCtrl::notifyStartDemoSkipFromScene() {
    EventDemoCtrlInfo* info = mEventDemoInfo;

    if (info->demoStartActor != nullptr)
        info->isDemoSkipStart = 1;
}

bool EventDemoCtrl::isDemoSkipStart() const {
    return mEventDemoInfo->isDemoSkipStart != 0;
}
