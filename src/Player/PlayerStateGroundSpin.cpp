#include "Player/PlayerStateGroundSpin.h"

#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Player/PlayerActionGroundMoveControl.h"
#include "Player/PlayerAnimator.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerInput.h"
#include "Util/ObjUtil.h"

namespace {
NERVE_IMPL(PlayerStateGroundSpin, GroundSpin);

NERVES_MAKE_STRUCT(PlayerStateGroundSpin, GroundSpin);
}  // namespace

PlayerStateGroundSpin::PlayerStateGroundSpin(al::LiveActor* parent,
                                             const IUsePlayerCollision* collision,
                                             const PlayerInput* input, const PlayerConst* pConst,
                                             PlayerAnimator* animator)
    : ActorStateBase("地上スピン", parent), mCollision(collision), mPlayerInput(input),
      mPlayerConst(pConst), mPlayerAnimator(animator) {
    mGroundMoveCtrl = new PlayerActionGroundMoveControl(parent, pConst, input, collision);
    mGroundMoveCtrl->set_c4(true);
    mGroundMoveCtrl->setup(0.0f, 0.0f, 0, 0, 0, 0.0f, 0.0f, 0);
    initNerve(&NrvPlayerStateGroundSpin.GroundSpin, 0);
}

void PlayerStateGroundSpin::appear() {
    al::ActorStateBase::appear();
    mGroundMoveCtrl->appear();
    mIsSpinClockwise = mPlayerInput->isSpinClockwise();
    al::setNerve(this, &NrvPlayerStateGroundSpin.GroundSpin);
}

void PlayerStateGroundSpin::exeGroundSpin() {
    if (al::isFirstStep(this))
        mPlayerAnimator->startAnim(mIsSpinClockwise ? "SpinGroundR" : "SpinGroundL");

    sead::Vector3f velocity = {0.0f, 0.0f, 0.0f};
    mGroundMoveCtrl->updateNormalAndSnap(&velocity);

    sead::Vector3f input = {0.0f, 0.0f, 0.0f};
    mPlayerInput->calcMoveInput(&input, mGroundMoveCtrl->getGroundNormal());

    velocity *= mPlayerConst->getGroundSpinBrakeRate();
    f32 maxSpeed = sead::Mathf::max(velocity.length(), mPlayerConst->getGroundSpinMoveSpeedMax());
    velocity += mPlayerConst->getGroundSpinAccelRate() * input;
    al::limitLength(&velocity, velocity, maxSpeed);

    al::setVelocity(mActor,
                    velocity - mGroundMoveCtrl->getGroundNormal() * mPlayerConst->getGravityMove());

    sead::Vector3f frontDir = {0.0f, 0.0f, 0.0f};
    if (!al::tryNormalizeOrZero(&frontDir, velocity))
        al::calcFrontDir(&frontDir, mActor);

    rs::slerpUpFront(mActor, sead::Vector3f::ey, frontDir, mPlayerConst->getSlerpQuatRate(), 0.0f);

    if (!al::isLessStep(this, mPlayerConst->getGroundSpinFrame()))
        kill();
}
