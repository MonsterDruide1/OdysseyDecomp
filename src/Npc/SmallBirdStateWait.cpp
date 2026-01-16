#include "Npc/SmallBirdStateWait.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"

namespace {
NERVE_IMPL(SmallBirdStateWait, Wait);
NERVE_IMPL(SmallBirdStateWait, Turn);

NERVES_MAKE_NOSTRUCT(SmallBirdStateWait, Wait, Turn);
}  // namespace

SmallBirdStateWait::SmallBirdStateWait(al::LiveActor* actor, const al::ActorInitInfo& info)
    : BirdStateWaitBase("小鳥待機", actor) {
    initNerve(&Wait, 0);
    mInitialFront.set(al::getFront(mActor));
    al::tryGetArg(&mTurnRange, info, "TurnRangeDegree");
}

void SmallBirdStateWait::appear() {
    BirdStateWaitBase::appear();
    al::setNerve(this, &Wait);
}

void SmallBirdStateWait::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(mActor, al::isHalfProbability() ? "GroundWaitA" : "GroundWaitB");
    if (al::isActionEnd(mActor))
        al::setNerve(this, &Turn);
}

void SmallBirdStateWait::exeTurn() {
    if (al::isFirstStep(this)) {
        al::startAction(mActor, "Turn");
        sead::Vector3f target_dir = {0.0f, 0.0f, 0.0f};
        if (mTurnRange >= 0.0f) {
            al::rotateVectorDegree(
                &target_dir, mInitialFront, -al::getGravity(mActor),
                al::getRandom(sead::Mathf::clampMax(mTurnRange, 15.0f), mTurnRange) *
                    (al::isHalfProbability() ? -1.0f : 1.0f));
        } else {
            al::rotateVectorDegree(&target_dir, al::getFront(mActor), -al::getGravity(mActor),
                                   al::getRandom(15.0f, 180.0f) *
                                       (al::isHalfProbability() ? -1.0f : 1.0f));
        }
        al::normalize(&target_dir);
        mTurnAnglePerStep =
            al::normalize(1.0f, 0.0f, al::getActionFrameMax(mActor, "Turn") - 1.0f) *
            al::calcAngleOnPlaneDegree(al::getFront(mActor), target_dir, -al::getGravity(mActor));
    }
    al::rotateVectorDegree(al::getFrontPtr(mActor), -al::getGravity(mActor), mTurnAnglePerStep);
    al::normalize(al::getFrontPtr(mActor));
    if (al::isActionEnd(mActor))
        al::setNerve(this, &Wait);
}
