#include "Player/PlayerStateWallAir.h"

#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Player/PlayerJudgePreInputJump.h"
#include "Player/PlayerStateNormalWallJump.h"
#include "Player/PlayerStateNormalWallSlide.h"
#include "Player/PlayerWallActionHistory.h"
#include "Util/JudgeUtil.h"

namespace {
NERVE_IMPL(PlayerStateWallAir, Slide)
NERVE_IMPL(PlayerStateWallAir, Jump)

NERVES_MAKE_NOSTRUCT(PlayerStateWallAir, Slide, Jump)
}  // namespace

PlayerStateWallAir::PlayerStateWallAir(al::LiveActor* player, const PlayerConst* pConst,
                                       const PlayerInput* input, const PlayerTrigger* trigger,
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

void PlayerStateWallAir::appear() {
    setDead(false);
    al::setNerve(this, &Slide);
}

bool PlayerStateWallAir::isAir() const {
    return al::isNerve(this, &Jump) && al::isGreaterStep(this, 0);
}

bool PlayerStateWallAir::isJustJump() const {
    return al::isNerve(this, &Jump) && al::isFirstStep(this);
}

bool PlayerStateWallAir::isEnableReactionCapCatch() const {
    return !isDead() && al::isNerve(this, &Jump);
}

void PlayerStateWallAir::startSlideSpinAttack() {
    al::LiveActor* player = mActor;
    const sead::Vector3f& gravity = al::getGravity(player);
    mWallActionHistory->recordWallJump(mCollision, al::getTrans(player) + gravity * 20.0f);

    sead::Vector3f front = {0.0f, 0.0f, 0.0f};
    al::calcFrontDir(&front, player);

    sead::Quatf quat = sead::Quatf::unit;
    al::makeQuatFrontUp(&quat, -front, -gravity);
    al::updatePoseQuat(player, quat);
}

void PlayerStateWallAir::calcSnapMoveCutDir(sead::Vector3f* cutDir) const {
    if (al::isNerve(this, &Slide))
        cutDir->set(mStateNormalWallSlide->getNormalStart());
    else
        cutDir->set(mStateNormalWallJump->getFront());
}

void PlayerStateWallAir::exeSlide() {
    if (al::updateNerveState(this)) {
        mWallActionHistory->recordWallLeave(al::getTrans(mActor),
                                            mStateNormalWallSlide->getNormalStart());
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
