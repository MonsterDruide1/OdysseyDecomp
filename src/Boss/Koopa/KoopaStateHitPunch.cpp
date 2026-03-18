#include "Boss/Koopa/KoopaStateHitPunch.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Boss/Koopa/KoopaFlag.h"
#include "Boss/Koopa/KoopaFunction.h"
#include "Boss/Koopa/KoopaNumberCounter.h"

namespace {
NERVE_IMPL(KoopaStateHitPunch, Hit);
NERVE_IMPL(KoopaStateHitPunch, HitFirst);
NERVE_IMPL(KoopaStateHitPunch, HitEnd);
NERVES_MAKE_NOSTRUCT(KoopaStateHitPunch, Hit, HitFirst, HitEnd);
}  // namespace

KoopaStateHitPunch::KoopaStateHitPunch(al::LiveActor* actor, KoopaCap* cap, KoopaFlag* flag,
                                       KoopaNumberCounter* counter)
    : al::ActorStateBase("パンチヒット", actor), mCap(cap), mFlag(flag), mCounter(counter) {
    initNerve(&Hit, 0);
}

void KoopaStateHitPunch::appear() {
    NerveStateBase::appear();
    if (mFlag->tryStartPunchHitFirst())
        al::setNerve(this, &HitFirst);
    else
        al::setNerve(this, &Hit);
}

void KoopaStateHitPunch::exeHitFirst() {
    if (al::isFirstStep(this))
        al::startAction(mActor, "GuardFirst");
    KoopaFunction::updateOnGroundVelocity(mActor);
    if (al::isActionEnd(mActor))
        al::setNerve(this, &Hit);
}

void KoopaStateHitPunch::exeHit() {
    if (al::isFirstStep(this))
        mCounter->startPunchHitAction(mActor);
    KoopaFunction::updateOnGroundVelocity(mActor);
    if (al::isActionEnd(mActor))
        al::setNerve(this, &HitEnd);
}

void KoopaStateHitPunch::exeHitEnd() {
    if (al::isFirstStep(this))
        al::startAction(mActor, "GuardEnd");
    KoopaFunction::updateOnGroundVelocity(mActor);
    if (al::isActionEnd(mActor)) {
        mFlag->setHasHitEnd();
        mCounter->recoverPunchHitCount(mCap);
        al::setVelocityZero(mActor);
        kill();
    }
}
