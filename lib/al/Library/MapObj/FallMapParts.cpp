#include "Library/MapObj/FallMapParts.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"

namespace {
using namespace al;

NERVE_ACTION_IMPL(FallMapParts, Appear)
NERVE_ACTION_IMPL(FallMapParts, Wait)
NERVE_ACTION_IMPL(FallMapParts, FallSign)
NERVE_ACTION_IMPL(FallMapParts, Fall)
NERVE_ACTION_IMPL(FallMapParts, End)

NERVE_ACTIONS_MAKE_STRUCT(FallMapParts, Appear, Wait, FallSign, Fall, End)
}  // namespace

namespace al {
FallMapParts::FallMapParts(const char* name) : LiveActor(name) {}

void FallMapParts::init(const ActorInitInfo& info) {
    initNerveAction(this, "Wait", &NrvFallMapParts.mCollector, 0);
    initMapPartsActor(this, info, nullptr);
    registerAreaHostMtx(this, info);

    mPos = getTrans(this);

    tryGetArg(&mFallTime, info, "FallTime");
    tryGetArg(&mIsInvalidAutoRestart, info, "IsInvalidAutoRestart");

    trySyncStageSwitchAppear(this);
}

bool FallMapParts::receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) {
    if (isMsgFloorTouch(message) && isNerve(this, NrvFallMapParts.Wait.data())) {
        startNerveAction(this, "FallSign");
        invalidateClipping(this);

        return true;
    }

    if (isMsgShowModel(message)) {
        if (!isNerve(this, NrvFallMapParts.End.data()))
            showModelIfHide(this);

        return true;
    }

    if (isMsgHideModel(message)) {
        if (!isNerve(this, NrvFallMapParts.End.data()))
            hideModelIfShow(this);

        return true;
    }

    if (isMsgRestart(message)) {
        appearAndSetStart();

        return true;
    }

    return false;
}

void FallMapParts::appearAndSetStart() {
    setTrans(this, mPos);
    resetPosition(this);
    showModelIfHide(this);
    startNerveAction(this, "Appear");
    setVelocityZero(this);

    makeActorAlive();
}

void FallMapParts::exeAppear() {
    if (isFirstStep(this)) {
        validateCollisionParts(this);
        if (!tryStartAction(this, "Appear")) {
            startAction(this, "Wait");

            return;
        }
    }

    if (!isExistAction(this) || isActionEnd(this))
        startAction(this, "Wait");
}

void FallMapParts::exeWait() {
    if (isFirstStep(this)) {
        tryStartAction(this, "Wait");
        validateClipping(this);
    }
}

void FallMapParts::exeFallSign() {
    if (isFirstStep(this))
        mIsStartAction = tryStartAction(this, "FallSign");

    if (!mIsStartAction) {
        f32 offset = sead::Mathf::sin(calcNerveValue(this, 20, 0.0f, sead::Mathf::pi() * 3)) * 3;
        setTrans(this, offset * sead::Vector3f::ey + mPos);
    }

    if (isEndFallSign())
        startNerveAction(this, "Fall");
}

bool FallMapParts::isEndFallSign() const {
    return mIsStartAction ? isActionEnd(this) : isGreaterEqualStep(this, 20);
}

void FallMapParts::exeFall() {
    if (isFirstStep(this)) {
        tryStartAction(this, "Fall");
        setTrans(this, mPos);
    }

    addVelocityToGravity(this, 0.3f);
    scaleVelocity(this, 0.9f);

    if (isGreaterStep(this, mFallTime))
        startNerveAction(this, "End");
}

void FallMapParts::exeEnd() {
    if (isFirstStep(this)) {
        tryStartAction(this, "End");
        hideModelIfShow(this);
        invalidateCollisionParts(this);
        setVelocityZero(this);

        if (mIsInvalidAutoRestart) {
            kill();

            return;
        }
    }

    if (isGreaterStep(this, 120)) {
        setTrans(this, mPos);
        resetPosition(this);
        showModelIfHide(this);
        startNerveAction(this, "Appear");
    }
}
}  // namespace al
