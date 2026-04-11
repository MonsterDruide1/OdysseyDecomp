#include "MapObj/LavaFryingPan.h"

#include <math/seadVector.h>

#include "Library/Fluid/RippleCtrl.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Movement/ClockMovement.h"
#include "Library/Nerve/NerveSetupUtil.h"

namespace {
NERVE_ACTION_IMPL(LavaFryingPan, Delay)
NERVE_ACTION_IMPL(LavaFryingPan, RotateSign)
NERVE_ACTION_IMPL(LavaFryingPan, Rotate)
NERVE_ACTION_IMPL(LavaFryingPan, WaitUp)
NERVE_ACTION_IMPL(LavaFryingPan, WaitDown)

NERVE_ACTIONS_MAKE_STRUCT(LavaFryingPan, Delay, RotateSign, Rotate, WaitUp, WaitDown)
}  // namespace

LavaFryingPan::LavaFryingPan(const char* name) : al::LiveActor(name) {}

void LavaFryingPan::init(const al::ActorInitInfo& info) {
    al::initNerveAction(this, "Rotate", &NrvLavaFryingPan.collector, 0);
    al::initActor(this, info);
    mClockMovement = new al::ClockMovement(info);
    mRippleCtrl = al::RippleCtrl::tryCreate(this);

    if (mRippleCtrl) {
        mRippleCtrl->init(info);
        mRippleCtrl->set_110(true);
    }

    makeActorAlive();
}

void LavaFryingPan::exeDelay() {}

void LavaFryingPan::exeRotateSign() {}

void LavaFryingPan::exeRotate() {}

void LavaFryingPan::exeWaitUp() {}

void LavaFryingPan::exeWaitDown() {}

void LavaFryingPan::control() {
    mClockMovement->updateNerve();
    al::setQuat(this, mClockMovement->getQuat());

    if (mRippleCtrl)
        mRippleCtrl->update();

    if (mClockMovement->isFirstStepRotateSign()) {
        al::startNerveAction(this, "RotateSign");
        return;
    }

    if (mClockMovement->isFirstStepRotate()) {
        al::startNerveAction(this, "Rotate");
        return;
    }

    if (mClockMovement->isFirstStepWait()) {
        if (al::isUpDir(this, sead::Vector3f::ey))
            al::startNerveAction(this, "WaitUp");
        else
            al::startNerveAction(this, "WaitDown");
    }
}
