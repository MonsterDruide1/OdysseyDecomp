#include "MapObj/DoorAreaChange.h"

#include <prim/seadSafeString.h>

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Player/PlayerUtil.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

#include "MapObj/CapMessageShowInfo.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolderAccessor.h"
#include "System/GameDataUtil.h"
#include "System/SaveObjInfo.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(DoorAreaChange, CloseWait);
NERVE_IMPL(DoorAreaChange, NoStartWithMessage);
NERVE_IMPL(DoorAreaChange, OpenWait);
NERVE_IMPL(DoorAreaChange, NoStart);
NERVE_IMPL(DoorAreaChange, Open);
NERVE_IMPL(DoorAreaChange, CloseBefore);
NERVE_IMPL(DoorAreaChange, Close);

NERVES_MAKE_NOSTRUCT(DoorAreaChange, Close);
NERVES_MAKE_STRUCT(DoorAreaChange, CloseWait, NoStartWithMessage, OpenWait, NoStart, Open,
                   CloseBefore);
}  // namespace

DoorAreaChange::DoorAreaChange(const char* name) : al::LiveActor(name) {}

void DoorAreaChange::init(const al::ActorInitInfo& info) {
    using DoorAreaChangeFunctor = al::FunctorV0M<DoorAreaChange*, void (DoorAreaChange::*)()>;

    if (al::isObjectName(info, "ShineTowerRocket"))
        al::initActorWithArchiveName(this, info, "DoorAreaChange", nullptr);
    else
        al::initActorWithArchiveName(this, info, "ShineTowerDoor", nullptr);

    al::initNerve(this, &NrvDoorAreaChange.CloseWait, 0);
    mSaveObjInfo = rs::createSaveObjInfoWriteSaveData(info);
    al::listenStageSwitchOn(this, "SwitchCloseAgain",
                            DoorAreaChangeFunctor(this, &DoorAreaChange::switchCloseAgain));

    if (rs::isInvalidChangeStage(this)) {
        al::setNerve(this, &NrvDoorAreaChange.NoStartWithMessage);
        makeActorAlive();
        return;
    }

    if (GameDataFunction::isPlayerStartLinkedObj(this, info, "NoDelete_DeadByPlayerStart")) {
        al::setNerve(this, &NrvDoorAreaChange.OpenWait);
        makeActorAlive();
        return;
    }
    if (al::listenStageSwitchOnAppear(this, DoorAreaChangeFunctor(this, &DoorAreaChange::appear))) {
        makeActorDead();
        return;
    }
    if (al::listenStageSwitchOnStart(this, DoorAreaChangeFunctor(this, &DoorAreaChange::start))) {
        makeActorAlive();
        al::setNerve(this, &NrvDoorAreaChange.NoStart);
        return;
    }
    s32 scenarioNo = 0;
    if (al::tryGetArg(&scenarioNo, info, "FixScenarioNo")) {
        if (scenarioNo == GameDataFunction::getScenarioNo(this)) {
            makeActorAlive();
            al::setNerve(this, &NrvDoorAreaChange.NoStart);
            return;
        }
    }
    if (rs::isOnSaveObjInfo(mSaveObjInfo))
        al::setNerve(this, &NrvDoorAreaChange.OpenWait);

    if (al::tryGetArg(&mIsNeedAppearCapMessage, info, "IsNeedAppearCapMessage") &&
        mIsNeedAppearCapMessage)
        mIsNeedAppearCapMessage = true;

    makeActorAlive();
}

bool DoorAreaChange::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                al::HitSensor* self) {
    if (rs::isMsgPlayerEyePriorityTarget(message))
        return true;
    if (rs::isMsgKoopaRingBeamInvalidTouch(message))
        return true;

    if (al::isNerve(this, &NrvDoorAreaChange.Open) ||
        al::isNerve(this, &NrvDoorAreaChange.OpenWait)) {
        if (rs::isMsgPlayerDisregardTargetMarker(message))
            return true;
        if (rs::isMsgPlayerDisregardHomingAttack(message))
            return true;
        if (isMsgPlayerDisregard(message))
            return true;
    }

    if (rs::isMsgCapTouchWall(message)) {
        if (al::isNerve(this, &NrvDoorAreaChange.NoStartWithMessage)) {
            sead::Vector3f capTouchPos = sead::Vector3f::zero;
            rs::tryGetCapTouchWallHitPos(&capTouchPos, message);
            al::startHitReactionHitEffect(this, "ヒット", capTouchPos);
            rs::requestHitReactionToAttacker(message, other, capTouchPos);
            return true;
        }
        if (al::isNerve(this, &NrvDoorAreaChange.CloseWait)) {
            rs::onSaveObjInfo(mSaveObjInfo);
            sead::Vector3f capTouchPos = sead::Vector3f::zero;
            rs::tryGetCapTouchWallHitPos(&capTouchPos, message);
            al::startHitReactionHitEffect(this, "ヒット", capTouchPos);
            rs::requestHitReactionToAttacker(message, other, capTouchPos);
            al::setNerve(this, &NrvDoorAreaChange.Open);
            return true;
        }
    }

    if (rs::isMsgCapReflect(message)) {
        if (al::isNerve(this, &NrvDoorAreaChange.NoStartWithMessage)) {
            al::startHitReactionHitEffect(this, "ヒット", other, self);
            rs::requestHitReactionToAttacker(message, self, other);
            rs::tryShowCapMsgWarpDisableInMiniGameDoorCap(this);
            return true;
        }
        if (al::isNerve(this, &NrvDoorAreaChange.CloseWait)) {
            rs::onSaveObjInfo(mSaveObjInfo);
            al::startHitReactionHitEffect(this, "ヒット", other, self);
            rs::requestHitReactionToAttacker(message, self, other);
            al::setNerve(this, &NrvDoorAreaChange.Open);
            return true;
        }
    }
    return false;
}

void DoorAreaChange::switchCloseAgain() {
    al::startAction(this, "Wait");
    al::validateCollisionParts(this);
    al::setNerve(this, &NrvDoorAreaChange.CloseWait);
}

void DoorAreaChange::start() {
    al::setNerve(this, &NrvDoorAreaChange.CloseWait);
}

void DoorAreaChange::setNoStart() {
    al::setNerve(this, &NrvDoorAreaChange.NoStart);
}

void DoorAreaChange::enableStart() {
    if (al::isNerve(this, &NrvDoorAreaChange.NoStart))
        al::setNerve(this, &NrvDoorAreaChange.CloseWait);
}

void DoorAreaChange::appear() {
    if (rs::isOnSaveObjInfo(mSaveObjInfo)) {
        kill();
        return;
    }
    al::LiveActor::appear();
}

bool DoorAreaChange::isOpen() const {
    return al::isNerve(this, &NrvDoorAreaChange.Open) ||
           al::isNerve(this, &NrvDoorAreaChange.OpenWait);
}

void DoorAreaChange::exeOpenWait() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "OpenWait");
        al::invalidateCollisionParts(this);
    }
    if (mIsHomeDoorSet) {
        if (GameDataFunction::isEnableCap(GameDataHolderAccessor(this)))
            return;
        al::validateCollisionParts(this);
        al::setNerve(this, &NrvDoorAreaChange.CloseWait);
    }
}

void DoorAreaChange::exeNoStart() {
    if (al::isFirstStep(this)) {
        al::validateCollisionParts(this);
        if (al::isExistAction(this, "CloseWait"))
            al::startAction(this, "Wait");
        else
            al::startAction(this, "CloseWait");
    }
}

void DoorAreaChange::exeCloseBefore() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "OpenWait");
        al::validateCollisionParts(this);
    }
    if (al::isGreaterEqualStep(this, 30))
        al::setNerve(this, &Close);
}

void DoorAreaChange::exeClose() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Close");
        al::validateCollisionParts(this);
    }
    if (al::isActionEnd(this))
        al::setNerve(this, &NrvDoorAreaChange.CloseWait);
}

void DoorAreaChange::setHomeDoor(bool isHomeDoor) {
    mIsHomeDoorSet = true;
    makeActorAlive();

    if (isHomeDoor)
        al::setNerve(this, &NrvDoorAreaChange.CloseBefore);
    else
        al::setNerve(this, &NrvDoorAreaChange.CloseWait);
}

void DoorAreaChange::exeOpen() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Open");
        al::invalidateCollisionParts(this);
        rs::tryCancelCapMessage(this, "DoorAreaChangeFirst");
    }
    if (al::isActionEnd(this)) {
        if (!mIsHomeDoorSet) {
            al::startHitReaction(this, "消滅");
            al::tryOnStageSwitch(this, "SwitchMoveOn");
        }
        al::setNerve(this, &NrvDoorAreaChange.OpenWait);
    }
}

void DoorAreaChange::exeCloseWait() {
    if (al::isFirstStep(this)) {
        if (!al::isExistAction(this, "CloseWait"))
            al::startAction(this, "Wait");
        else
            al::startAction(this, "CloseWait");
    }
    if (mIsNeedAppearCapMessage && al::isNearPlayer(this, 500)) {
        mIsNeedAppearCapMessage = false;
        rs::showCapMessage(this, "DoorAreaChangeFirst", 90, 600);
    }
}

void DoorAreaChange::exeNoStartWithMessage() {
    if (al::isFirstStep(this)) {
        if (!al::isExistAction(this, "CloseWait"))
            al::startAction(this, "Wait");
        else
            al::startAction(this, "CloseWait");
    }
}
