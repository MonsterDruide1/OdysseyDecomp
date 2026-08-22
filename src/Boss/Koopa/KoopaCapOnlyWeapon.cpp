#include "Boss/Koopa/KoopaCapOnlyWeapon.h"

#include "Library/Joint/JointControllerKeeper.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Boss/Koopa/KoopaCapScaleAnimator.h"
#include "Boss/Koopa/KoopaCapStateSpinThrow.h"
#include "Boss/Koopa/KoopaFunction.h"
#include "Boss/Koopa/KoopaItemHolder.h"
#include "Player/PlayerPushReceiver.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(KoopaCapOnlyWeapon, SpinThrow)

NERVE_IMPL(KoopaCapOnlyWeapon, WaitHoverDelay)
NERVE_IMPL(KoopaCapOnlyWeapon, WaitHoverStart)
NERVE_IMPL(KoopaCapOnlyWeapon, WaitHover)

NERVES_MAKE_NOSTRUCT(KoopaCapOnlyWeapon, SpinThrow)
NERVES_MAKE_STRUCT(KoopaCapOnlyWeapon, WaitHoverDelay, WaitHoverStart, WaitHover)
}  // namespace

KoopaCapOnlyWeapon::KoopaCapOnlyWeapon(const char* name) : al::LiveActor(name) {}

void KoopaCapOnlyWeapon::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "KoopaCapDummy", nullptr);
    al::initNerve(this, &SpinThrow, 1);
    al::createAndSetColliderSpecialPurpose(this, "MoveLimit");

    mStateSpinThrow = new KoopaCapStateSpinThrow(this);
    al::initNerveState(this, mStateSpinThrow, &SpinThrow, "スピン投げ");
    al::initJointControllerKeeper(this, 1);
    KoopaFunction::initCapJointSideRotator(this, &mCapSideRotator);

    mScaleAnimator = new KoopaCapScaleAnimator(this);
    mPlayerPushReceiver = new PlayerPushReceiver(this);
    makeActorDead();
}

void KoopaCapOnlyWeapon::appear() {
    al::LiveActor::appear();
    al::startHitReaction(this, "出現");
    mCapSideRotator = 0.0f;
}

void KoopaCapOnlyWeapon::kill() {
    al::LiveActor::kill();
    mPlayerPushReceiver->clear();
    mScaleAnimator->reset();
}

void KoopaCapOnlyWeapon::disappear() {
    al::startHitReaction(this, "間接消滅");
    kill();
}

void KoopaCapOnlyWeapon::control() {
    mScaleAnimator->update(this);
}

void KoopaCapOnlyWeapon::updateCollider() {
    sead::Vector3f pushedVelocity = {0.0f, 0.0f, 0.0f};
    mPlayerPushReceiver->calcPushedVelocity(&pushedVelocity, al::getVelocity(this));

    sead::Vector3f velocity = al::getVelocity(this);
    al::setVelocity(this, pushedVelocity);
    al::LiveActor::updateCollider();
    al::setVelocity(this, velocity);
    mPlayerPushReceiver->clear();
}

void KoopaCapOnlyWeapon::startWaitHover(s32 delay) {
    if (al::isDead(this))
        appear();

    al::hideModelIfShow(this);

    if (delay >= 1) {
        mWaitHoverDelay = delay;
        al::setNerve(this, &NrvKoopaCapOnlyWeapon.WaitHoverDelay);
        return;
    }

    al::setNerve(this, &NrvKoopaCapOnlyWeapon.WaitHoverStart);
}

void KoopaCapOnlyWeapon::startSpinThrowChase(const sead::Vector3f* targetTrans, f32 degree) {
    if (al::isDead(this))
        appear();

    al::invalidateClipping(this);
    mStateSpinThrow->startSpinThrowChase(targetTrans, degree, false);
    al::setNerve(this, &SpinThrow);
}

bool KoopaCapOnlyWeapon::isEndWaitHoverStart() const {
    return !al::isNerve(this, &NrvKoopaCapOnlyWeapon.WaitHoverDelay) &&
           !al::isNerve(this, &NrvKoopaCapOnlyWeapon.WaitHoverStart);
}

bool KoopaCapOnlyWeapon::isWaitHover() const {
    return !isEndWaitHoverStart() || al::isNerve(this, &NrvKoopaCapOnlyWeapon.WaitHover);
}

void KoopaCapOnlyWeapon::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isSensorEnemyAttack(self)) {
        if (isWaitHover())
            al::sendMsgEnemyAttack(other, self);
        else
            rs::sendMsgKoopaCapSpinAttack(other, self);
    }
}

bool KoopaCapOnlyWeapon::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                    al::HitSensor* self) {
    if (al::isSensorEnemyBody(self)) {
        if (isWaitHover()) {
            if (rs::isMsgCapReflect(message)) {
                rs::requestHitReactionToAttacker(message, self, other);
                mScaleAnimator->startWaitHoverScaling();
                return true;
            }
        } else if (rs::isMsgCapAttack(message)) {
            if (mItemHolder)
                mItemHolder->tryGenerateLifeUpItem(this);

            rs::requestHitReactionToAttacker(message, self, other);
            al::startHitReaction(this, "消滅");
            kill();
            return true;
        }
    }

    return false;
}

void KoopaCapOnlyWeapon::exeWaitHoverDelay() {
    al::setNerveAtGreaterEqualStep(this, &NrvKoopaCapOnlyWeapon.WaitHoverStart, mWaitHoverDelay);
}

void KoopaCapOnlyWeapon::exeWaitHoverStart() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "WaitHoverStart");
        al::showModelIfHide(this);
    }

    al::setNerveAtActionEnd(this, &NrvKoopaCapOnlyWeapon.WaitHover);
}

void KoopaCapOnlyWeapon::exeWaitHover() {
    if (al::isFirstStep(this))
        al::startAction(this, "WaitHover");
}

void KoopaCapOnlyWeapon::exeSpinThrow() {
    if (al::isFirstStep(this))
        al::startAction(this, "Spin");

    if (al::updateNerveState(this)) {
        al::startHitReaction(this, "間接消滅");
        kill();
    }
}
