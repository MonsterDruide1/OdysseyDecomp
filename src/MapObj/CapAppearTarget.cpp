#include "MapObj/CapAppearTarget.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Player/PlayerUtil.h"
#include "Library/Stage/StageSwitchUtil.h"

#include "MapObj/CapAppearMapParts.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(CapAppearTarget, Wait);
NERVE_IMPL(CapAppearTarget, Appear);
NERVE_IMPL(CapAppearTarget, Disappear);
NERVE_IMPL(CapAppearTarget, Reaction);
NERVE_IMPL(CapAppearTarget, Dead);

NERVES_MAKE_NOSTRUCT(CapAppearTarget, Appear, Dead);

struct {
    CapAppearTargetNrvWait Wait;
    CapAppearTargetNrvReaction Reaction;
    CapAppearTargetNrvDisappear Disappear;
} NrvCapAppearTarget;
}  // namespace

CapAppearTarget::CapAppearTarget(const char* name, CapAppearMapParts* mapParts)
    : al::LiveActor(name), mMapParts(mapParts) {}

void CapAppearTarget::init(const al::ActorInitInfo& info) {
    al::initChildActorWithArchiveNameNoPlacementInfo(this, info, "CapAppearTarget", nullptr);
    al::initNerve(this, &NrvCapAppearTarget.Wait, 0);

    if (al::isValidStageSwitch(mMapParts, "SwitchAppear"))
        makeActorDead();
    else
        makeActorAlive();
}

void CapAppearTarget::appear() {
    al::LiveActor::appear();
    al::startAction(this, "Appear");
    calcAnim();
    al::startHitReactionHitEffect(this, "出現", al::getTrans(this));
    al::invalidateClipping(this);
    al::setNerve(this, &Appear);
}

void CapAppearTarget::switchAppear() {
    al::LiveActor::appear();
    al::startAction(this, "Appear");
    al::setNerve(this, &Appear);
}

bool CapAppearTarget::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                 al::HitSensor* self) {
    if (rs::isMsgCapAttack(message) && (al::isNerve(this, &NrvCapAppearTarget.Wait) ||
                                        al::isNerve(this, &NrvCapAppearTarget.Disappear))) {
        al::startHitReactionHitEffect(this, "攻撃された時", al::getTrans(this));
        mMapParts->appear();
        al::setNerve(this, &NrvCapAppearTarget.Reaction);
        return true;
    }

    if ((al::isMsgPlayerObjTouch(message) || al::isMsgKickStoneAttack(message)) &&
        mIsValidTouchReaction && al::isNerve(this, &NrvCapAppearTarget.Wait)) {
        al::invalidateClipping(this);
        al::setNerve(this, &NrvCapAppearTarget.Disappear);
        return true;
    }

    return false;
}

void CapAppearTarget::exeWait() {
    if (al::isFirstStep(this)) {
        al::validateClipping(this);
        al::startAction(this, "Wait");
    }

    if (!mIsValidTouchReaction && !al::isNearPlayer(this, 300.0f))
        mIsValidTouchReaction = true;
}

void CapAppearTarget::exeReaction() {
    if (al::isFirstStep(this))
        al::startAction(this, "Reaction");

    if (al::isActionEnd(this)) {
        al::setNerve(this, &NrvCapAppearTarget.Wait);
        mIsValidTouchReaction = false;
    }
}

void CapAppearTarget::exeAppear() {
    if (al::isActionEnd(this))
        al::setNerve(this, &NrvCapAppearTarget.Wait);
}

void CapAppearTarget::exeDisappear() {
    if (al::isFirstStep(this))
        al::startAction(this, "DisAppear");

    if (al::isActionEnd(this))
        al::setNerve(this, &Dead);
}

void CapAppearTarget::exeDead() {
    if (al::isFirstStep(this))
        kill();
}
