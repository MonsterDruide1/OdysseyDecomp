#include "Enemy/EnemyStateReset.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Player/PlayerHolder.h"
#include "Library/Player/PlayerUtil.h"

#include "Enemy/EnemyCap.h"
#include "Util/Hack.h"

namespace {
NERVE_IMPL(EnemyStateReset, Wait);

NERVES_MAKE_STRUCT(EnemyStateReset, Wait);
}  // namespace

EnemyStateReset::EnemyStateReset(al::LiveActor* actor, const al::ActorInitInfo& info, EnemyCap* cap)
    : al::ActorStateBase("リセット状態", actor), mEnemyCap(cap) {
    initNerve(&NrvEnemyStateReset.Wait, 0);
    al::tryGetTrans(&mPos, info);
    al::tryGetRotate(&mRot, info);
    al::tryGetArg(&mIsRevive, info, "IsRevive");
}

void EnemyStateReset::appear() {
    al::LiveActor* actor = mActor;  // getting the actor in each function call below causes
                                    // mismatch, have to declare a variable up here for it

    setDead(false);
    if (!mIsRevive) {
        actor->kill();
        return;
    }

    al::setVelocityZero(actor);
    al::invalidateClipping(actor);
    al::hideModelIfShow(actor);
    al::stopAction(actor);
    rs::startReset(actor);

    if (!mIsInvalidateSensors)
        al::invalidateHitSensors(actor);

    al::setNerve(this, &NrvEnemyStateReset.Wait);
}

void EnemyStateReset::kill() {
    setDead(true);
    if (!mIsRevive)
        return;

    al::LiveActor* actor = mActor;

    al::validateClipping(actor);
    al::showModelIfHide(actor);
    rs::endReset(actor);
    al::restartAction(actor);
    if (!mIsInvalidateSensors)
        al::validateHitSensors(actor);
    if (rs::tryAppearEnemyCap(mEnemyCap))
        al::tryStartVisAnimIfExist(mActor, "CapOn");  // mActor is used here instead of the declared
                                                      // actor variable. Requires for match
}

void EnemyStateReset::exeWait() {
    if (al::isStep(this, 2))
        al::resetRotatePosition(mActor, mRot, mPos);
    if (al::isGreaterEqualStep(this, 2) && al::isResetablePlayerPos(mActor, mValidDistance))
        kill();
}
