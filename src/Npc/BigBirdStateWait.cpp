#include "Npc/BigBirdStateWait.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(BigBirdStateWait, WaitLeft);
NERVE_IMPL_(BigBirdStateWait, WaitLeftInit, WaitLeft);
NERVE_IMPL_(BigBirdStateWait, WaitRightInit, WaitRight);
NERVE_IMPL(BigBirdStateWait, WaitRight);
NERVE_IMPL(BigBirdStateWait, TurnLeft);
NERVE_IMPL(BigBirdStateWait, TurnRight);

NERVES_MAKE_STRUCT(BigBirdStateWait, WaitLeft, WaitLeftInit, WaitRightInit, WaitRight, TurnLeft,
                   TurnRight);
}  // namespace

BigBirdStateWait::BigBirdStateWait(al::LiveActor* actor) : BirdStateWaitBase("大鳥待機", actor) {
    initNerve(&NrvBigBirdStateWait.WaitLeft, 0);
}

static const al::Nerve* getWaitNerve() {
    if (al::isHalfProbability())
        return &NrvBigBirdStateWait.WaitLeft;
    else
        return &NrvBigBirdStateWait.WaitRight;
}

static const al::Nerve* getWaitInitNerve() {
    if (al::isHalfProbability())
        return &NrvBigBirdStateWait.WaitLeftInit;
    else
        return &NrvBigBirdStateWait.WaitRightInit;
}

void BigBirdStateWait::appear() {
    BirdStateWaitBase::appear();
    if (al::isSklAnimPlaying(mActor, 0))
        al::setNerve(this, getWaitNerve());
    else
        al::setNerve(this, getWaitInitNerve());
}

static void setRandomFrame(al::LiveActor* actor) {
    al::setActionFrame(actor,
                       sead::Mathf::floor(al::getActionFrameMax(actor, al::getActionName(actor)) *
                                          al::getRandom(0.25f, 0.75f)));
}

static f32 getTurnFrameRate(f32 angle) {
    return al::lerpValue(0.75f, 1.3f, al::normalize(sead::Mathf::abs(angle), 45.0f, 165.0f));
}

void BigBirdStateWait::exeWaitLeft() {
    if (al::isFirstStep(this)) {
        al::startAction(mActor, "GroundWait");
        if (al::isNerve(this, &NrvBigBirdStateWait.WaitLeftInit))
            setRandomFrame(mActor);
    }
    if (al::isActionEnd(mActor))
        al::setNerve(this, &NrvBigBirdStateWait.TurnLeft);
}

void BigBirdStateWait::exeTurnLeft() {
    if (al::isFirstStep(this)) {
        al::startAction(mActor, "TurnLeft");
        f32 angle = al::getRandom(45.0f, 165.0f);
        mTurnAnglePerStep =
            al::normalize(1.0f, 0.0f, al::getActionFrameMax(mActor, "TurnLeft") - 1.0f) * angle;
        al::setActionFrameRate(mActor, getTurnFrameRate(angle));
    }
    al::rotateVectorDegree(al::getFrontPtr(mActor), -al::getGravity(mActor), mTurnAnglePerStep);
    al::normalize(al::getFrontPtr(mActor));
    if (al::isActionEnd(mActor))
        al::setNerve(this, &NrvBigBirdStateWait.WaitRight);
}

void BigBirdStateWait::exeWaitRight() {
    if (al::isFirstStep(this)) {
        al::startAction(mActor, "GroundWait");
        if (al::isNerve(this, &NrvBigBirdStateWait.WaitRightInit))
            setRandomFrame(mActor);
    }
    if (al::isActionEnd(mActor))
        al::setNerve(this, &NrvBigBirdStateWait.TurnRight);
}

void BigBirdStateWait::exeTurnRight() {
    f32 max_frame = al::getActionFrameMax(mActor, "TurnRight");
    if (al::isFirstStep(this)) {
        al::startAction(mActor, "TurnRight");
        f32 angle = al::getRandom(-165.0f, -45.0f);
        mTurnAnglePerStep = al::normalize(1.0f, 0.0f, max_frame - 1.0f) * angle;
        al::setActionFrameRate(mActor, getTurnFrameRate(angle));
    }
    f32 max_angle = mTurnAnglePerStep * max_frame;
    f32 old_angle = max_angle * al::easeInOut(al::normalize(al::getNerveStep(this), 0,
                                                            static_cast<s32>(max_frame)));
    f32 new_angle = max_angle * al::easeInOut(al::normalize(al::getNerveStep(this) + 1, 0,
                                                            static_cast<s32>(max_frame)));
    al::rotateVectorDegree(al::getFrontPtr(mActor), -al::getGravity(mActor), new_angle - old_angle);
    al::normalize(al::getFrontPtr(mActor));
    if (al::isActionEnd(mActor))
        al::setNerve(this, &NrvBigBirdStateWait.WaitLeft);
}
