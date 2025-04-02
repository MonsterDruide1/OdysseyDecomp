#include "Player/PlayerStateAutoDash.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveStateBase.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Player/PlayerFunction.h"

#include "Player/PlayerFunction.h"

namespace {
NERVE_IMPL(PlayerStateAutoDash, Run)
NERVE_IMPL(PlayerStateAutoDash, Dash)
NERVES_MAKE_NOSTRUCT(PlayerStateAutoDash, Run, Dash)
}  // namespace

PlayerStateAutoDash::PlayerStateAutoDash(al::LiveActor* player)
    : al::ActorStateBase("自動ダッシュ", player) {
    initNerve(&Run, 0);
}

void PlayerStateAutoDash::appear() {
    al::LiveActor* player = mActor;
    setDead(false);

    if (al::calcSpeed(player) < 12.0f)
        al::setNerve(this, &Run);
    else
        al::setNerve(this, &Dash);
}

void PlayerStateAutoDash::exeDash() {
    if (al::isFirstStep(this)) {
        al::startAction(mActor, "Dash");
        mRunTimer = 0;
    }

    al::addVelocityToGravity(mActor, 6.0f);
    al::scaleVelocity(mActor, 0.7f);

    al::LiveActor* player = mActor;
    al::setSklAnimFrameRate(player, al::calcSpeed(player) * 0.08f, 0);

    sead::Vector3f accelDir = {0.0f, 0.0f, 0.0f};
    al::addPlayerAccelStickGravity(mActor, &accelDir, 6.0f, {0.0f, 1.0f, 0.0f},
                                   PlayerFunction::getPlayerInputPort(mActor),
                                   &PlayerFunction::getPlayerViewMtx(mActor));
    al::faceToDirection(mActor, accelDir);

    if (al::isVelocitySlow(mActor, 6.0f)) {
        kill();
        return;
    }

    al::reboundVelocityFromCollision(mActor, 0.0f, 0.0f, 1.0f);
}

void PlayerStateAutoDash::exeRun() {
    if (al::isFirstStep(this)) {
        al::startAction(mActor, "Dash");
        mRunTimer = 0;
    }

    al::addVelocityToGravity(mActor, 3.0f);
    al::scaleVelocity(mActor, 0.7f);

    sead::Vector3f accelDir = {0.0f, 0.0f, 0.0f};
    al::addPlayerAccelStickGravity(mActor, &accelDir, 3.0f, {0.0f, 1.0f, 0.0f},
                                   PlayerFunction::getPlayerInputPort(mActor),
                                   &PlayerFunction::getPlayerViewMtx(mActor));
    al::faceToDirection(mActor, accelDir);

    al::LiveActor* player = mActor;
    al::setSklAnimFrameRate(player, al::calcSpeed(player) * 0.27f, 0);

    if (al::isVelocitySlow(mActor, 3.0f)) {
        kill();
        return;
    }

    al::reboundVelocityFromCollision(mActor, 0.0f, 0.0f, 1.0f);
    mRunTimer++;
    if (mRunTimer >= 60)
        al::setNerve(this, &Dash);
}
