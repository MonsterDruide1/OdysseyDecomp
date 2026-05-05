#include "Player/YoshiStateHackDown.h"

#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Player/PlayerActionAirMoveControl.h"
#include "Player/PlayerAnimator.h"
#include "Player/PlayerConst.h"
#include "Util/ObjUtil.h"
#include "Util/PlayerCollisionUtil.h"

namespace {
NERVE_IMPL(YoshiStateHackDown, Down);
NERVE_IMPL(YoshiStateHackDown, Land);

NERVES_MAKE_NOSTRUCT(YoshiStateHackDown, Down, Land);
}  // namespace

YoshiStateHackDown::YoshiStateHackDown(al::LiveActor* player, IUsePlayerHack** hacker,
                                       const PlayerConst* pConst,
                                       const IUsePlayerCollision* collider,
                                       PlayerAnimator* animator)
    : HackerStateBase("ダウン", player, hacker), mConst(pConst), mCollider(collider),
      mAnimator(animator) {
    mAirMoveControl = new PlayerActionAirMoveControl(player, pConst, nullptr, collider, false);
    mAirMoveControl->setCalcGroundNormalOrUpDir(true);
    mAirMoveControl->setHacker(hacker);
    initNerve(&Down, 0);
}

void YoshiStateHackDown::appear() {
    al::LiveActor* player = mActor;
    al::NerveStateBase::appear();
    rs::calcGroundNormalOrGravityDir(&mUp, player, mCollider);

    sead::Vector3f front = {0.0f, 0.0f, 0.0f};
    al::calcFrontDir(&front, player);
    al::setVelocity(player, front * -mConst->getPushPowerDamage());

    mAirMoveControl->setup(mConst->getJumpMoveSpeedMax(), mConst->getPushPowerDamage(), 0,
                           mConst->getHopPowerDamage(), mConst->getGravityDamage(), 9999, 1.0f);
    al::setNerve(this, &Down);
}

bool YoshiStateHackDown::isLand() const {
    return al::isNerve(this, &Land);
}

bool YoshiStateHackDown::isEnableCancel() const {
    return isLand() && al::isGreaterEqualStep(this, mConst->getDamageCancelFrame());
}

void YoshiStateHackDown::exeDown() {
    al::LiveActor* player = mActor;
    if (al::isFirstStep(this))
        mAnimator->startAnim("NoDamageDown");

    mAirMoveControl->update();
    if (rs::isOnGroundLessAngle(player, mCollider, mConst->getStandAngleMin())) {
        rs::startHitReactionLandIfLanding(player, mCollider, false);
        rs::brakeLandVelocityGroundNormal(player, &mUp, mCollider, -al::getGravity(player), 0.0f,
                                          mConst->getGravity());
        al::setNerve(this, &Land);
        return;
    }

    if (al::isGreaterEqualStep(this, 120))
        kill();
}

void YoshiStateHackDown::exeLand() {
    if (al::isFirstStep(this))
        mAnimator->startAnim("DamageLand");

    al::LiveActor* player = mActor;
    if (rs::isOnGroundLessAngle(player, mCollider, mConst->getStandAngleMin())) {
        rs::brakeLandVelocityGroundNormal(player, &mUp, mCollider, mUp, 0.0f, mConst->getGravity());
    } else {
        al::tryAddVelocityLimit(player, al::getGravity(player) * mConst->getGravityAir(),
                                mConst->getFallSpeedMax());
    }

    if (mAnimator->isAnimEnd())
        kill();
}
