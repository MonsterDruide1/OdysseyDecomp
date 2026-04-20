#include "Event/EventDemoCtrl.h"

#include "Util/DemoUtil.h"
#include "Util/NpcEventFlowUtil.h"

void endEventDemo(EventDemoCtrlInfo* info) {
    switch (info->demoType) {
    case EventDemoType::TalkOnlyRequester:
    case EventDemoType::CutSceneTalkOnlyRequester:
        rs::requestEndDemoNormalTalk(info->demoStartActor);
        break;

    case EventDemoType::TalkKeepHack:
    case EventDemoType::CutSceneKeepHack:
        rs::requestEndDemoKeepHackTalk(info->demoStartActor);
        break;

    case EventDemoType::Normal:
        rs::requestEndDemoNormal(info->demoStartActor);
        break;

    case EventDemoType::Talk:
    default:
        rs::requestEndDemoWithPlayerKeepCarryTalk(info->demoStartActor);
        break;

    case EventDemoType::TalkUseCoin:
        rs::requestEndDemoWithPlayerUseCoinTalk(info->demoStartActor);
        break;

    case EventDemoType::KeepBind:
        rs::requestEndDemoWithPlayerKeepBindTalk(info->demoStartActor);
        break;

    case EventDemoType::CutScene:
        rs::requestEndDemoWithPlayerCinemaFrameTalk(info->demoStartActor);
        break;
    }

    info->demoType = EventDemoType::None;
    info->demoStartActor = nullptr;
    info->isRequestEndDemo = false;
}

EventDemoCtrl::EventDemoCtrl() {
    mEventDemoInfo = new EventDemoCtrlInfo;
}

bool EventDemoCtrl::isSuccessLockTalkDemo(const al::LiveActor* actor) {
    return mEventDemoInfo->lockTalkActor == actor;
}

bool EventDemoCtrl::tryLockStartTalkDemo(al::LiveActor* actor) {
    EventDemoCtrlInfo* info = mEventDemoInfo;

    if (!rs::isSuccessNpcEventBalloonMessage(actor))
        return false;

    if (rs::isActiveDemo(actor))
        return false;

    if (info->demoType != EventDemoType::None)
        return false;

    info->lockTalkActor = actor;
    return true;
}

bool EventDemoCtrl::tryLockStartTalkDemoWithoutBalloon(al::LiveActor* actor) {
    EventDemoCtrlInfo* info = mEventDemoInfo;

    if (rs::isActiveDemo(actor))
        return false;

    if (info->demoType != EventDemoType::None)
        return false;

    info->lockTalkActor = actor;
    return true;
}

bool EventDemoCtrl::tryStartTalkDemo(al::LiveActor* actor) {
    EventDemoCtrlInfo* info = mEventDemoInfo;

    if (!rs::requestStartDemoWithPlayerKeepCarryTalk(actor, true))
        return false;

    info->demoType = EventDemoType::Talk;
    info->demoStartActor = actor;
    info->lockTalkActor = nullptr;
    info->isRequestEndDemo = false;
    info->isDemoSkipStart = false;
    return true;
}

bool EventDemoCtrl::tryStartTalkOnlyRequesterDemo(al::LiveActor* actor) {
    EventDemoCtrlInfo* info = mEventDemoInfo;

    if (!rs::requestStartDemoNormalTalk(actor, true))
        return false;

    info->demoType = EventDemoType::TalkOnlyRequester;
    info->demoStartActor = actor;
    info->lockTalkActor = nullptr;
    info->isRequestEndDemo = false;
    info->isDemoSkipStart = false;
    return true;
}

bool EventDemoCtrl::tryStartTalkKeepHackDemo(al::LiveActor* actor) {
    EventDemoCtrlInfo* info = mEventDemoInfo;

    if (!rs::requestStartDemoKeepHackTalk(actor, true))
        return false;

    info->demoType = EventDemoType::TalkKeepHack;
    info->demoStartActor = actor;
    info->lockTalkActor = nullptr;
    info->isRequestEndDemo = false;
    info->isDemoSkipStart = false;
    return true;
}

bool EventDemoCtrl::tryStartTalkUseCoinDemo(al::LiveActor* actor) {
    EventDemoCtrlInfo* info = mEventDemoInfo;

    if (!rs::requestStartDemoWithPlayerUseCoinTalk(actor, true))
        return false;

    info->demoType = EventDemoType::TalkUseCoin;
    info->demoStartActor = actor;
    info->lockTalkActor = nullptr;
    info->isRequestEndDemo = false;
    info->isDemoSkipStart = false;
    return true;
}

bool EventDemoCtrl::tryStartNormalDemo(al::LiveActor* actor) {
    EventDemoCtrlInfo* info = mEventDemoInfo;

    if (info->lockTalkActor != nullptr)
        return false;

    if (rs::isActiveDemo(actor))
        return false;

    if (info->demoType != EventDemoType::None)
        return false;

    bool isSuccess = true;
    if (!rs::requestStartDemoNormal(actor, true)) {
        isSuccess = false;
    } else {
        info->demoType = EventDemoType::Normal;
        info->demoStartActor = actor;
        info->lockTalkActor = nullptr;
        info->isRequestEndDemo = false;
        info->isDemoSkipStart = false;
    }

    return isSuccess;
}

bool EventDemoCtrl::tryStartKeepBindDemo(al::LiveActor* actor) {
    EventDemoCtrlInfo* info = mEventDemoInfo;

    if (info->lockTalkActor != nullptr)
        return false;

    if (rs::isActiveDemo(actor))
        return false;

    if (info->demoType != EventDemoType::None)
        return false;

    if (!rs::requestStartDemoWithPlayerKeepBindTalk(actor, true))
        return false;

    info->demoType = EventDemoType::KeepBind;
    info->demoStartActor = actor;
    info->lockTalkActor = nullptr;
    info->isRequestEndDemo = false;
    info->isDemoSkipStart = false;
    return true;
}

bool EventDemoCtrl::tryStartCutSceneDemo(al::LiveActor* actor) {
    EventDemoCtrlInfo* info = mEventDemoInfo;

    if (info->lockTalkActor != nullptr)
        return false;

    if (rs::isActiveDemo(actor))
        return false;

    if (info->demoType != EventDemoType::None)
        return false;

    if (!rs::requestStartDemoWithPlayerCinemaFrameTalk(actor, true))
        return false;

    info->demoType = EventDemoType::CutScene;
    info->demoStartActor = actor;
    info->lockTalkActor = nullptr;
    info->isRequestEndDemo = false;
    info->isDemoSkipStart = false;
    return true;
}

bool EventDemoCtrl::tryStartCutSceneKeepHackDemo(al::LiveActor* actor) {
    EventDemoCtrlInfo* info = mEventDemoInfo;

    if (info->lockTalkActor != nullptr)
        return false;

    if (rs::isActiveDemo(actor))
        return false;

    if (info->demoType != EventDemoType::None)
        return false;

    if (!rs::requestStartDemoKeepHackTalk(actor, true))
        return false;

    info->demoType = EventDemoType::CutSceneKeepHack;
    info->demoStartActor = actor;
    info->lockTalkActor = nullptr;
    info->isRequestEndDemo = false;
    info->isDemoSkipStart = false;
    return true;
}

bool EventDemoCtrl::tryStartCutSceneTalkOnlyRequesterDemo(al::LiveActor* actor) {
    EventDemoCtrlInfo* info = mEventDemoInfo;

    if (info->lockTalkActor != nullptr)
        return false;

    if (rs::isActiveDemo(actor))
        return false;

    if (info->demoType != EventDemoType::None)
        return false;

    if (!rs::requestStartDemoNormalTalk(actor, true))
        return false;

    info->demoType = EventDemoType::CutSceneTalkOnlyRequester;
    info->demoStartActor = actor;
    info->lockTalkActor = nullptr;
    info->isRequestEndDemo = false;
    info->isDemoSkipStart = false;
    return true;
}

void EventDemoCtrl::requestEndDemo(al::LiveActor* actor) {
    mEventDemoInfo->isRequestEndDemo = true;
}

void EventDemoCtrl::endCutSceneDemo(al::LiveActor* actor) {
    mEventDemoInfo->isRequestEndDemo = true;
}

void EventDemoCtrl::endCutSceneTalkOnlyRequesterDemo(al::LiveActor* actor) {
    mEventDemoInfo->isRequestEndDemo = true;
}

void EventDemoCtrl::endCutSceneDemoBySkip(al::LiveActor* actor) {
    EventDemoCtrlInfo* info = mEventDemoInfo;
    endEventDemo(info);
    info->isDemoSkipStart = false;
}

bool EventDemoCtrl::isActiveDemo() const {
    return mEventDemoInfo->demoType != EventDemoType::None;
}

bool EventDemoCtrl::isActiveDemoWithPlayer() const {
    return mEventDemoInfo->demoType == EventDemoType::Talk;
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
    if (mEventDemoInfo->demoStartActor != nullptr)
        mEventDemoInfo->isDemoSkipStart = true;
}

bool EventDemoCtrl::isDemoSkipStart() const {
    return mEventDemoInfo->isDemoSkipStart != 0;
}
