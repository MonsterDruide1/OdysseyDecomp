#include "Player/PlayerStateBind.h"

#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveStateBase.h"
#include "Library/Nerve/NerveUtil.h"

#include "Player/PlayerActionAirMoveControl.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerPuppet.h"
#include "Player/PlayerTrigger.h"

namespace {
NERVE_IMPL(PlayerStateBind, Bind)
NERVE_IMPL(PlayerStateBind, EndOnGround)
NERVE_IMPL(PlayerStateBind, EndJump)
NERVE_IMPL(PlayerStateBind, EndCapThrow)
NERVE_IMPL(PlayerStateBind, EndFall)

NERVES_MAKE_NOSTRUCT(PlayerStateBind, Bind, EndOnGround, EndJump, EndCapThrow, EndFall)
}  // namespace

PlayerStateBind::PlayerStateBind(al::LiveActor* player, const PlayerConst* playerConst,
                                 const PlayerInput* input, const PlayerPuppet* playerPuppet,
                                 const IUsePlayerCollision* collision, PlayerTrigger* trigger)
    : al::ActorStateBase("バインド", player), mConst(playerConst), mPuppet(playerPuppet),
      mTrigger(trigger) {
    mAirMoveControl = new PlayerActionAirMoveControl(player, playerConst, input, collision, false);
    initNerve(&Bind, 0);
}

void PlayerStateBind::appear() {
    al::setVelocityZero(mActor);

    if (mPuppet->isBinding())
        al::setNerve(this, &Bind);
    else
        endBind();

    al::NerveStateBase::appear();
}

void PlayerStateBind::endBind() {
    if (mPuppet->isBindEndOnGround())
        al::setNerve(this, &EndOnGround);
    else if (mPuppet->isBindEndJump())
        al::setNerve(this, &EndJump);
    else if (mPuppet->isBindEndCapThrow())
        al::setNerve(this, &EndCapThrow);
    else
        al::setNerve(this, &EndFall);
}

void PlayerStateBind::exeBind() {}

void PlayerStateBind::exeEndOnGround() {
    if (al::isFirstStep(this))
        mTrigger->set(PlayerTrigger::EActionTrigger_val3);

    al::tryAddVelocityLimit(mActor, al::getGravity(mActor) * mConst->getGravityAir(),
                            mConst->getFallSpeedMax());
}

void PlayerStateBind::exeEndJump() {
    if (al::isFirstStep(this)) {
        mTrigger->set(PlayerTrigger::EActionTrigger_val3);

        const PlayerBindEndJumpInfo* bindEndJumpInfo = mPuppet->getBindEndJumpInfo();
        al::setVelocity(mActor, bindEndJumpInfo->_0);

        mAirMoveControl->setup(10000.0f, mConst->getJumpMoveSpeedMin(), 0, al::calcSpeedV(mActor),
                               bindEndJumpInfo->gravity, bindEndJumpInfo->_c, 0.0f);
    }

    mAirMoveControl->update();
}

void PlayerStateBind::exeEndCapThrow() {
    if (al::isFirstStep(this))
        mTrigger->set(PlayerTrigger::EActionTrigger_val3);

    al::tryAddVelocityLimit(mActor, al::getGravity(mActor) * mConst->getGravityAir(),
                            mConst->getFallSpeedMax());
}

void PlayerStateBind::exeEndFall() {
    if (al::isFirstStep(this)) {
        mTrigger->set(PlayerTrigger::EActionTrigger_val3);

        mAirMoveControl->setup(10000.0f, mConst->getJumpMoveSpeedMin(), 0, al::calcSpeedV(mActor),
                               mConst->getJumpGravity(), 0, 0.0f);
    }

    mAirMoveControl->update();
}

bool PlayerStateBind::isEndOnGround() const {
    if (al::isNerve(this, &Bind))
        return false;

    return al::isNerve(this, &EndOnGround);
}

bool PlayerStateBind::isEndAir() const {
    if (al::isNerve(this, &Bind))
        return false;

    return al::isNerve(this, &EndJump) || al::isNerve(this, &EndFall);
}

bool PlayerStateBind::isEndCapThrow() const {
    if (al::isNerve(this, &Bind))
        return false;

    return al::isNerve(this, &EndCapThrow);
}

bool PlayerStateBind::isInvalidInput() const {
    return al::isNerve(this, &EndJump) &&
           al::isLessEqualStep(this, mPuppet->getBindEndJumpInfo()->_c);
}
