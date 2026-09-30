#include "MapObj/KuriboTowerSwitch.h"

#include <math/seadMathCalcCommon.h>
#include <math/seadVector.h>

#include "Library/Audio/System/SimpleAudioUser.h"
#include "Library/Collision/PartsConnectorUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Obj/CollisionObj.h"
#include "Library/Obj/PartsFunction.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Se/SeFunction.h"
#include "Library/Stage/StageSwitchUtil.h"

#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(KuriboTowerSwitch, OffWait);
NERVE_IMPL(KuriboTowerSwitch, On);
NERVE_IMPL(KuriboTowerSwitch, OnWait);
NERVE_IMPL(KuriboTowerSwitch, Disable);
NERVE_IMPL(KuriboTowerSwitch, ReactionLand);

NERVES_MAKE_STRUCT(KuriboTowerSwitch, OffWait, On, OnWait, Disable, ReactionLand);
}  // namespace

KuriboTowerSwitch::KuriboTowerSwitch(const char* name) : al::LiveActor(name) {}

void KuriboTowerSwitch::init(const al::ActorInitInfo& initInfo) {
    al::initActorWithArchiveName(this, initInfo, "KuriboTowerSwitch", nullptr);
    al::initNerve(this, &NrvKuriboTowerSwitch.OffWait, 0);
    mMtxConnector = al::createMtxConnector(this);
    mCollisionObj = al::createCollisionObj(this, initInfo, "KuriboTowerSwitch_Body",
                                           al::getHitSensor(this, "Collision"), "Move", nullptr);
    mCollisionObj->makeActorAlive();
    al::validateCollisionParts(mCollisionObj);
    makeActorAlive();

    al::tryGetArg(&mNum, initInfo, "Num");
    al::startMtpAnimAndSetFrameAndStop(this, "Number", sead::Mathi::min(mNum - 1, 9));

    bool isPlaySuccessSe = false;
    al::tryGetArg(&isPlaySuccessSe, initInfo, "IsPlaySuccessSe");
    if (isPlaySuccessSe)
        mAudioUser = new al::SimpleAudioUser("SuccessSeObj", initInfo);
}

void KuriboTowerSwitch::initAfterPlacement() {
    if (mMtxConnector)
        al::attachMtxConnectorToCollision(mMtxConnector, this, false);
}

void KuriboTowerSwitch::control() {
    if (mMtxConnector)
        al::connectPoseQT(this, mMtxConnector);

    if (!_129)
        _128 = true;
    _129 = false;

    if (mDisplayNum < 4)
        mDisplayNum++;
}

bool KuriboTowerSwitch::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                   al::HitSensor* self) {
    if (rs::isMsgPlayerDisregardHomingAttack(message))
        return true;

    if (al::isNerve(this, &NrvKuriboTowerSwitch.On) ||
        al::isNerve(this, &NrvKuriboTowerSwitch.OnWait) ||
        al::isNerve(this, &NrvKuriboTowerSwitch.Disable)) {
        return false;
    }

    if (rs::getNumKuriboTowerOn(message) < (u32)mNum) {
        if (!al::isMsgPlayerFloorTouch(message))
            return false;

        if (3 < mDisplayNum && _128 && isReactionNerve()) {
            _128 = false;
            al::setNerve(this, &NrvKuriboTowerSwitch.ReactionLand);
        }

        _129 = true;
        mDisplayNum = 0;
        return true;
    }

    sead::Vector3f dir = al::getSensorPos(other) - al::getSensorPos(self);
    sead::Vector3f up;
    al::calcUpDir(&up, this);
    al::verticalizeVec(&dir, up, dir);
    if (dir.length() > 100.0f)
        return false;

    al::startHitReaction(this, "規定数乗っかった");
    al::invalidateClipping(this);
    al::setNerve(this, &NrvKuriboTowerSwitch.On);
    return true;
}

bool KuriboTowerSwitch::isReactionNerve() const {
    return al::isNerve(this, &NrvKuriboTowerSwitch.OffWait) ||
           al::isNerve(this, &NrvKuriboTowerSwitch.ReactionLand);
}

void KuriboTowerSwitch::exeDisable() {
    if (al::isFirstStep(this))
        al::tryStartActionIfNotPlaying(this, "OffWait");
}

void KuriboTowerSwitch::exeOffWait() {
    if (al::isFirstStep(this))
        al::tryStartActionIfNotPlaying(this, "OffWait");
}

void KuriboTowerSwitch::exeReactionLand() {
    if (al::isFirstStep(this))
        al::startAction(this, "ReactionLand");

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvKuriboTowerSwitch.OffWait);
}

void KuriboTowerSwitch::exeOn() {
    if (al::isFirstStep(this)) {
        al::invalidateHitSensors(this);
        al::startAction(this, "On");
        if (mAudioUser)
            al::startSe(mAudioUser, "Riddle");
    }

    if (al::isActionEnd(this)) {
        mCollisionObj->kill();
        al::startHitReaction(this, "スイッチ押し込み終わり");
        al::setNerve(this, &NrvKuriboTowerSwitch.OnWait);
    }
}

void KuriboTowerSwitch::exeOnWait() {
    if (al::isFirstStep(this)) {
        al::tryOnStageSwitch(this, "SwitchTrampleOn");
        al::startAction(this, "OnWait");
        al::validateClipping(this);
    }
}
