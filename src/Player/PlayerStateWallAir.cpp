
#include "Player/PlayerStateWallAir.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/VectorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Player/PlayerJudgePreInputJump.h"
#include "Player/PlayerStateNormalWallSlide.h"
#include "Player/PlayerWallActionHistory.h"
#include "Util/StageSceneFunction.h"
#include "math/seadQuat.h"

namespace {
NERVE_IMPL(PlayerStateWallAir, Slide)
NERVE_IMPL(PlayerStateWallAir, Jump)

NERVES_MAKE_NOSTRUCT(PlayerStateWallAir, Slide, Jump)
}  // namespace

PlayerStateWallAir::PlayerStateWallAir(al::LiveActor* player, PlayerConst const* pConst,
                                       PlayerInput const* input, PlayerTrigger const* trigger,
                                       IUsePlayerCollision* collision, IJudge* judgeWallKeep,
                                       PlayerJudgePreInputJump* judgePreInputJump,
                                       PlayerAnimator* animator,
                                       PlayerWallActionHistory* wallActionHistory,
                                       PlayerActionDiveInWater* actionDiveInWater)
    : al::ActorStateBase("空中壁接触", player), mConst(pConst), mJudgeWallKeep(judgeWallKeep),
      mJudgePreInputJump(judgePreInputJump), mCollision(collision),
      mWallActionHistory(wallActionHistory) {
    initNerve(&Slide, 2);
    mStateNormalWallSlide =
        new PlayerStateNormalWallSlide(player, mConst, input, collision, animator);
    mStateNormalWallJump = new PlayerStateNormalWallJump(player, mConst, input, collision, trigger,
                                                         animator, actionDiveInWater);
    al::initNerveState(this, mStateNormalWallSlide, &Slide, "壁すべり");
    al::initNerveState(this, mStateNormalWallJump, &Jump, "壁ジャンプ");
}

PlayerStateWallAir::~PlayerStateWallAir() = default;

void PlayerStateWallAir::appear() {
    mIsDead = false;
    al::setNerve(this, &Slide);
}

bool PlayerStateWallAir::isAir() const {
    return al::isNerve(this, &Jump) && al::isGreaterStep(this, 0);
}

bool PlayerStateWallAir::isJustJump() const {
    return al::isNerve(this, &Jump) && al::isFirstStep(this);
}

bool PlayerStateWallAir::isEnableReactionCapCatch() const {
    return !mIsDead && al::isNerve(this, &Jump);
}

void PlayerStateWallAir::startSlideSpinAttack() {
    const sead::Vector3f& gravity = al::getGravity(mActor);
    sead::Vector3f jumpPos = gravity * 20.0f + al::getTrans(mActor);
    mWallActionHistory->recordWallJump(mCollision, jumpPos);

    sead::Vector3f front = {0.0f, 0.0f, 0.0f};
    al::calcFrontDir(&front, mActor);

    sead::Quatf quat = sead::Quatf::unit;
    al::makeQuatFrontUp(&quat, -front, -gravity);
    al::updatePoseQuat(mActor, quat);
}

void PlayerStateWallAir::calcSnapMoveCutDir(sead::Vector3f* snap) const {
    if (al::isNerve(this, &Slide))
        *snap = mStateNormalWallSlide->mNormalStart;
    else
        *snap = mStateNormalWallJump->mFront;
}

void PlayerStateWallAir::exeSlide() {
    if (al::updateNerveState(this)) {
        mWallActionHistory->recordWallLeave(al::getTrans(mActor),
                                            mStateNormalWallSlide->mNormalStart);
        kill();
        return;
    }

    if (rs::judgeAndResetReturnTrue(mJudgePreInputJump)) {
        mWallActionHistory->recordWallJump(mCollision, al::getTrans(mActor));
        al::setNerve(this, &Jump);
    }
}

void PlayerStateWallAir::exeJump() {
    if (al::updateNerveState(this)) {
        kill();
        return;
    }

    if (rs::updateJudgeAndResult(mJudgeWallKeep))
        al::setNerve(this, &Slide);
}
