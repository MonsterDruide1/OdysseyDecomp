#include "Enemy/KuriboWingHackState.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Se/SeFunction.h"

#include "Player/HackerJudgeNormalFall.h"
#include "Player/HackerStateWingFly.h"
#include "Util/Hack.h"
#include "Util/JudgeUtil.h"
#include "Util/ObjUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
HackerStateWingFly::HackerStateWingFlyParam sDefaultFlyParam{};

NERVE_IMPL(KuriboWingHackState, Wait);
NERVE_IMPL(KuriboWingHackState, Fly);
NERVE_IMPL(KuriboWingHackState, Walk);
NERVE_IMPL_(KuriboWingHackState, MoveAir, Fall);
NERVE_IMPL(KuriboWingHackState, Land);
NERVE_IMPL(KuriboWingHackState, Fall);
NERVE_IMPL(KuriboWingHackState, AttackTrample);

NERVES_MAKE_STRUCT(KuriboWingHackState, Wait, Fly, Walk, MoveAir, Land, Fall, AttackTrample);

void updateVelocity(al::LiveActor* actor, const KuriboWingHackState* state,
                    const IUsePlayerHack* playerHack) {
    al::addVelocityToGravity(actor, 0.7f);
    if (al::isOnGround(actor, 0)) {
        const sead::Vector3f& groundNormal = al::getCollidedGroundNormal(actor);
        const f32 normalX = groundNormal.x;
        const f32 normalY = groundNormal.y;
        const f32 normalZ = groundNormal.z;
        const sead::Vector3f& velocity = al::getVelocity(actor);
        f32 speed = normalX * velocity.x + normalY * velocity.y + normalZ * velocity.z;
        if (speed < 0.0f) {
            f32 rebound = speed * -0.2f;
            sead::Vector3f reboundVelocity = {normalX * rebound, normalY * rebound, 0.0f};
            reboundVelocity.z = normalZ * rebound;
            al::addVelocity(actor, reboundVelocity);
        }
    }

    bool isOnMoveStick = rs::isOnHackMoveStick(playerHack);
    f32 scale;
    if (al::isOnGround(actor, 0)) {
        if (al::isCollidedFloorCode(actor, "Slide"))
            scale = 0.998f;
        else if (isOnMoveStick && !al::isNerve(state, &NrvKuriboWingHackState.Wait))
            scale = 0.6f;
        else
            scale = 0.0f;
    } else {
        scale = 0.98f;
    }

    al::scaleVelocity(actor, scale);
}

void updateMove(f32* turnSpeed, al::LiveActor* actor, const IUsePlayerHack* playerHack) {
    sead::Vector3f groundNormal = sead::Vector3f::ey;
    if (al::isOnGround(actor, 0))
        groundNormal = al::getCollidedGroundNormal(actor);

    sead::Vector3f stickAccel = {0.0f, 0.0f, 0.0f};
    f32 accel = al::isOnGround(actor, 0) ? 7.0f : 1.0f;
    rs::addHackActorAccelStick(actor, playerHack, &stickAccel, accel, groundNormal);

    if (!rs::isOnHackMoveStick(playerHack)) {
        al::turnToDirection(actor, stickAccel, *turnSpeed);
        return;
    }

    if (al::isOnGround(actor, 3)) {
        sead::Vector3f moveDir;
        if (!rs::calcHackerMoveDir(&moveDir, playerHack, sead::Vector3f::ey)) {
            *turnSpeed = 0.0f;
            al::turnToDirection(actor, stickAccel, *turnSpeed);
            return;
        }

        sead::Vector3f frontDir;
        al::calcFrontDir(&frontDir, actor);

        const f32 angle = al::calcAngleDegree(moveDir, frontDir);
        if (angle > 135.0f)
            *turnSpeed = 20.0f;
        else if (*turnSpeed > 10.0f)
            *turnSpeed = (*turnSpeed * 0.9f) + 1.0f;
        else
            *turnSpeed = 10.0f;
    } else {
        *turnSpeed = 10.0f;
    }

    al::turnToDirection(actor, stickAccel, *turnSpeed);
}
}  // namespace

KuriboWingHackState::KuriboWingHackState(al::LiveActor* actor, IUsePlayerHack** playerHack)
    : al::ActorStateBase("ハック状態", actor), mPlayerHack(playerHack) {
    initNerve(&NrvKuriboWingHackState.Wait, 1);
    mStateWingFly = new HackerStateWingFly(mActor, mPlayerHack, nullptr);
    mStateWingFly->setFlyParam(sDefaultFlyParam);
    al::initNerveState(this, mStateWingFly, &NrvKuriboWingHackState.Fly, "状態:はばたき");
    mJudgeNormalFall = new HackerJudgeNormalFall(mActor, 4);
}

void KuriboWingHackState::appear() {
    al::ActorStateBase::appear();
    rs::resetJudge(mJudgeNormalFall);
    mStateWingFly->updateFlyLimit();

    if (al::isOnGround(mActor, 0)) {
        al::setNerve(this, &NrvKuriboWingHackState.Wait);
        return;
    }

    al::setSeKeeperPlayNamePrefix(mActor, "PHack");
    mStateWingFly->setIsJudgeFall(true);
    al::setNerve(this, &NrvKuriboWingHackState.Fly);
}

void KuriboWingHackState::control() {}

void KuriboWingHackState::kill() {
    al::resetSeKeeperPlayNamePrefix(mActor);
    al::ActorStateBase::kill();
}

void KuriboWingHackState::exeWait() {
    if (al::isFirstStep(this)) {
        if (!al::isActionPlaying(mActor, "Move"))
            al::startAction(mActor, "Wait");
        rs::resetJudge(mJudgeNormalFall);
    }

    if (al::isActionPlaying(mActor, "Move")) {
        f32 frame = al::getActionFrame(mActor);
        if (frame == al::getActionFrameMax(mActor, "Move") - 1.0f)
            al::startAction(mActor, "Wait");
    }

    updateVelocity(mActor, this, *mPlayerHack);

    bool isOnMoveStick = rs::isOnHackMoveStick(*mPlayerHack);
    bool isOnGround = al::isOnGround(mActor, 0);
    if (isOnMoveStick) {
        if (isOnGround)
            al::setNerve(this, &NrvKuriboWingHackState.Walk);
        else
            al::setNerve(this, &NrvKuriboWingHackState.MoveAir);
        return;
    }

    if (isOnGround)
        mStateWingFly->updateFlyLimit();

    if (rs::updateJudgeAndResult(mJudgeNormalFall)) {
        mStateWingFly->setIsJudgeFall(true);
        al::setNerve(this, &NrvKuriboWingHackState.Fly);
        return;
    }

    IUsePlayerHack* playerHack = *mPlayerHack;
    if (rs::isTriggerHackPreInputAnyButton(playerHack) || rs::isTriggerHackSwing(playerHack))
        al::setNerve(this, &NrvKuriboWingHackState.Fly);
}

void KuriboWingHackState::exeWalk() {
    if (al::isFirstStep(this))
        al::tryStartActionIfNotPlaying(mActor, "Walk");

    updateMove(&mTurnSpeed, mActor, *mPlayerHack);
    updateVelocity(mActor, this, *mPlayerHack);

    if (rs::updateJudgeAndResult(mJudgeNormalFall)) {
        mStateWingFly->setIsJudgeFall(true);
        al::setNerve(this, &NrvKuriboWingHackState.Fly);
        return;
    }

    if (al::isOnGround(mActor, 0))
        mStateWingFly->updateFlyLimit();

    if (!rs::isOnHackMoveStick(*mPlayerHack)) {
        al::setNerve(this, &NrvKuriboWingHackState.Wait);
        return;
    }

    IUsePlayerHack* playerHack = *mPlayerHack;
    if (rs::isTriggerHackPreInputAnyButton(playerHack) || rs::isTriggerHackSwing(playerHack))
        al::setNerve(this, &NrvKuriboWingHackState.Fly);
}

void KuriboWingHackState::exeLand() {
    if (al::isFirstStep(this)) {
        al::setVelocityZero(mActor);
        al::startAction(mActor, "Land");
    }

    updateVelocity(mActor, this, *mPlayerHack);

    if (al::isGreaterStep(this, 10) && rs::isOnHackMoveStick(*mPlayerHack)) {
        al::setNerve(this, &NrvKuriboWingHackState.Walk);
        return;
    }

    if (al::isOnGround(mActor, 0))
        mStateWingFly->updateFlyLimit();

    IUsePlayerHack* playerHack = *mPlayerHack;
    if (rs::isTriggerHackPreInputAnyButton(playerHack) || rs::isTriggerHackSwing(playerHack)) {
        al::setNerve(this, &NrvKuriboWingHackState.Fly);
        return;
    }

    if (al::isActionEnd(mActor))
        al::setNerve(this, &NrvKuriboWingHackState.Wait);
}

void KuriboWingHackState::exeFly() {
    if (al::updateNerveState(this)) {
        if (rs::isOnHackMoveStick(*mPlayerHack))
            al::setNerve(this, &NrvKuriboWingHackState.Walk);
        else
            al::setNerve(this, &NrvKuriboWingHackState.Land);
    }
}

void KuriboWingHackState::exeFall() {
    if (al::isFirstStep(this)) {
        al::LiveActor* actor = mActor;
        const char* actionName =
            al::isNerve(this, &NrvKuriboWingHackState.MoveAir) ? "Move" : "Fall";
        al::startAction(actor, actionName);
    }

    updateMove(&mTurnSpeed, mActor, *mPlayerHack);
    updateVelocity(mActor, this, *mPlayerHack);

    IUsePlayerHack* playerHack = *mPlayerHack;
    if (rs::isTriggerHackPreInputAnyButton(playerHack) || rs::isTriggerHackSwing(playerHack)) {
        al::setNerve(this, &NrvKuriboWingHackState.Fly);
        return;
    }

    bool isOnGround = al::isOnGround(mActor, 0);
    bool isMoveAir = al::isNerve(this, &NrvKuriboWingHackState.MoveAir);
    if (isOnGround) {
        const al::Nerve* walkNerve = &NrvKuriboWingHackState.Walk;
        const al::Nerve* landNerve = &NrvKuriboWingHackState.Land;
        const al::Nerve* nerve = isMoveAir ? walkNerve : landNerve;
        al::setNerve(this, nerve);
        return;
    }

    if (isMoveAir) {
        if (!rs::isOnHackMoveStick(*mPlayerHack))
            al::setNerve(this, &NrvKuriboWingHackState.Fall);
    } else if (al::isNerve(this, &NrvKuriboWingHackState.Fall)) {
        if (rs::isOnHackMoveStick(*mPlayerHack))
            al::setNerve(this, &NrvKuriboWingHackState.MoveAir);
    }
}

void KuriboWingHackState::exeAttackTrample() {
    if (al::isFirstStep(this)) {
        al::startAction(mActor, "AttackTrample");
        al::setVelocityZeroV(mActor);
        al::addVelocityJump(mActor, 20.0f);
    }

    updateMove(&mTurnSpeed, mActor, *mPlayerHack);
    updateVelocity(mActor, this, *mPlayerHack);

    if (al::isLessStep(this, 8))
        return;

    IUsePlayerHack* playerHack = *mPlayerHack;
    if (rs::isTriggerHackPreInputAnyButton(playerHack) || rs::isTriggerHackSwing(playerHack)) {
        al::setNerve(this, &NrvKuriboWingHackState.Fly);
        return;
    }

    if (al::isActionEnd(mActor)) {
        al::setNerve(this, &NrvKuriboWingHackState.Fall);
        return;
    }

    if (al::isCollidedGround(mActor)) {
        al::setVelocityZero(mActor);
        al::setNerve(this, &NrvKuriboWingHackState.Wait);
    }
}

void KuriboWingHackState::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isNerve(this, &NrvKuriboWingHackState.Fly))
        mStateWingFly->attackSensor(self, other);

    if (al::isSensorEnemyBody(self) && !al::isSensorName(self, "Head") &&
        !rs::sendMsgHackerNoReaction(*mPlayerHack, other, self) &&
        !rs::sendMsgHackAttackKick(other, self)) {
        if (al::isNerve(this, &NrvKuriboWingHackState.AttackTrample) ||
            al::isNerve(this, &NrvKuriboWingHackState.Wait) ||
            !rs::trySendMsgPlayerAttackTrample(mActor, self, other)) {
            al::sendMsgPush(other, self);
        } else {
            al::setNerve(this, &NrvKuriboWingHackState.AttackTrample);
        }
    }
}

void KuriboWingHackState::resetFlyLimit(const sead::Vector3f& flyLimit) {
    mStateWingFly->setFlyLimit(flyLimit);
}
