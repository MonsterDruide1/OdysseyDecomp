#include "MapObj/CapAcceleratorKeyMoveMapParts.h"

#include "Library/KeyPose/KeyPoseKeeper.h"
#include "Library/KeyPose/KeyPoseKeeperUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
using CapAcceleratorKeyMoveMapParts = al::CapAcceleratorKeyMoveMapParts;
NERVE_ACTION_IMPL(CapAcceleratorKeyMoveMapParts, Wait)
NERVE_ACTION_IMPL(CapAcceleratorKeyMoveMapParts, Move)
NERVE_ACTION_IMPL(CapAcceleratorKeyMoveMapParts, Stop)

NERVE_ACTIONS_MAKE_STRUCT(CapAcceleratorKeyMoveMapParts, Wait, Move, Stop)
}  // namespace

al::CapAcceleratorKeyMoveMapParts::CapAcceleratorKeyMoveMapParts(const char* name)
    : LiveActor(name) {}

void al::CapAcceleratorKeyMoveMapParts::init(const ActorInitInfo& info) {
    al::initNerveAction(this, "Wait", &::NrvCapAcceleratorKeyMoveMapParts.collector, 0);
    al::initActorWithArchiveName(this, info, "CapAcceleratorKeyMoveMapParts", nullptr);
    mKeyPoseKeeper = al::createKeyPoseKeeper(info);
    makeActorAlive();
}

void al::CapAcceleratorKeyMoveMapParts::control() {}

void al::CapAcceleratorKeyMoveMapParts::exeWait() {
    if (al::isFirstStep(this)) {
        int waitTime = al::calcKeyMoveWaitTime(mKeyPoseKeeper);
        if (!((waitTime >> 31) & 1))
            mKeyMoveWaitTime = waitTime;
    }

    if (al::isGreaterEqualStep(this, mKeyMoveWaitTime)) {
        if (al::isRestart(mKeyPoseKeeper)) {
            al::restartKeyPose(mKeyPoseKeeper, al::getTransPtr(this), al::getQuatPtr(this));
            al::resetPosition(this);
            al::startNerveAction(this, "Wait");
        } else {
            al::startNerveAction(this, "Move");
        }
    }
}

void al::CapAcceleratorKeyMoveMapParts::exeMove() {
    if (al::isFirstStep(this))
        mKeyMoveMoveTime = al::calcKeyMoveMoveTime(mKeyPoseKeeper);

    f32 rate = al::calcNerveRate(this, mKeyMoveMoveTime);
    al::calcLerpKeyTrans(al::getTransPtr(this), mKeyPoseKeeper, rate);
    al::calcSlerpKeyQuat(al::getQuatPtr(this), mKeyPoseKeeper, rate);

    if (al::isGreaterEqualStep(this, mKeyMoveMoveTime)) {
        al::nextKeyPose(mKeyPoseKeeper);
        if (al::isStop(mKeyPoseKeeper))
            al::startNerveAction(this, "Stop");
        else
            al::startNerveAction(this, "Wait");
    }
}

void al::CapAcceleratorKeyMoveMapParts::exeStop() {
    if (al::isFirstStep(this) && al::isInvalidClipping(this))
        al::validateClipping(this);
}
