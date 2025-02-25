#include "Enemy/EnemyStateWander.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(EnemyStateWander, Wait);
NERVE_IMPL(EnemyStateWander, Walk);
NERVE_IMPL(EnemyStateWander, Fall);

NERVES_MAKE_STRUCT(EnemyStateWander, Wait, Walk, Fall);
}  // namespace

EnemyStateWander::EnemyStateWander(al::LiveActor* actor, const char* stateName)
    : al::ActorStateBase("さんぽ状態", actor), mStateName(stateName) {
    initNerve(&NrvEnemyStateWander.Wait, 0);
}

void EnemyStateWander::appear() {
    setDead(false);
    al::setNerve(this, &NrvEnemyStateWander.Walk);
}

void EnemyStateWander::exeWait() {
    if (al::isFirstStep(this)) {
        al::startAction(mActor, "Wait");
        mMaxWaitTime = al::getRandom(60);
    }
    if (al::isGreaterEqualStep(this, mMaxWaitTime + 120)) {
        al::setNerve(this, &NrvEnemyStateWander.Walk);
        return;
    }
    if (!al::isOnGround(mActor, 0)) {
        al::setNerve(this, &NrvEnemyStateWander.Fall);
        return;
    }
    if (al::isFallNextMove(mActor, al::getVelocity(mActor), 50.0f, 200.0f)) {
        al::scaleVelocity(mActor, -1.0f);
        al::setVelocityY(mActor, 0.0f);
    }
    al::scaleVelocity(mActor, 0.7f);
    al::addVelocityToGravityFittedGround(mActor, 2.0f, 0);
}

void EnemyStateWander::exeWalk() {
    if (al::isFirstStep(this)) {
        al::startAction(mActor, mStateName);
        mMaxWaitTime = al::getRandom(50);
        mIsSwapRotation = al::isHalfProbability();
    }
    if (al::isGreaterEqualStep(this, mMaxWaitTime + 100)) {
        al::setNerve(this, &NrvEnemyStateWander.Wait);
        return;
    }
    if (!al::isOnGround(mActor, 0)) {
        al::setNerve(this, &NrvEnemyStateWander.Fall);
        return;
    }

    if (mIsSwapRotation)
        al::rotateQuatYDirDegree(mActor, 1.0f);
    else
        al::rotateQuatYDirDegree(mActor, -1.0f);

    sead::Vector3f frontDir = sead::Vector3f::zero;
    al::calcFrontDir(&frontDir, mActor);

    f32 walkSpeed = mWalkSpeed > 0.0f ? mWalkSpeed : 1.0f;

    auto* actor = mActor;

    const sead::Vector3f& velocity = al::getVelocity(actor);

    if (al::isFallNextMove(actor, walkSpeed * frontDir + velocity, 50.0f, 200.0f)) {
        al::scaleVelocity(mActor, -1.0f);
        al::setVelocityY(mActor, 0.0f);
    } else {
        al::addVelocityToDirection(mActor, frontDir, walkSpeed);
    }
    al::scaleVelocity(mActor, 0.7f);
    al::addVelocityToGravityFittedGround(mActor, 2.0f, 0);
}

void EnemyStateWander::exeFall() {
    if (al::isFirstStep(this))
        al::startAction(mActor, "Fall");
    if (!al::isOnGround(mActor, 0)) {
        al::scaleVelocity(mActor, 0.98f);
        al::addVelocityToGravity(mActor, 2.0f);
    } else
        al::setNerve(this, &NrvEnemyStateWander.Wait);
}

bool EnemyStateWander::isWait() const {
    return al::isNerve(this, &NrvEnemyStateWander.Wait);
}

bool EnemyStateWander::isWalk() const {
    return al::isNerve(this, &NrvEnemyStateWander.Walk);
}

bool EnemyStateWander::isFall() const {
    return al::isNerve(this, &NrvEnemyStateWander.Fall);
}

void EnemyStateWander::changeWalkAnim(const char* animName) {
    if (al::isActionPlaying(mActor, mStateName))
        al::startAction(mActor, animName);
    mStateName = animName;
}
