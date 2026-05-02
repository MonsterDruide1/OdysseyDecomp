#include "Boss/BossKnuckle/BossKnuckleThrowIce.h"

#include <new>

#include "Library/Effect/EffectSystemInfo.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Matrix/MatrixUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(BossKnuckleThrowIce, Wait);
NERVE_IMPL(BossKnuckleThrowIce, Break);
NERVE_IMPL(BossKnuckleThrowIce, Fly);
NERVE_IMPL(BossKnuckleThrowIce, BlowDown);
NERVE_IMPL(BossKnuckleThrowIce, Appear);
NERVES_MAKE_NOSTRUCT(BossKnuckleThrowIce, Wait, Break, Fly, BlowDown, Appear);
}  // namespace

BossKnuckleThrowIce::BossKnuckleThrowIce(const char* name) : al::LiveActor(name) {}

void BossKnuckleThrowIce::init(const al::ActorInitInfo& initInfo) {
    al::initActorWithArchiveName(this, initInfo, "BossKnuckleThrowIce", nullptr);
    al::initNerve(this, &Wait, 0);
    makeActorDead();
}

void BossKnuckleThrowIce::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (!al::isNerve(this, &Break) && al::isNerve(this, &Fly) &&
        al::sendMsgEnemyAttack(other, self)) {
        al::faceToSensor(this, other);
        al::setNerve(this, &BlowDown);
    }
}

void BossKnuckleThrowIce::appear() {
    al::showModelIfHide(this);
    al::setNerve(this, &Appear);
    al::startAction(this, "Appear");
    al::LiveActor::appear();
}

void BossKnuckleThrowIce::appearOnHand(const al::LiveActor* hand) {
    mHand = hand;

    sead::Vector3f trans;
    const sead::Matrix34f* wristMtx = al::getJointMtxPtr(hand, "Wrist");
    sead::Vector3f localOffset = sead::Vector3f::zero + sead::Vector3f(300.0f, -150.0f, 0.0f);
    al::calcTransLocalOffsetByMtx(&trans, *wristMtx, localOffset);
    al::setTrans(this, trans);
    al::calcFrontDir(&localOffset, hand);
    al::setFront(this, localOffset);
    al::invalidateClipping(this);
    al::showModelIfHide(this);
    al::setNerve(this, &Appear);
    al::startAction(this, "Appear");
    al::LiveActor::appear();
}

void BossKnuckleThrowIce::doBreak() {
    if (al::isAlive(this)) {
        al::hideModelIfShow(this);
        al::setVelocityZero(this);
        al::startAction(this, "Wait");
        al::startHitReaction(this, "全消滅");
        al::setNerve(this, &Break);
    }
}

void BossKnuckleThrowIce::doThrow(const sead::Vector3f& velocity) {
    mHand = nullptr;
    new (&mThrowVelocity) sead::Vector3f(velocity);

    sead::Vector3f throwDirection = velocity;
    al::normalize(&throwDirection);
    sead::Vector3f throwVelocity = throwDirection * 70.0f;
    al::setVelocity(this, throwVelocity);
    al::setNerve(this, &Fly);
}

bool BossKnuckleThrowIce::isInAir() const {
    if (al::isNerve(this, &Fly))
        return true;
    return al::isNerve(this, &BlowDown);
}

bool BossKnuckleThrowIce::isBreak() const {
    return al::isNerve(this, &Break);
}

void BossKnuckleThrowIce::exeAppear() {
    if (al::isFirstStep(this))
        al::startAction(this, "Appear");

    const al::LiveActor* hand = mHand;
    if (hand) {
        sead::Vector3f trans;
        const sead::Matrix34f* wristMtx = al::getJointMtxPtr(hand, "Wrist");
        sead::Vector3f localOffset = sead::Vector3f::zero + sead::Vector3f(300.0f, -150.0f, 0.0f);
        al::calcTransLocalOffsetByMtx(&trans, *wristMtx, localOffset);
        al::setTrans(this, trans);
        al::calcFrontDir(&localOffset, hand);
        al::setFront(this, localOffset);
    }

    if (al::isActionEnd(this))
        al::setNerve(this, &Wait);
}

void BossKnuckleThrowIce::exeWait() {
    const al::LiveActor* hand = mHand;
    if (hand) {
        f32 offsetY = -al::getNerveStep(this) * 20.0f;
        const sead::Matrix34f* wristMtx = al::getJointMtxPtr(hand, "Wrist");

        sead::Vector3f trans;
        sead::Vector3f localOffset = {300.0f, offsetY + -150.0f, 0.0f};
        al::calcTransLocalOffsetByMtx(&trans, *wristMtx, localOffset);
        al::setTrans(this, trans);
        al::calcFrontDir(&localOffset, hand);
        al::setFront(this, localOffset);
    }
}

void BossKnuckleThrowIce::exeFly() {
    if (al::isFirstStep(this))
        al::startAction(this, "Throw");

    sead::Vector3f velocity = al::getVelocity(this);
    al::lerpVec(&velocity, velocity, mThrowVelocity, 0.08f);
    al::setVelocity(this, velocity);
}

void BossKnuckleThrowIce::exeBlowDown() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "BlowDown");
        al::setVelocityToFront(this, -70.0f);
        al::addVelocityY(this, 80.0f);
    }

    al::addVelocityToGravity(this, 2.0f);
    al::scaleVelocity(this, 0.98f);
    if (al::isActionEnd(this)) {
        al::startHitReaction(this, "消滅");
        al::hideModelIfShow(this);
        al::setVelocityZero(this);
        al::setNerve(this, &Break);
    }
}

void BossKnuckleThrowIce::exeBreak() {
    if (al::isGreaterEqualStep(this, 90)) {
        al::validateClipping(this);
        al::deleteEffectAll(this);
        kill();
    }
}
