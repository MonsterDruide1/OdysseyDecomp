#include "Boss/Koopa/KoopaStateAttackSpinShell.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Boss/Koopa/KoopaFunction.h"
#include "Boss/Koopa/KoopaShell.h"
#include "Util/CameraUtil.h"
#include "Util/PlayerUtil.h"

namespace {
NERVE_IMPL(KoopaStateAttackSpinShell, AttackShellStart);
NERVE_IMPL(KoopaStateAttackSpinShell, AttackShell);
NERVE_IMPL(KoopaStateAttackSpinShell, AttackShellEnd);
NERVES_MAKE_NOSTRUCT(KoopaStateAttackSpinShell, AttackShellStart, AttackShell, AttackShellEnd);
}  // namespace

KoopaStateAttackSpinShell::KoopaStateAttackSpinShell(al::LiveActor* actor, KoopaShell* shell)
    : al::ActorStateBase("コウラスピン攻撃", actor), mShell(shell) {
    initNerve(&AttackShellStart, 0);
}

void KoopaStateAttackSpinShell::startOnGround() {
    mIsRecover = false;
    appear();
}

void KoopaStateAttackSpinShell::startRecover() {
    mIsRecover = true;
    appear();
}

void KoopaStateAttackSpinShell::exeAttackShellStart() {
    if (al::isFirstStep(this)) {
        al::startAction(mActor, "AttackShellStart");
        rs::setKoopaShellCameraDistanceCurve(mActor);
    }

    if (al::isActionEnd(mActor))
        al::setNerve(this, &AttackShell);
}

void KoopaStateAttackSpinShell::exeAttackShell() {
    if (al::isFirstStep(this)) {
        al::hideModelIfShow(mActor);
        al::copyPose(mShell, mActor);
        if (mIsRecover)
            mShell->appearRecover();
        else
            mShell->appear();
    }

    al::resetPosition(mActor, al::getTrans(mShell));
    if (mShell->isDeactive()) {
        al::faceToTarget(mActor, rs::getPlayerPos(mActor));
        al::showModelIfHide(mActor);
        al::startAction(mActor, "AttackShellEnd");
        if (mIsRecover) {
            KoopaFunction::startCapOnAnim(mActor);
            al::onCollide(mActor);
        }
        mShell->kill();
        al::setNerve(this, &AttackShellEnd);
    }
}

void KoopaStateAttackSpinShell::exeAttackShellEnd() {
    if (al::isFirstStep(this))
        rs::resetKoopaShellCameraDistanceCurve(mActor);

    if (al::isActionEnd(mActor))
        kill();
}

void KoopaStateAttackSpinShell::appear() {
    NerveStateBase::appear();
    al::setNerve(this, &AttackShellStart);
}
