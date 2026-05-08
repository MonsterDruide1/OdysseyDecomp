#include "MapObj/CapAccelerator.h"

#include <math/seadVector.h>

#include "Library/Collision/PartsConnectorUtil.h"
#include "Library/KeyPose/KeyPoseKeeper.h"
#include "Library/KeyPose/KeyPoseKeeperUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"

#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(CapAccelerator, Wait);
NERVE_IMPL(CapAccelerator, Hit);
NERVE_IMPL(CapAccelerator, Trample);
NERVE_IMPL(CapAccelerator, Reaction);

NERVES_MAKE_STRUCT(CapAccelerator, Wait, Hit, Trample, Reaction);
}  // namespace

CapAccelerator::CapAccelerator(const char* name) : al::LiveActor(name) {}

void CapAccelerator::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "CapAccelerator", nullptr);
    al::initNerve(this, &NrvCapAccelerator.Wait, 0);

    if (al::isObjectName(info, "CapAcceleratorKeyMoveMapParts")) {
        auto* keyMoveMapParts = new al::CapAcceleratorKeyMoveMapParts("キームーブマップパーツ");
        al::initCreateActorWithPlacementInfo(keyMoveMapParts, info);
        mKeyMoveMapParts = keyMoveMapParts;
        al::initSubActorKeeperNoFile(this, info, 1);
        al::registerSubActor(this, mKeyMoveMapParts);
    } else {
        mMtxConnector = al::tryCreateMtxConnector(this, info);
    }

    al::startAction(this, "Wait");
    makeActorAlive();
}

void CapAccelerator::initAfterPlacement() {
    if (mMtxConnector)
        al::attachMtxConnectorToCollision(mMtxConnector, this, false);
}

void CapAccelerator::control() {
    if (mMtxConnector)
        al::connectPoseQT(this, mMtxConnector);
    else if (mKeyMoveMapParts)
        al::copyPose(this, mKeyMoveMapParts);
}

bool CapAccelerator::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                al::HitSensor* self) {
    if (rs::isMsgCapBeamerBeam(message)) {
        if (al::isGreaterStep(this, 20)) {
            mCanReceiveRethrow = false;
            al::setNerve(this, &NrvCapAccelerator.Wait);
            return true;
        }

        return false;
    }

    if (al::isMsgPlayerTrampleReflect(message) ||
        rs::isMsgPlayerAndCapObjHipDropReflectAll(message)) {
        mCanReceiveRethrow = false;
        rs::requestHitReactionToAttacker(message, self, other);
        al::setNerve(this, &NrvCapAccelerator.Hit);
        return true;
    }

    if ((rs::isMsgCapItemGet(message) || rs::isMsgCapStartLockOn(message)) &&
        al::isSensorSimple(self) && !mCanReceiveRethrow) {
        sead::Vector3f front;
        sead::Vector3f up;
        al::calcFrontDir(&front, this);
        al::calcUpDir(&up, this);

        sead::Vector3f sendPos = al::getSensorPos(self) + front;

        if (rs::sendMsgCapRethrow(other, self, sendPos, front, up)) {
            mCanReceiveRethrow = true;
            al::setNerve(this, &NrvCapAccelerator.Trample);
        }
    }

    return false;
}

void CapAccelerator::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isSensorMapObj(self))
        rs::sendMsgPushToPlayerAndKillVelocityToTarget(this, self, other);
}

void CapAccelerator::exeWait() {}

void CapAccelerator::exeReaction() {
    if (al::isFirstStep(this))
        al::startAction(this, "Reaction");

    if (al::isStep(this, 30))
        mCanReceiveRethrow = false;

    if (al::isActionEnd(this)) {
        mCanReceiveRethrow = false;
        al::startAction(this, "Wait");
        al::setNerve(this, &NrvCapAccelerator.Wait);
    }
}

void CapAccelerator::exeTrample() {
    if (al::isFirstStep(this))
        al::startAction(this, "ReactionTrample");

    if (al::isActionEnd(this)) {
        al::startAction(this, "Wait");
        al::setNerve(this, &NrvCapAccelerator.Wait);
    }
}

void CapAccelerator::exeHit() {
    if (al::isFirstStep(this))
        al::startAction(this, "Hit");

    if (al::isActionEnd(this)) {
        al::startAction(this, "Wait");
        al::setNerve(this, &NrvCapAccelerator.Wait);
    }
}
