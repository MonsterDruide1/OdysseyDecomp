#include "Npc/TalkNpc.h"

#include "Audio/BgmAnimeSynchronizer.h"
#include "Library/Area/AreaObjUtil.h"
#include "Library/Clipping/ClippingActorInfo.h"
#include "Library/Event/EventFlowUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Npc/TalkNpcStateEvent.h"
#include "System/GameDataUtil.h"
#include "Util/NpcAnimUtil.h"
#include "Util/NpcEventFlowUtil.h"
#include "Util/PlayerUtil.h"

namespace {
NERVE_IMPL(TalkNpc, Event);
NERVE_IMPL(TalkNpc, KinopioBrigadeFirstEvent);
NERVE_IMPL(TalkNpc, HackEvent);
NERVE_IMPL(TalkNpc, DemoKoopa);

NERVES_MAKE_STRUCT(TalkNpc, Event, KinopioBrigadeFirstEvent, HackEvent);
NERVES_MAKE_NOSTRUCT(TalkNpc, DemoKoopa);
}  // namespace

void TalkNpc::kill() {
    al::LiveActor::kill();
    al::tryOnSwitchDeadOn(this);
}

void TalkNpc::endClipped() {
    al::LiveActor::endClipped();
    if (_218) {
        _218->_28 = true;
    }
}

void TalkNpc::onFukankunCameraNearClipSetting() {
    _1a0->mClippingActorInfo->setFarClipLength(300.0f);
}

void TalkNpc::offFukankunCameraNearClipSetting() {
    _1a0->mClippingActorInfo->setFarClipLength(_1a0->mSavedNearClip);
}

TalkNpcStateEvent* TalkNpc::tryGetCurrentEventState() const {
    if (al::isNerve(this, &NrvTalkNpc.Event)) {
        return _130;
    }
    if (al::isNerve(this, &NrvTalkNpc.HackEvent)) {
        return _138;
    }
    if (al::isNerve(this, &NrvTalkNpc.KinopioBrigadeFirstEvent)) {
        return _140;
    }
    return nullptr;
}

al::IUseEventFlowData* TalkNpc::tryGetCurrentEvent() const {
    TalkNpcStateEvent* eventState = nullptr;
    if (al::isNerve(this, &NrvTalkNpc.Event)) {
        eventState = _130;
    } else if (al::isNerve(this, &NrvTalkNpc.HackEvent)) {
        eventState = _138;
    } else if (al::isNerve(this, &NrvTalkNpc.KinopioBrigadeFirstEvent)) {
        eventState = _140;
    }
    if (eventState) {
        return eventState->mEventFlowData;
    }
    return nullptr;
}

void TalkNpc::startDemoKoopa(const sead::Vector3f& front) {
    sead::Vector3f dir = front - al::getTrans(this);
    al::verticalizeVec(&dir, al::getGravity(this), dir);
    if (!al::tryNormalizeOrZero(&dir)) {
        // NOTE: plain assignment emits three 32-bit copies here; the
        // original is a single 8-byte + 4-byte copy (memcpy lowering).
        __builtin_memcpy(&dir, &al::getFront(this), sizeof(dir));
    }
    al::setFront(this, dir);
    al::setNerve(this, &DemoKoopa);
}

void TalkNpc::endDemoKoopa() {
    al::setFront(this, _1d4);
    al::setNerve(this, &NrvTalkNpc.Event);
}

void TalkNpc::exeDemoKoopa() {
    if (al::isFirstStep(this)) {
        rs::startNpcAction(this, "Wait");
    }
}

void TalkNpc::exeKinopioBrigadeFirstEvent() {
    rs::tryConnectToCollisionTFSV(this, _1b0, &_1b8);
    if (al::updateNerveState(this)) {
        rs::onFlagKinopioBrigadeNpcFirstTalk(this);
        al::IUseEventFlowData* eventFlowData = _130->mEventFlowData;
        al::recordActorFront(eventFlowData,
                             al::getRecordActorFront(_140->mEventFlowData));
        al::setNerve(this, &NrvTalkNpc.Event);
    }
}

void TalkNpc::exeHackEvent() {
    rs::tryConnectToCollisionTFSV(this, _1b0, &_1b8);
    if (rs::isActiveEventDemo(this)) {
        al::updateNerveState(this);
        return;
    }
    TalkNpcStateEvent* eventState = _138;
    if (!eventState) {
        al::setNerve(this, &NrvTalkNpc.Event);
        return;
    }
    const al::AreaObjGroup* areaObjGroup = _228;
    if (!rs::isPlayerHackType(this, _154)) {
        al::setNerve(this, &NrvTalkNpc.Event);
        return;
    }
    if (areaObjGroup && al::isInAreaObj(areaObjGroup, rs::getPlayerPos(this))) {
        al::updateNerveState(this);
        return;
    }
    if (al::checkInsideTerritoryPos(eventState->mEventFlowData, this,
                                     rs::getPlayerPos(this), 0.0f)) {
        al::updateNerveState(this);
        return;
    }
    al::setNerve(this, &NrvTalkNpc.Event);
}

void TalkNpc::exeEvent() {
    rs::tryConnectToCollisionTFSV(this, _1b0, &_1b8);
    TalkNpcStateEvent* hackEventState = _138;
    s32 hackType = _154;
    bool isActiveDemo = rs::isActiveEventDemo(this);
    if (hackEventState && !isActiveDemo && rs::isPlayerHackType(this, hackType)) {
        al::IUseEventFlowData* eventFlowData = hackEventState->mEventFlowData;
        if (al::checkInsideTerritoryPos(eventFlowData, this, rs::getPlayerPos(this),
                                         0.0f)) {
            al::setNerve(this, &NrvTalkNpc.HackEvent);
            return;
        }
    }
    al::updateNerveState(this);
    const char* volleyBallEntryName = rs::tryGetTalkNpcVolleyBallEntryName(this);
    const char* jumpingRopeEntryName = rs::tryGetTalkNpcJumpingRopeEntryName(this);
    const char* radiconEntryName = rs::tryGetTalkNpcRadiconEntryName(this);
    if (volleyBallEntryName) {
        _130->trySwitchEventEntry(volleyBallEntryName);
    }
    if (jumpingRopeEntryName) {
        _130->trySwitchEventEntry(jumpingRopeEntryName);
    }
    if (radiconEntryName) {
        _130->trySwitchEventEntry(radiconEntryName);
    }
    if (_218) {
        _218->trySyncBgm();
    }
}
