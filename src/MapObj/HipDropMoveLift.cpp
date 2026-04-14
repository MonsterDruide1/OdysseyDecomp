#include "MapObj/HipDropMoveLift.h"

#include "Library/KeyPose/KeyPoseKeeper.h"
#include "Library/KeyPose/KeyPoseKeeperUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_ACTION_IMPL(HipDropMoveLift, StandBy)
NERVE_ACTION_IMPL(HipDropMoveLift, Wait)
NERVE_ACTION_IMPL(HipDropMoveLift, Move)

NERVE_ACTIONS_MAKE_STRUCT(HipDropMoveLift, StandBy, Wait, Move)
}  // namespace

HipDropMoveLift::HipDropMoveLift(const char* name) : al::LiveActor(name) {}

void HipDropMoveLift::init(const al::ActorInitInfo& info) {
    al::initNerveAction(this, "Wait", &NrvHipDropMoveLift.collector, 0);
    al::initMapPartsActor(this, info, nullptr);
    mKeyPoseKeeper = al::createKeyPoseKeeper(info);
    mKeyPoseKeeper->setMoveTypeTurn();
    al::registerAreaHostMtx(this, info);
    al::startNerveAction(this, "StandBy");
    makeActorAlive();
}

bool HipDropMoveLift::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                 al::HitSensor* self) {
    if (al::isMsgPlayerHipDropAll(message) &&
        al::isNerve(this, NrvHipDropMoveLift.StandBy.data())) {
        start();
        return true;
    }

    return false;
}

void HipDropMoveLift::start() {
    if (!al::isNerve(this, NrvHipDropMoveLift.StandBy.data()) ||
        al::getKeyPoseCount(mKeyPoseKeeper) < 2)
        return;

    if (al::isExistAction(this, "Start"))
        al::startAction(this, "Start");
    al::startNerveAction(this, "Wait");
    al::invalidateClipping(this);
}

void HipDropMoveLift::exeStandBy() {
    if (al::isFirstStep(this))
        al::validateClipping(this);
}

void HipDropMoveLift::exeWait() {
    if (al::isFirstStep(this)) {
        s32 waitTime = al::calcKeyMoveWaitTime(mKeyPoseKeeper);
        if (waitTime >= 0)
            mWaitTime = waitTime;
    }

    if (al::isGreaterEqualStep(this, mWaitTime))
        setWaitEndNerve();
}

void HipDropMoveLift::setWaitEndNerve() {
    if (!al::isRestart(mKeyPoseKeeper)) {
        al::startNerveAction(this, "Move");
        return;
    }

    al::restartKeyPose(mKeyPoseKeeper, al::getTransPtr(this), al::getQuatPtr(this));
    al::resetPosition(this);
    al::startNerveAction(this, "Wait");
}

void HipDropMoveLift::exeMove() {
    if (al::isFirstStep(this))
        mMoveTime = al::calcKeyMoveMoveTime(mKeyPoseKeeper);

    f32 rate = al::calcNerveRate(this, mMoveTime);
    al::calcLerpKeyTrans(al::getTransPtr(this), mKeyPoseKeeper, rate);
    al::calcSlerpKeyQuat(al::getQuatPtr(this), mKeyPoseKeeper, rate);

    if (al::isGreaterEqualStep(this, mMoveTime)) {
        al::nextKeyPose(mKeyPoseKeeper);

        if (al::isFirstKey(mKeyPoseKeeper))
            al::startNerveAction(this, "StandBy");
        else if (al::calcKeyMoveWaitTime(mKeyPoseKeeper) != 0)
            al::startNerveAction(this, "Wait");
        else
            setWaitEndNerve();
    }
}
