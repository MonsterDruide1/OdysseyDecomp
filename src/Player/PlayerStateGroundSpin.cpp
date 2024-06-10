#include "Player/PlayerStateGroundSpin.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/MathLengthUtil.h"
#include "Stuff.h"

namespace {
NERVE_IMPL(PlayerStateGroundSpin, GroundSpin);

NERVE_MAKE(PlayerStateGroundSpin, GroundSpin);
}  // namespace

PlayerStateGroundSpin::PlayerStateGroundSpin(al::LiveActor* parent, const IUsePlayerCollision* collision, const PlayerInput* input,
                                             const PlayerConst* playerConst, PlayerAnimator* animator)
    : ActorStateBase("地上スピン", parent), mCollision(collision), mPlayerInput(input), mPlayerConst(playerConst), mPlayerAnimator(animator) {
    mGroundMoveCtrl = new PlayerActionGroundMoveControl(parent, playerConst, input, collision);
    mGroundMoveCtrl->_C4 = true;
    mGroundMoveCtrl->setup(0.0f, 0.0f, 0, 0, 0, 0.0f, 0.0f, 0);
    initNerve(&GroundSpin, 0);
}

PlayerStateGroundSpin::~PlayerStateGroundSpin() = default;

void PlayerStateGroundSpin::appear() {
    mIsDead = false;
    mGroundMoveCtrl->appear();
    mIsSpinClockwise = mPlayerInput->isSpinClockwise();
    al::setNerve(this, &GroundSpin);
}

void PlayerStateGroundSpin::exeGroundSpin() {
    if (al::isFirstStep(this))
        mPlayerAnimator->startAnim(mIsSpinClockwise ? "SpinGroundR" : "SpinGroundL");

    sead::Vector3f velocity = {0.0f, 0.0f, 0.0f};
    mGroundMoveCtrl->updateNormalAndSnap(&velocity);

    sead::Vector3f input = {0.0f, 0.0f, 0.0f};
    mPlayerInput->calcMoveInput(&input, mGroundMoveCtrl->mGravityDir);

    velocity *= mPlayerConst->getGroundSpinBrakeRate();
    f32 velMaxLength = sead::Mathf::max(velocity.length(), mPlayerConst->getGroundSpinMoveSpeedMax());
    velocity += mPlayerConst->getGroundSpinAccelRate() * input;
    al::limitLength(&velocity, velocity, velMaxLength);

    al::setVelocity(mActor, velocity - mGroundMoveCtrl->mGravityDir * mPlayerConst->getGravityMove());

    sead::Vector3f frontDir = {0.0f, 0.0f, 0.0f};
    if (!al::tryNormalizeOrZero(&frontDir, velocity))
        al::calcFrontDir(&frontDir, mActor);

    rs::slerpUpFront(mActor, sead::Vector3f::ey, frontDir, mPlayerConst->getSlerpQuatRate(), 0.0);

    if (!al::isLessStep(this, mPlayerConst->getGroundSpinFrame()))
        kill();
}
