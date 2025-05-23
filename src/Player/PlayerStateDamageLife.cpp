#include "Player/PlayerStateDamageLife.h"

#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Player/IUsePlayerCeilingCheck.h"
#include "Player/PlayerActionAirMoveControl.h"
#include "Player/PlayerAnimator.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerFunction.h"
#include "Player/PlayerTrigger.h"
#include "Util/ObjUtil.h"
#include "Util/PlayerCollisionUtil.h"
#include "Util/PlayerUtil.h"

namespace {
NERVE_IMPL(PlayerStateDamageLife, Damage);
NERVE_IMPL(PlayerStateDamageLife, Land);
NERVE_IMPL(PlayerStateDamageLife, Dead);

NERVES_MAKE_NOSTRUCT(PlayerStateDamageLife, Damage, Land, Dead);
}  // namespace

PlayerStateDamageLife::PlayerStateDamageLife(al::LiveActor* player, const PlayerConst* pConst,
                                             const IUsePlayerCollision* collider,
                                             const PlayerInput* input,
                                             const IUsePlayerCeilingCheck* ceilingCheck,
                                             PlayerAnimator* animator, PlayerTrigger* trigger)
    : al::ActorStateBase("ダメージ", player), mConst(pConst), mCollider(collider),
      mCeilingCheck(ceilingCheck), mAnimator(animator), mTrigger(trigger) {
    mActionAirMoveControl = new PlayerActionAirMoveControl(player, pConst, input, collider, true);
    initNerve(&Damage, 0);
}

void PlayerStateDamageLife::appear() {
    ActorStateBase::appear();
    mBrakeLandUpFactor = 0.0f;
    rs::calcGroundNormalOrGravityDir(&mUp, mActor, mCollider);
    mIsNoDamageDown = mTrigger->isOnNoDamageDown();

    if (PlayerFunction::isPlayerDeadStatus(mActor)) {
        al::setVelocityZero(mActor);
        al::setNerve(this, &Dead);
        return;
    }

    if (mTrigger->isOn(PlayerTrigger::EActionTrigger_val6)) {
        al::setNerve(this, &Land);
        return;
    }

    if (mTrigger->isOn(PlayerTrigger::EPreMovementTrigger_val2)) {
        al::LiveActor* player = mActor;
        const sead::Vector3f& gravity = al::getGravity(player);
        sead::Vector3f velH = {0.0f, 0.0f, 0.0f};
        sead::Vector3f velV = {0.0f, 0.0f, 0.0f};
        al::separateVelocityHV(&velH, &velV, player);
        sead::Vector3f velDirH = {0.0f, 0.0f, 0.0f};
        if (al::tryNormalizeOrZero(&velDirH, velH))
            velDirH.negate();
        else
            al::calcFrontDir(&velDirH, player);

        sead::Quatf quat = sead::Quatf::unit;
        al::makeQuatFrontUp(&quat, velDirH, -gravity);
        al::updatePoseQuat(player, quat);

        mTrigger->set(PlayerTrigger::EActionTrigger_val3);
        mActionAirMoveControl->setup(1000.0f, velH.length(), 0, -velV.dot(gravity),
                                     mConst->getGravityDamage(), 9999, 1.0f);
        mBrakeLandUpFactor = 0.0f;
    } else {
        if (rs::isPlayer3D(mActor)) {
            sead::Vector3f front = {0.0f, 0.0f, 0.0f};
            al::calcFrontDir(&front, mActor);
            al::setVelocity(mActor, front * -mConst->getPushPowerDamage());
        }

        mActionAirMoveControl->setup(mConst->getJumpMoveSpeedMax(), mConst->getPushPowerDamage(), 0,
                                     mConst->getHopPowerDamage(), mConst->getGravityDamage(), 9999,
                                     1.0f);
    }

    al::setNerve(this, &Damage);
}

bool PlayerStateDamageLife::isLand() const {
    return al::isNerve(this, &Land);
}

bool PlayerStateDamageLife::isEnableCancel() const {
    return isLand() && al::isGreaterEqualStep(this, mConst->getDamageCancelFrame());
}

bool PlayerStateDamageLife::isFormSquat() const {
    return !isDead() && !mCeilingCheck->isEnableStandUp();
}

void PlayerStateDamageLife::exeDamage() {
    al::LiveActor* player = mActor;
    if (al::isFirstStep(this)) {
        if (mAnimator->isSubAnimPlaying())
            mAnimator->endSubAnim();

        if (rs::isPlayer2D(player))
            mAnimator->startAnim("Damage");
        else if (mIsNoDamageDown)
            mAnimator->startAnim("NoDamageDown");
        else
            mAnimator->startAnim("DamageDown");
    }

    mActionAirMoveControl->update();
    if (rs::isOnGroundLessAngle(player, mCollider, mConst->getStandAngleMin())) {
        rs::startHitReactionLandIfLanding(player, mCollider, false);
        rs::brakeLandVelocityGroundNormal(player, &mUp, mCollider, -al::getGravity(player),
                                          mBrakeLandUpFactor, mConst->getGravity());
        al::setNerve(this, &Land);
        return;
    }

    if (rs::isPlayer2D(player) || al::isGreaterEqualStep(this, 120)) {
        kill();
        return;
    }
}

void PlayerStateDamageLife::exeLand() {
    if (al::isFirstStep(this) && rs::isPlayer3D(mActor))
        mAnimator->startAnim("DamageLand");
    al::LiveActor* player = mActor;

    if (rs::isOnGroundLessAngle(player, mCollider, mConst->getStandAngleMin())) {
        rs::brakeLandVelocityGroundNormal(player, &mUp, mCollider, mUp, mBrakeLandUpFactor,
                                          mConst->getGravity());
    } else {
        al::tryAddVelocityLimit(player, al::getGravity(player) * mConst->getGravityAir(),
                                mConst->getFallSpeedMax());
    }

    if (rs::isPlayer2D(player))
        kill();

    if (mAnimator->isAnimEnd())
        kill();
}

void PlayerStateDamageLife::exeDead() {
    if (al::isFirstStep(this)) {
        al::LiveActor* player = mActor;
        if (rs::isPlayer2D(player)) {
            mAnimator->startAnim("Dead");
        } else {
            if (mAnimator->isSubAnimPlaying())
                mAnimator->endSubAnim();

            if (mTrigger->isOn(PlayerTrigger::ECollisionTrigger_val6))
                mAnimator->startAnim("DeadPoison");
            else
                mAnimator->startAnimDead();

            rs::faceToCamera(player);
        }

        rs::slerpUp(player, -al::getGravity(player), 1.0f, 0.0f);
    }

    if (mAnimator->isAnimEnd())
        kill();
}
