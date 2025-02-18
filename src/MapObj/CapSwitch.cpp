#include "MapObj/CapSwitch.h"

#include "Library/Collision/PartsConnector.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchKeeper.h"
#include "Library/Thread/FunctorV0M.h"

#include "MapObj/AppearSwitchSave.h"
#include "Player/CapTargetInfo.h"
#include "Util/Hack.h"
#include "Util/PlayerUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(CapSwitch, OffWaitInvalid);
NERVE_IMPL(CapSwitch, OffWait);
NERVE_IMPL(CapSwitch, OnWait);
NERVE_IMPL(CapSwitch, ReturnOff);
NERVE_IMPL(CapSwitch, OffWaitCapHold);
NERVE_IMPL(CapSwitch, HitReaction);

NERVES_MAKE_NOSTRUCT(CapSwitch, HitReaction);
NERVES_MAKE_STRUCT(CapSwitch, OffWaitInvalid, OffWait, OnWait, ReturnOff, OffWaitCapHold);
}  // namespace

CapSwitch::CapSwitch(const char* name) : al::LiveActor(name) {}

void CapSwitch::init(const al::ActorInitInfo& info) {
    using CapSwitchFunctor = al::FunctorV0M<CapSwitch*, void (CapSwitch::*)()>;

    al::initActorWithArchiveName(this, info, "CapSwitch", nullptr);

    if (al::tryGetBoolArgOrFalse(info, "IsNoReaction"))
        al::initNerve(this, &NrvCapSwitch.OffWaitInvalid, 0);
    else {
        al::initNerve(this, &NrvCapSwitch.OffWait, 0);
        mMtxConnector = al::tryCreateMtxConnector(this, info);

        if (al::isObjectName(info, "CapSwitchSave")) {
            mAppearSwitchSave = new AppearSwitchSave(this, info);
            if (mAppearSwitchSave->isOn()) {
                al::tryOnStageSwitch(this, "CapAttackOn");
                al::setNerve(this, &NrvCapSwitch.OnWait);
                makeActorAlive();
                return;
            }
        }

        mCapTargetInfo = rs::createCapTargetInfo(this, nullptr);

        if (al::listenStageSwitchOnStart(this, CapSwitchFunctor(this, &CapSwitch::listenStart)))
            al::setNerve(this, &NrvCapSwitch.OffWaitInvalid);
        al::listenStageSwitchOn(this, "ResetSwitch",
                                CapSwitchFunctor(this, &CapSwitch::listenReset));
    }

    makeActorAlive();
}

void CapSwitch::listenStart() {
    al::setNerve(this, &NrvCapSwitch.OffWait);
}

void CapSwitch::listenReset() {
    if (isOn())
        al::setNerve(this, &NrvCapSwitch.ReturnOff);
}

bool CapSwitch::receiveMsg(const al::SensorMsg* msg, al::HitSensor* other, al::HitSensor* self) {
    if (al::isNerve(this, &NrvCapSwitch.OffWaitInvalid))
        return rs::isMsgPlayerDisregardHomingAttack(msg);

    if (rs::isMsgCapStartLockOn(msg) && al::isNerve(this, &NrvCapSwitch.OffWait))
        return true;

    if (rs::tryReceiveMsgInitCapTargetAndSetCapTargetInfo(msg, mCapTargetInfo)) {
        rs::tryGetFlyingCapPos(&mFlyingCapPos, this);
        al::invalidateClipping(this);
        al::setNerve(this, &NrvCapSwitch.OffWaitCapHold);
        return true;
    }

    if ((rs::isMsgCapKeepLockOn(msg) || rs::isMsgCapIgnoreCancelLockOn(msg)) &&
        al::isNerve(this, &NrvCapSwitch.OffWaitCapHold))
        return true;

    if (rs::isMsgCapCancelLockOn(msg))
        return true;

    return false;
}

void CapSwitch::initAfterPlacement() {
    if (mMtxConnector)
        al::attachMtxConnectorToCollision(mMtxConnector, this, false);
}

void CapSwitch::control() {
    if (!al::isNerve(this, &NrvCapSwitch.OffWaitInvalid) && mMtxConnector)
        al::connectPoseQT(this, mMtxConnector);
}

void CapSwitch::exeOffWait() {
    if (al::isFirstStep(this)) {
        al::validateClipping(this);
        al::validateHitSensors(this);
        al::startAction(this, "OffWait");
    }
}

void CapSwitch::exeOffWaitCapHold() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "HitReaction");
        mPlayerPos = rs::getPlayerPos(this);
    }

    sead::Vector3f hitTargetPos = sead::Vector3f::zero;
    al::calcJointPos(&hitTargetPos, this, "HatTarget");

    sead::Vector3f frontDir = sead::Vector3f::ez;
    al::calcFrontDir(&frontDir, this);

    f32 angle = al::isNearZero((hitTargetPos - mPlayerPos).normalize(), 0.001f) ?
                    0.0f :
                    al::calcAngleDegree(frontDir, hitTargetPos - mPlayerPos);

    sead::Vector3f dir =
        sead::Vector3f(0, al::isLeftTarget(this, mPlayerPos) ? -angle : angle, 270.0f);

    mCapTargetInfo->setFollowLockOnMtx("HatTarget", sead::Vector3f::zero, dir);

    if (al::isActionPlaying(this, "HitReaction") && al::isActionEnd(this))
        al::startAction(this, mIsFacingFront ? "HitReactionKeepFront" : "HitReactionKeepBack");

    if (al::isGreaterEqualStep(this, 30)) {
        rs::requestLockOnCapHitReaction(this, mCapTargetInfo, "帽子スイッチをオンにした");
        al::setNerve(this, &HitReaction);
    }
}

void CapSwitch::exeHitReaction() {
    if (al::isFirstStep(this)) {
        al::startHitReaction(this, "起動");
        al::startAction(this, mIsFacingFront ? "HitReactionFront" : "HitReactionBack");
    }

    if (al::isActionEnd(this)) {
        al::tryOnStageSwitch(this, "CapAttackOn");
        if (mAppearSwitchSave) {
            mAppearSwitchSave->onSwitch();
            al::tryOnStageSwitch(this, "CapAttackFirstOn");
        }
        al::setNerve(this, &NrvCapSwitch.OnWait);
    }
}

void CapSwitch::exeOnWait() {
    if (al::isFirstStep(this)) {
        al::invalidateHitSensors(this);
        al::validateClipping(this);
        al::startAction(this, mIsFacingFront ? "OnWaitFront" : "OnWaitBack");
    }
}

void CapSwitch::exeReturnOff() {
    if (al::isFirstStep(this))
        al::startAction(this, mIsFacingFront ? "OffFront" : "OffBack");

    if (al::isActionEnd(this)) {
        al::tryOffStageSwitch(this, "CapAttackOn");
        al::setNerve(this, &NrvCapSwitch.OffWait);
    }
}

void CapSwitch::exeOffWaitInvalid() {}

bool CapSwitch::isOn() const {
    return al::isNerve(this, &NrvCapSwitch.OnWait);
}
