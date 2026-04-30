#include "MapObj/AppearSwitchFallMapParts.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"

namespace {
using namespace al;

NERVE_ACTION_IMPL(AppearSwitchFallMapParts, Appear)
NERVE_ACTION_IMPL(AppearSwitchFallMapParts, Wait)
NERVE_ACTION_IMPL(AppearSwitchFallMapParts, FallSign)
NERVE_ACTION_IMPL(AppearSwitchFallMapParts, Fall)
NERVE_ACTION_IMPL(AppearSwitchFallMapParts, End)

NERVE_ACTIONS_MAKE_STRUCT(AppearSwitchFallMapParts, Appear, Wait, FallSign, Fall, End)
}  // namespace

AppearSwitchFallMapParts::AppearSwitchFallMapParts(const char* name) : al::LiveActor(name) {}

void AppearSwitchFallMapParts::init(const al::ActorInitInfo& info) {
    al::initNerveAction(this, "Wait", &NrvAppearSwitchFallMapParts.collector, 0);
    al::initMapPartsActor(this, info, nullptr);
    al::registerAreaHostMtx(this, info);

    mPos = al::getTrans(this);

    al::tryGetArg(&mFallTime, info, "FallTime");
    al::tryGetArg(&mIsInvalidAutoRestart, info, "IsInvalidAutoRestart");

    al::trySyncStageSwitchAppear(this);
}

bool AppearSwitchFallMapParts::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                          al::HitSensor* self) {
    if (al::isMsgFloorTouch(message) &&
        al::isNerve(this, NrvAppearSwitchFallMapParts.Wait.data())) {
        al::startNerveAction(this, "FallSign");
        al::invalidateClipping(this);

        return true;
    }

    if (al::isMsgShowModel(message)) {
        if (!al::isNerve(this, NrvAppearSwitchFallMapParts.End.data()))
            al::showModelIfHide(this);

        return true;
    }

    if (al::isMsgHideModel(message)) {
        if (!al::isNerve(this, NrvAppearSwitchFallMapParts.End.data()))
            al::hideModelIfShow(this);

        return true;
    }

    if (al::isMsgRestart(message)) {
        appearAndSetStart();

        return true;
    }

    return false;
}

void AppearSwitchFallMapParts::appearAndSetStart() {
    al::setTrans(this, mPos);
    al::resetPosition(this);
    al::showModelIfHide(this);
    al::startNerveAction(this, "Wait");
    al::setVelocityZero(this);
    al::validateCollisionParts(this);

    makeActorAlive();
}

void AppearSwitchFallMapParts::exeAppear() {
    if (al::isFirstStep(this)) {
        al::validateCollisionParts(this);
        if (!al::tryStartAction(this, "Appear")) {
            al::startNerveAction(this, "Wait");

            return;
        }
    }

    if (!al::isExistAction(this) || al::isActionEnd(this))
        al::startNerveAction(this, "Wait");
}

void AppearSwitchFallMapParts::exeWait() {
    if (al::isFirstStep(this)) {
        al::tryStartAction(this, "Wait");
        al::validateClipping(this);
    }
}

void AppearSwitchFallMapParts::exeFallSign() {
    if (al::isFirstStep(this))
        mIsStartAction = al::tryStartAction(this, "FallSign");

    if (!mIsStartAction) {
        f32 offset =
            sead::Mathf::sin(al::calcNerveValue(this, 20, 0.0f, sead::Mathf::pi() * 3)) * 3;
        al::setTrans(this, offset * sead::Vector3f::ey + mPos);
    }

    if (isEndFallSign())
        al::startNerveAction(this, "Fall");
}

bool AppearSwitchFallMapParts::isEndFallSign() const {
    return mIsStartAction ? al::isActionEnd(this) : al::isGreaterEqualStep(this, 20);
}

void AppearSwitchFallMapParts::exeFall() {
    if (al::isFirstStep(this)) {
        al::tryStartAction(this, "Fall");
        al::setTrans(this, mPos);
    }

    al::addVelocityToGravity(this, 0.3f);
    al::scaleVelocity(this, 0.9f);

    if (al::isGreaterStep(this, mFallTime))
        al::startNerveAction(this, "End");
}

void AppearSwitchFallMapParts::exeEnd() {
    if (al::isFirstStep(this)) {
        al::tryStartAction(this, "End");
        al::hideModelIfShow(this);
        al::invalidateCollisionParts(this);
        al::setVelocityZero(this);

        if (mIsInvalidAutoRestart) {
            kill();

            return;
        }
    }

    if (al::isGreaterStep(this, 120)) {
        al::setTrans(this, mPos);
        al::resetPosition(this);
        al::showModelIfHide(this);
        al::startNerveAction(this, "Appear");
    }
}
