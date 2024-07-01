#include "Player/PlayerStateDamageLife.h"
#include "CUSTOM/PlayerAnimator.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/VectorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Player/IUsePlayerCeilingCheck.h"
#include "Player/PlayerActionAirMoveControl.h"
#include "Player/PlayerFunction.h"
#include "Player/PlayerTrigger.h"
#include "Util/PlayerCollisionUtil.h"
#include "PlayerUtil.h"
#include "Stuff.h"
#include "math/seadQuat.h"

namespace {
NERVE_IMPL(PlayerStateDamageLife, Damage);
NERVE_IMPL(PlayerStateDamageLife, Land);
NERVE_IMPL(PlayerStateDamageLife, Dead);

NERVES_MAKE_NOSTRUCT(PlayerStateDamageLife, Damage, Land, Dead);
}  // namespace

PlayerStateDamageLife::PlayerStateDamageLife(al::LiveActor* player, PlayerConst const* pConst,
                                             IUsePlayerCollision const* collider,
                                             PlayerInput const* input,
                                             IUsePlayerCeilingCheck const* ceilingCheck,
                                             PlayerAnimator* animator, PlayerTrigger* trigger)
    : al::ActorStateBase("ダメージ", player), mConst(pConst), mCollider(collider),
      mCeilingCheck(ceilingCheck), mAnimator(animator), mTrigger(trigger) {
    mActionAirMoveControl = new PlayerActionAirMoveControl(player, pConst, input, collider, true);
    initNerve(&Damage, 0);
}

void PlayerStateDamageLife::appear() {
    mIsDead = false;
    _5C = 0.0f;
    rs::calcGroundNormalOrGravityDir(&mUp, mActor, mCollider);
    mIsNoDamageDown = mTrigger->isOnNoDamageDown();

    if(PlayerFunction::isPlayerDeadStatus(mActor)) {
        al::setVelocityZero(mActor);
        al::setNerve(this, &Dead);
        return;
    }

    if(mTrigger->isOn(PlayerTrigger::EActionTrigger_val6)) {
        al::setNerve(this, &Land);
        return;
    }

    if(mTrigger->isOn(PlayerTrigger::EActionTrigger_val2)) {
        const sead::Vector3f& gravity = al::getGravity(mActor);
        sead::Vector3f velH = {0.0f, 0.0f, 0.0f};
        sead::Vector3f velV = {0.0f, 0.0f, 0.0f};
        al::separateVelocityHV(&velH, &velV, mActor);
        sead::Vector3f velDirH = {0.0f, 0.0f, 0.0f};
        if(al::tryNormalizeOrZero(&velDirH, velH)) {
            velDirH = -velDirH;
        } else {
            al::calcFrontDir(&velDirH, mActor);
        }

        sead::Quatf quat = sead::Quatf::unit;
        al::makeQuatFrontUp(&quat, velDirH, -gravity);
        al::updatePoseQuat(mActor, quat);

        mTrigger->set(PlayerTrigger::EActionTrigger_val3);
        mActionAirMoveControl->setup(1000.0f, velH.length(), 0, -velV.dot(gravity), mConst->getGravityDamage(), 9999, 1.0f);
        _5C = 0.0f;
    } else {
        if(rs::isPlayer3D(mActor)) {
            sead::Vector3f front = {0.0f, 0.0f, 0.0f};
            al::calcFrontDir(&front, mActor);
            al::setVelocity(mActor, front * mConst->getPushPowerDamage());
        }

        mActionAirMoveControl->setup(mConst->getJumpMoveSpeedMax(), mConst->getPushPowerDamage(), 0, mConst->getHopPowerDamage(), mConst->getGravityDamage(), 9999, 1.0f);
    }

    al::setNerve(this, &Damage);
}

bool PlayerStateDamageLife::isLand() const {
    return al::isNerve(this, &Land);
}

bool PlayerStateDamageLife::isEnableCancel() const {
    if(!isLand()) return false;
    return al::isGreaterEqualStep(this, mConst->getDamageCancelFrame());
}

bool PlayerStateDamageLife::isFormSquat() const {
    if(mIsDead) return false;
    return !mCeilingCheck->isEnableStandUp();
}

void PlayerStateDamageLife::exeDamage() {
    if(al::isFirstStep(this)) {
        if(mAnimator->unk2)
            mAnimator->endSubAnim();

        if(rs::isPlayer2D(mActor))
            mAnimator->startAnim("Damage");
        else if(mIsNoDamageDown)
            mAnimator->startAnim("NoDamageDown");
        else
            mAnimator->startAnim("DamageDown");
    }

    mActionAirMoveControl->update();
    if(rs::isOnGroundLessAngle(mActor, mCollider, mConst->getStandAngleMin())) {
        rs::startHitReactionLandIfLanding(mActor, mCollider, false);
        rs::brakeLandVelocityGroundNormal(mActor, &mUp, mCollider, -al::getGravity(mActor), _5C, mConst->getGravity());
        al::setNerve(this, &Land);
        return;
    }

    if(rs::isPlayer2D(mActor) || al::isGreaterEqualStep(this, 120)) {
        kill();
        return;
    }
}

void PlayerStateDamageLife::exeLand() {
    if(al::isFirstStep(this) && rs::isPlayer3D(mActor)) {
        mAnimator->startAnim("DamageLand");
    }

    if(rs::isOnGroundLessAngle(mActor, mCollider, mConst->getStandAngleMin())) {
        rs::brakeLandVelocityGroundNormal(mActor, &mUp, mCollider, mUp, _5C, mConst->getGravity());
    } else {
        al::tryAddVelocityLimit(mActor, mConst->getGravityAir() * al::getGravity(mActor), mConst->getFallSpeedMax());
    }

    if(rs::isPlayer2D(mActor))
        kill();

    if(mAnimator->isAnimEnd())
        kill();
}

void PlayerStateDamageLife::exeDead() {
    if(al::isFirstStep(this)) {
        if(rs::isPlayer2D(mActor)) {
            mAnimator->startAnim("Dead");
        } else {
            if(mAnimator->unk2)
                mAnimator->endSubAnim();

            if(mTrigger->isOn(PlayerTrigger::EActionTrigger_val6))
                mAnimator->startAnim("DeadPoison");
            else
                mAnimator->startAnimDead();

            rs::faceToCamera(mActor);
        }

        rs::slerpUp(mActor, -al::getGravity(mActor), 1.0f, 0.0f);
    }

    if(mAnimator->isAnimEnd())
        kill();
}
