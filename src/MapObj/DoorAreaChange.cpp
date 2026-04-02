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
using namespace al;

NERVE_IMPL(DoorAreaChange, Wait);
NERVE_IMPL(DoorAreaChange, Start);
NERVE_IMPL(DoorAreaChange, OpenWait);
NERVE_IMPL(DoorAreaChange, NoStart);
NERVE_IMPL(DoorAreaChange, CloseBefore);
NERVE_IMPL(DoorAreaChange, Open);
NERVE_IMPL(DoorAreaChange, Close);
NERVE_IMPL(DoorAreaChange, CloseWait);
NERVE_IMPL(DoorAreaChange, NoStartWithMessage);

NERVES_MAKE_STRUCT(DoorAreaChange, Wait, OpenWait, NoStart, Open, CloseBefore, Start,
                   NoStartWithMessage);
NERVES_MAKE_NOSTRUCT(DoorAreaChange, Close, CloseWait);
}  // namespace

DoorAreaChange::DoorAreaChange(const char* name) : al::LiveActor(name) {}

void DoorAreaChange::init(const al::ActorInitInfo& info) {
    using DoorAreaChangeFunctor = FunctorV0M<DoorAreaChange*, void (DoorAreaChange::*)()>;

    if (al::isObjectName(info, "ShineTowerRocket"))
        initActorWithArchiveName(this, info, "DoorAreaChange", nullptr);
    else
        initActorWithArchiveName(this, info, "ShineTowerDoor", nullptr);
    initNerve(this, &CloseWait, 0);
    mSaveObjInfo = rs::createSaveObjInfoWriteSaveData(info);
    listenStageSwitchOn(this, "SwitchCloseAgain",
                        DoorAreaChangeFunctor(this, &DoorAreaChange::switchCloseAgain));
    if (!rs::isInvalidChangeStage(this)) {
        if (GameDataFunction::isPlayerStartLinkedObj(this, info, "NoDelete_DeadByPlayerStart")) {
            setNerve(this, &NrvDoorAreaChange.OpenWait);
            makeActorAlive();
            return;
        }
        if (listenStageSwitchOnAppear(this, DoorAreaChangeFunctor(this, &DoorAreaChange::appear))) {
            makeActorDead();
            return;
        }
        if (listenStageSwitchOnStart(this, DoorAreaChangeFunctor(this, &DoorAreaChange::start))) {
            makeActorAlive();
            setNerve(this, &NrvDoorAreaChange.NoStart);
            return;
        }
        s32 scenarioNo = 0;
        if (tryGetArg(&scenarioNo, info, "FixScenarioNo"))
            if (scenarioNo == GameDataFunction::getScenarioNo(this)) {
                makeActorAlive();
                setNerve(this, &NrvDoorAreaChange.NoStart);
                return;
            }
        if (rs::isOnSaveObjInfo(mSaveObjInfo))
            setNerve(this, &NrvDoorAreaChange.OpenWait);
        if (tryGetArg(&mIsDoorClosed, info, "IsNeedAppearCapMessage") && mIsDoorClosed)
            mIsDoorClosed = true;
    } else {
        setNerve(this, &NrvDoorAreaChange.Wait);
    }
    makeActorAlive();
}

bool DoorAreaChange::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                al::HitSensor* self) {
    if (rs::isMsgPlayerEyePriorityTarget(message))
        return true;
    if (rs::isMsgKoopaRingBeamInvalidTouch(message))
        return true;
    if (isNerve(this, &NrvDoorAreaChange.Open) || isNerve(this, &NrvDoorAreaChange.OpenWait)) {
        if (rs::isMsgPlayerDisregardTargetMarker(message))
            return true;
        if (rs::isMsgPlayerDisregardHomingAttack(message))
            return true;
        if (isMsgPlayerDisregard(message))
            return true;
    }
    if (rs::isMsgCapTouchWall(message)) {
        if (isNerve(this, &NrvDoorAreaChange.Wait)) {
            sead::Vector3f zero = sead::Vector3f::zero;
            rs::tryGetCapTouchWallHitPos(&zero, message);
            startHitReactionHitEffect(this, "ヒット", zero);
            rs::requestHitReactionToAttacker(message, other, zero);
            return true;
        }
        if (isNerve(this, &CloseWait)) {
            rs::onSaveObjInfo(mSaveObjInfo);
            sead::Vector3f zero = sead::Vector3f::zero;
            rs::tryGetCapTouchWallHitPos(&zero, message);
            startHitReactionHitEffect(this, "ヒット", zero);
            rs::requestHitReactionToAttacker(message, other, zero);
            setNerve(this, &NrvDoorAreaChange.Open);
            return true;
        }
    }
    if (!rs::isMsgCapReflect(message))
        return false;
    if (isNerve(this, &NrvDoorAreaChange.Wait)) {
        startHitReactionHitEffect(this, "ヒット", other, self);
        rs::requestHitReactionToAttacker(message, self, other);
        rs::tryShowCapMsgWarpDisableInMiniGameDoorCap(this);
        return true;
    }
    if (!isNerve(this, &CloseWait))
        return false;
    rs::onSaveObjInfo(mSaveObjInfo);
    startHitReactionHitEffect(this, "ヒット", other, self);
    rs::requestHitReactionToAttacker(message, self, other);
    setNerve(this, &NrvDoorAreaChange.Open);
    return true;
}

void DoorAreaChange::switchCloseAgain() {
    al::startAction(this, "Wait");
    al::validateCollisionParts(this);
    al::setNerve(this, &CloseWait);
}

void DoorAreaChange::start() {
    al::setNerve(this, &CloseWait);
}

void DoorAreaChange::setNoStart() {
    setNerve(this, &NrvDoorAreaChange.NoStart);
}

void DoorAreaChange::enableStart() {
    if (isNerve(this, &NrvDoorAreaChange.NoStart))
        setNerve(this, &CloseWait);
}

void DoorAreaChange::appear() {
    if (rs::isOnSaveObjInfo(mSaveObjInfo)) {
        kill();
        return;
    }
    al::LiveActor::appear();
}

bool DoorAreaChange::isOpen() const {
    if (isNerve(this, &NrvDoorAreaChange.Open))
        return true;
    return isNerve(this, &NrvDoorAreaChange.OpenWait);
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
        al::setNerve(this, &CloseWait);
    }
}

void DoorAreaChange::exeNoStart() {
    if (isFirstStep(this)) {
        validateCollisionParts(this);
        if (isExistAction(this, "CloseWait"))
            startAction(this, "Wait");
        else
            startAction(this, "CloseWait");
    }
}

void DoorAreaChange::exeCloseBefore() {
    if (isFirstStep(this)) {
        startAction(this, "OpenWait");
        validateCollisionParts(this);
    }
    if (isGreaterEqualStep(this, 30))
        setNerve(this, &CloseWait);
}

void DoorAreaChange::exeClose() {
    if (isFirstStep(this)) {
        startAction(this, "Close");
        validateCollisionParts(this);
    }
    if (isActionEnd(this))
        setNerve(this, &Close);
}

void DoorAreaChange::setHomeDoor(bool b) {
    mIsHomeDoorSet = true;
    makeActorAlive();

    if (b)
        setNerve(this, &NrvDoorAreaChange.CloseBefore);
    else
        setNerve(this, &CloseWait);
}

void DoorAreaChange::exeOpen() {
    if (isFirstStep(this)) {
        startAction(this, "Open");
        invalidateCollisionParts(this);
        rs::tryCancelCapMessage(this, "DoorAreaChangeFirst");
    }
    if (isActionEnd(this)) {
        if (!mIsHomeDoorSet) {
            startHitReaction(this, "消滅");
            tryOnStageSwitch(this, "SwitchMoveOn");
        }
        setNerve(this, &NrvDoorAreaChange.OpenWait);
    }
}

void DoorAreaChange::exeCloseWait() {
    if (isFirstStep(this)) {
        if (!isExistAction(this, "CloseWait"))
            startAction(this, "Wait");
        else
            startAction(this, "CloseWait");
    }
    if (mIsDoorClosed && isNearPlayer(this, 500)) {
        mIsDoorClosed = false;
        rs::showCapMessage(this, "DoorAreaChangeFirst", 90, 600);
    }
}

void DoorAreaChange::exeNoStartWithMessage() {
    if (isFirstStep(this)) {
        if (!isExistAction(this, "CloseWait"))
            startAction(this, "Wait");
        else
            startAction(this, "CloseWait");
    }
}
