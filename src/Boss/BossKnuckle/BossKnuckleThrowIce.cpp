#include "Boss/BossKnuckle/BossKnuckleThrowIce.h"

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

NERVES_MAKE_STRUCT(BossKnuckleThrowIce, Break, Fly, BlowDown);
NERVES_MAKE_NOSTRUCT(BossKnuckleThrowIce, Wait, Appear);

}  // namespace

BossKnuckleThrowIce::BossKnuckleThrowIce(const char* name) : al::LiveActor(name) {}

void BossKnuckleThrowIce::init(const al::ActorInitInfo& initInfo) {
    al::initActorWithArchiveName(this, initInfo, "BossKnuckleThrowIce", nullptr);
    al::initNerve(this, &Wait, 0);
    makeActorDead();
}

void BossKnuckleThrowIce::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (!al::isNerve(this, &NrvBossKnuckleThrowIce.Break) &&
        al::isNerve(this, &NrvBossKnuckleThrowIce.Fly) && al::sendMsgEnemyAttack(other, self)) {
        al::faceToSensor(this, other);
        al::setNerve(this, &NrvBossKnuckleThrowIce.BlowDown);
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
    al::calcTransLocalOffsetByMtx(&trans, *al::getJointMtxPtr(hand, "Wrist"),
                                  sead::Vector3f::zero + sead::Vector3f(300.0f, -150.0f, 0.0f));
    al::setTrans(this, trans);

    sead::Vector3f frontDir;
    al::calcFrontDir(&frontDir, hand);
    al::setFront(this, frontDir);

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
        al::setNerve(this, &NrvBossKnuckleThrowIce.Break);
    }
}

void BossKnuckleThrowIce::doThrow(const sead::Vector3f& velocity) {
    mHand = nullptr;
    mThrowVelocity.set(velocity);

    sead::Vector3f throwDirection = velocity;
    al::normalize(&throwDirection);
    al::setVelocity(this, throwDirection * 70.0f);

    al::setNerve(this, &NrvBossKnuckleThrowIce.Fly);
}

bool BossKnuckleThrowIce::isInAir() const {
    return al::isNerve(this, &NrvBossKnuckleThrowIce.Fly) ||
           al::isNerve(this, &NrvBossKnuckleThrowIce.BlowDown);
}

bool BossKnuckleThrowIce::isBreak() const {
    return al::isNerve(this, &NrvBossKnuckleThrowIce.Break);
}

void BossKnuckleThrowIce::exeAppear() {
    if (al::isFirstStep(this))
        al::startAction(this, "Appear");

    const al::LiveActor* hand = mHand;
    if (hand) {
        sead::Vector3f trans;
        al::calcTransLocalOffsetByMtx(&trans, *al::getJointMtxPtr(hand, "Wrist"),
                                      sead::Vector3f::zero + sead::Vector3f(300.0f, -150.0f, 0.0f));
        al::setTrans(this, trans);

        sead::Vector3f frontDir;
        al::calcFrontDir(&frontDir, hand);
        al::setFront(this, frontDir);
    }

    if (al::isActionEnd(this))
        al::setNerve(this, &Wait);
}

void BossKnuckleThrowIce::exeWait() {
    const al::LiveActor* hand = mHand;
    if (hand) {
        f32 offsetY = -al::getNerveStep(this) * 20.0f;
        sead::Vector3f trans;
        al::calcTransLocalOffsetByMtx(&trans, *al::getJointMtxPtr(hand, "Wrist"),
                                      sead::Vector3f(300.0f, offsetY - 150.0f, 0.0f));
        al::setTrans(this, trans);

        sead::Vector3f frontDir;
        al::calcFrontDir(&frontDir, hand);
        al::setFront(this, frontDir);
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
        al::setNerve(this, &NrvBossKnuckleThrowIce.Break);
    }
}

void BossKnuckleThrowIce::exeBreak() {
    if (al::isGreaterEqualStep(this, 90)) {
        al::validateClipping(this);
        al::deleteEffectAll(this);
        kill();
    }
}
