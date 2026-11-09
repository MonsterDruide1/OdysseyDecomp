#include "Npc/HideAndSeekCapMan.h"

#include "Library/Camera/CameraUtil.h"
#include "Library/Event/EventFlowExecutor.h"
#include "Library/Event/EventFlowUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolderAccessor.h"
#include "Util/NpcEventFlowUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(HideAndSeekCapMan, Wait);
NERVE_IMPL(HideAndSeekCapMan, Scared);
NERVE_IMPL(HideAndSeekCapMan, Disappear);

NERVES_MAKE_STRUCT(HideAndSeekCapMan, Wait, Scared, Disappear);

NERVE_IMPL(HideAndSeekCapMan, Hide);
NERVE_IMPL(HideAndSeekCapMan, Appear);

NERVES_MAKE_NOSTRUCT(HideAndSeekCapMan, Hide, Appear);

const sead::Vector3f sBalloonLocalOffset = {0.0f, 15.0f, 0.0f};
}  // namespace

HideAndSeekCapMan::HideAndSeekCapMan(const char* name) : al::PartsModel(name) {}

HideAndSeekCapMan* HideAndSeekCapMan::tryCreateLinkNpcIfClearGame(al::LiveActor* actor,
                                                                   const al::ActorInitInfo& info,
                                                                   const char* arcSuffix,
                                                                   const char* suffix) {
    if (!al::isExistLinkChild(info, "HideAndSeekCapMan", 0)) {
        return nullptr;
    }
    HideAndSeekCapMan* capMan =
        new HideAndSeekCapMan(al::getLinksActorDisplayName(info, "HideAndSeekCapMan", 0));
    capMan->initPartsFixFile(actor, info, "CapManEyes", arcSuffix, suffix);
    al::initLinksActor(capMan, info, "HideAndSeekCapMan", 0);
    return capMan;
}

void HideAndSeekCapMan::init(const al::ActorInitInfo& info) {
    al::initNerve(this, &Hide, 0);
    mEventFlowExecutor = rs::initEventFlowFromPlacementInfo(this, info, nullptr);
    if (rs::isDefinedEventCamera(mEventFlowExecutor, "Default")) {
        rs::initEventCameraObject(mEventFlowExecutor, info, "Default");
    }
    al::setBalloonLocalOffset(mEventFlowExecutor, sBalloonLocalOffset);
    rs::startEventFlow(mEventFlowExecutor, "Init");
    _150 = GameDataFunction::tryFindLinkedShineIndex(this, info);
}

void HideAndSeekCapMan::control() {
    GameDataFunction::setHintTrans(GameDataHolderAccessor{this}, _150, al::getTrans(this));
}

void HideAndSeekCapMan::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isNerve(this, &NrvHideAndSeekCapMan.Wait) ||
        al::isNerve(this, &NrvHideAndSeekCapMan.Scared)) {
        rs::sendMsgEventFlowScareCheck(other, self, mEventFlowExecutor);
    }
}

bool HideAndSeekCapMan::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                   al::HitSensor* self) {
    return rs::isMsgPlayerDisregardHomingAttack(message);
}

void HideAndSeekCapMan::exeAppear() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Appear");
    }
    if (al::isActionEnd(this)) {
        al::validateHitSensors(this);
        al::setNerve(this, &NrvHideAndSeekCapMan.Wait);
    }
}

void HideAndSeekCapMan::exeWait() {
    if (al::isFirstStep(this)) {
        rs::startEventFlow(mEventFlowExecutor, "Init");
    }
    rs::updateEventFlow(mEventFlowExecutor);
    if (al::isScare(mEventFlowExecutor)) {
        al::setNerve(this, &NrvHideAndSeekCapMan.Scared);
        return;
    }
    if (rs::isActiveEventDemo(this)) {
        return;
    }
    sead::Vector3f frontDir(0.0f, 0.0f, 0.0f);
    sead::Vector3f cameraDir(0.0f, 0.0f, 0.0f);
    al::calcFrontDir(&frontDir, this);
    al::calcCameraDir(&cameraDir, this, 0);
    if (cameraDir.dot(frontDir) >= 0.0f) {
        _154 = 30;
    } else if (--_154 <= 0) {
        al::setNerve(this, &NrvHideAndSeekCapMan.Disappear);
    }
}

void HideAndSeekCapMan::exeDisappear() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Disappear");
        al::invalidateHitSensors(this);
    }
    al::setNerveAtActionEnd(this, &Hide);
}

void HideAndSeekCapMan::exeHide() {
    sead::Vector3f frontDir(0.0f, 0.0f, 0.0f);
    sead::Vector3f cameraDir(0.0f, 0.0f, 0.0f);
    al::calcFrontDir(&frontDir, this);
    al::calcCameraDir(&cameraDir, this, 0);
    if (!(cameraDir.dot(frontDir) < 0.0f)) {
        if (_154++ >= 29) {
            al::setNerve(this, &Appear);
        }
    } else {
        _154 = 0;
    }
}

void HideAndSeekCapMan::exeScared() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Scared");
    }
    rs::updateEventFlow(mEventFlowExecutor);
    if (al::isScare(mEventFlowExecutor)) {
        return;
    }
    al::setNerve(this, &NrvHideAndSeekCapMan.Wait);
}
