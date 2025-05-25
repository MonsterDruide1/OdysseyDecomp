#include "Npc/BigBirdStateFlyAway.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/NpcAnimUtil.h"

namespace {
NERVE_IMPL(BigBirdStateFlyAway, FlyAwayPrepare);
NERVE_IMPL(BigBirdStateFlyAway, GroundReaction);
NERVE_IMPL(BigBirdStateFlyAway, FlyReaction);
NERVE_IMPL(BigBirdStateFlyAway, FlyAwayStart);
NERVE_IMPL(BigBirdStateFlyAway, FlyAway);

NERVES_MAKE_NOSTRUCT(BigBirdStateFlyAway, FlyAwayStart);
NERVES_MAKE_NOSTRUCT(BigBirdStateFlyAway, FlyAway);
NERVES_MAKE_STRUCT(BigBirdStateFlyAway, FlyAwayPrepare, GroundReaction, FlyReaction);
}  // namespace

BigBirdStateFlyAway::BigBirdStateFlyAway(al::LiveActor* actor)
    : BirdStateFlyAwayBase("大鳥の飛び去り", actor) {
    initNerve(&NrvBigBirdStateFlyAway.FlyAwayPrepare, 0);
}

void BigBirdStateFlyAway::appear() {
    BirdStateFlyAwayBase::appear();
    if (isFlyAwayByCapGround())
        al::setNerve(this, &NrvBigBirdStateFlyAway.GroundReaction);
    else if (isFlyAwayByTouchAir())
        al::setNerve(this, &NrvBigBirdStateFlyAway.FlyReaction);
    else
        al::setNerve(this, &NrvBigBirdStateFlyAway.FlyAwayPrepare);
}

void BigBirdStateFlyAway::exeGroundReaction() {
    if (al::isFirstStep(this))
        al::startAction(mActor, "GroundReaction");
    if (al::isActionEnd(mActor))
        al::setNerve(this, &NrvBigBirdStateFlyAway.FlyAwayPrepare);
}

void BigBirdStateFlyAway::exeFlyReaction() {
    if (al::isFirstStep(this)) {
        al::startAction(mActor, "FlyReaction");
        al::setVelocityZero(mActor);
    }
    if (al::isActionEnd(mActor))
        al::setNerve(this, &NrvBigBirdStateFlyAway.FlyAwayPrepare);
}

void BigBirdStateFlyAway::exeFlyAwayPrepare() {
    if (al::isFirstStep(this)) {
        al::startAction(mActor, "FlyStart");
        al::setVelocityY(mActor, 30);
    }
    al::addVelocity(mActor, al::getGravity(mActor) * 0.75f +
                                al::getFront(mActor) * al::calcNerveValue(this, 10, 0.0f, 2.0f));
    al::scaleVelocityHV(mActor, 0.97f, 0.9f);
    al::setNerveAtGreaterEqualStep(this, &FlyAwayStart, 10);
}

void BigBirdStateFlyAway::exeFlyAwayStart() {
    al::addVelocity(
        mActor,
        -al::getGravity(mActor) * 0.7f +
            al::getFront(mActor) *
                al::calcNerveValue(this, al::getActionFrameMax(mActor, "FlyStart"), 2.0f, 0.8f));
    al::scaleVelocity(mActor, 0.97f);
    if (al::isActionEnd(mActor))
        al::setNerve(this, &FlyAway);
}

void BigBirdStateFlyAway::exeFlyAway() {
    if (al::isFirstStep(this))
        al::startAction(mActor, "Fly");
    al::addVelocity(mActor, -al::getGravity(mActor) * 0.7f + al::getFront(mActor) * 0.8f);
    al::scaleVelocity(mActor, 0.97f);
    BirdFunction::tryUpdateFlyAwayDisappearDitherAlpha(mActor, this, 180, 240);
    if (al::isGreaterEqualStep(this, 240))
        kill();
}
