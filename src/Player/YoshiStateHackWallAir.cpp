#include "Player/YoshiStateHackWallAir.h"

#include <math/seadQuat.h>

#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Player/PlayerAnimator.h"
#include "Player/PlayerWallActionHistory.h"
#include "Player/YoshiStateHackWallCling.h"
#include "Player/YoshiStateHackWallJump.h"
#include "Util/Hack.h"

namespace {
NERVE_IMPL(YoshiStateHackWallAir, Cling)
NERVE_IMPL(YoshiStateHackWallAir, Jump)

NERVES_MAKE_NOSTRUCT(YoshiStateHackWallAir, Cling, Jump)
}  // namespace

YoshiStateHackWallAir::YoshiStateHackWallAir(
    al::LiveActor* actor, IUsePlayerHack** hacker, const PlayerConst* pConst,
    const IUsePlayerCollision* collision, const PlayerTrigger* trigger, const YoshiTongue* tongue,
    PlayerWallActionHistory* wallActionHistory, PlayerAnimator* animator)
    : HackerStateBase("空中壁", actor, hacker) {
    mAnimator = animator;
    mCollision = collision;
    mWallActionHistory = wallActionHistory;
    mStateWallCling = nullptr;
    mStateWallJump = nullptr;

    initNerve(&Cling, 2);
    mStateWallCling = new YoshiStateHackWallCling(actor, hacker, pConst, collision, animator);
    mStateWallJump =
        new YoshiStateHackWallJump(actor, hacker, pConst, collision, trigger, tongue, animator);
    al::initNerveState(this, mStateWallCling, &Cling, "壁接着");
    al::initNerveState(this, mStateWallJump, &Jump, "壁ジャンプ");
}

void YoshiStateHackWallAir::appear() {
    al::NerveStateBase::appear();
    mWallActionHistory->reset();
    al::setNerve(this, &Cling);
}

bool YoshiStateHackWallAir::isCling() const {
    return al::isNerve(this, &Cling);
}

bool YoshiStateHackWallAir::isAir() const {
    return al::isNerve(this, &Jump) && al::isGreaterStep(this, 0);
}

void YoshiStateHackWallAir::setupCling(const al::CollisionParts* collisionParts,
                                       const sead::Vector3f& position,
                                       const sead::Vector3f& normal) {
    mStateWallCling->setup(collisionParts, position, normal);
}

void YoshiStateHackWallAir::startShrink() {
    mWallActionHistory->recordWallJump(mCollision, al::getTrans(mActor));

    if (al::isNerve(this, &Cling)) {
        al::LiveActor* actor = mActor;
        sead::Vector3f front(0.0f, 0.0f, 0.0f);
        al::calcFrontDir(&front, actor);

        sead::Vector3f up = {0.0f, 0.0f, 0.0f};
        al::calcUpDir(&up, actor);

        sead::Quatf quat = sead::Quatf::unit;
        al::makeQuatFrontUp(&quat, -front, up);
        al::updatePoseQuat(actor, quat);
        mAnimator->startAnim("Fall");
    }
}

void YoshiStateHackWallAir::exeCling() {
    if (al::updateNerveState(this)) {
        sead::Vector3f front = {0.0f, 0.0f, 0.0f};
        al::calcFrontDir(&front, mActor);
        front.negate();

        mWallActionHistory->recordWallJump(al::getTrans(mActor), front);
        kill();
        return;
    }

    if (rs::isTriggerHackJump(*mHacker)) {
        mWallActionHistory->recordWallJump(mCollision, al::getTrans(mActor));
        al::setNerve(this, &Jump);
    }
}

void YoshiStateHackWallAir::exeJump() {
    if (al::updateNerveState(this))
        kill();
}
