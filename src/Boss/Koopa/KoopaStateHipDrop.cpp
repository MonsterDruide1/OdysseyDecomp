#include "Boss/Koopa/KoopaStateHipDrop.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Boss/Koopa/KoopaFlag.h"
#include "Boss/Koopa/KoopaJumpMovement.h"
#include "Boss/Koopa/KoopaLandPointHolder.h"
#include "Boss/Koopa/KoopaRingBeamEmitter.h"
#include "Util/PlayerUtil.h"

namespace {
NERVE_IMPL(KoopaStateHipDrop, JumpStart);
NERVE_IMPL(KoopaStateHipDrop, HipDropLandStart);
NERVE_IMPL(KoopaStateHipDrop, HipDropLand);
NERVE_IMPL(KoopaStateHipDrop, HipDropLandEnd);
NERVE_IMPL(KoopaStateHipDrop, HipDropSign);
NERVE_IMPL(KoopaStateHipDrop, Jump);
NERVE_IMPL(KoopaStateHipDrop, HipDropStart);
NERVE_IMPL(KoopaStateHipDrop, HipDrop);
NERVES_MAKE_NOSTRUCT(KoopaStateHipDrop, JumpStart, HipDropStart, HipDropLandStart, HipDropLand,
                     HipDropLandEnd, HipDropSign, Jump, HipDrop);
}  // namespace

KoopaStateHipDrop::KoopaStateHipDrop(al::LiveActor* actor, KoopaFlag* flag,
                                     KoopaRingBeamEmitter* ringBeamEmitter)
    : al::ActorStateBase("ヒップドロップ", actor), mFlag(flag), mRingBeamEmitter(ringBeamEmitter) {
    initNerve(&JumpStart, 0);
    mJumpMovement = new KoopaJumpMovement();
}

void KoopaStateHipDrop::decideEitherFarSidePointAndStart(KoopaLandPointHolder* landPointHolder) {
    landPointHolder->decidePointEitherFarSide(rs::getPlayerPos(mActor));
    const sead::Vector3f& currentPointTrans = landPointHolder->getCurrentPointTrans();
    f32 targetTransX = currentPointTrans.x + 0.0f;
    f32 targetTransY = currentPointTrans.y + 800.0f;
    f32 targetTransZ = currentPointTrans.z + 0.0f;
    KoopaJumpMovement* jumpMovement = mJumpMovement;
    al::LiveActor* actor = mActor;
    sead::Vector3f targetTrans = {targetTransX, targetTransY, targetTransZ};
    const sead::Quatf& targetQuat = landPointHolder->getCurrentPointQuat();
    sead::Vector3f up = -al::getGravity(actor);
    jumpMovement->start(actor, targetTrans, targetQuat, up);
    al::setNerve(this, &JumpStart);
}

bool KoopaStateHipDrop::isAfterHipDropLand() const {
    return al::isNerve(this, &HipDropLandStart) || al::isNerve(this, &HipDropLand) ||
           al::isNerve(this, &HipDropLandEnd);
}

void KoopaStateHipDrop::exeJumpStart() {
    if (al::isFirstStep(this))
        al::startAction(mActor, "JumpStart");

    if (mJumpMovement->move(mActor, false)) {
        al::setVelocityZero(mActor);
        al::setNerve(this, &HipDropSign);
    } else if (al::isActionEnd(mActor)) {
        al::setNerve(this, &Jump);
    }
}

void KoopaStateHipDrop::exeJump() {
    if (al::isFirstStep(this))
        al::startAction(mActor, "Jump");

    if (mJumpMovement->move(mActor, false)) {
        al::setVelocityZero(mActor);
        al::setNerve(this, &HipDropSign);
    }
}

void KoopaStateHipDrop::exeHipDropSign() {
    if (al::isFirstStep(this))
        al::startAction(mActor, "HipDropSign");

    if (al::isActionEnd(mActor))
        al::setNerve(this, &HipDropStart);
}

void KoopaStateHipDrop::exeHipDropStart() {
    if (al::isFirstStep(this)) {
        al::startAction(mActor, "HipDropStart");
        al::setVelocityToGravity(mActor, 150.0f);
        if (al::isNoCollide(mActor))
            al::onCollide(mActor);
    }

    al::addVelocityToGravity(mActor, 2.0f);
    al::scaleVelocity(mActor, 0.99f);

    if (al::isOnGround(mActor, 0)) {
        al::setVelocityZero(mActor);
        al::setNerve(this, &HipDropLandStart);
    } else if (al::isActionEnd(mActor)) {
        al::setNerve(this, &HipDrop);
    }
}

void KoopaStateHipDrop::exeHipDrop() {
    if (al::isFirstStep(this))
        al::startAction(mActor, "HipDrop");

    al::addVelocityToGravity(mActor, 2.0f);
    al::scaleVelocity(mActor, 0.99f);

    if (al::isOnGround(mActor, 0)) {
        al::setVelocityZero(mActor);
        al::setNerve(this, &HipDropLandStart);
    }
}

void KoopaStateHipDrop::exeHipDropLandStart() {
    if (al::isFirstStep(this)) {
        al::startAction(mActor, "HipDropLandStart");
        al::startHitReaction(mActor, "ヒップドロップ着地");
        mRingBeamEmitter->emitHipDrop(al::getTrans(mActor));
        mRingBeamEmitter->emitHipDrop(al::getTrans(mActor));
        mRingBeamEmitter->emitHipDrop(al::getTrans(mActor));
    }

    if (al::isActionEnd(mActor))
        al::setNerve(this, &HipDropLand);
}

void KoopaStateHipDrop::exeHipDropLand() {
    if (al::isFirstStep(this))
        al::startAction(mActor, "HipDropLand");

    if (al::isActionEnd(mActor))
        al::setNerve(this, &HipDropLandEnd);
}

void KoopaStateHipDrop::exeHipDropLandEnd() {
    if (al::isFirstStep(this))
        al::startAction(mActor, "HipDropLandEnd");

    if (al::isActionEnd(mActor)) {
        mFlag->setHasHipDropLandEnd();
        kill();
    }
}

void KoopaStateHipDrop::appear() {
    NerveStateBase::appear();
    al::setNerve(this, &JumpStart);
}

void KoopaStateHipDrop::kill() {
    NerveStateBase::kill();
}
