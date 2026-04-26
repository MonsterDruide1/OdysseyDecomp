#include "Player/PlayerStateAbyss.h"

#include "Library/Audio/System/AudioKeeperFunction.h"
#include "Library/Bgm/BgmLineFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveStateBase.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Se/SeFunction.h"

#include "Player/PlayerAnimator.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerRecoverySafetyPoint.h"
#include "Player/PlayerStateRecoveryDead.h"
#include "Util/PlayerUtil.h"

namespace {
NERVE_IMPL(PlayerStateAbyss, Fall)
NERVE_IMPL(PlayerStateAbyss, Recovery)
NERVES_MAKE_NOSTRUCT(PlayerStateAbyss, Fall, Recovery)
}  // namespace

PlayerStateAbyss::PlayerStateAbyss(al::LiveActor* player, const PlayerConst* playerConst,
                                   PlayerRecoverySafetyPoint* recoverySafetyPoint,
                                   PlayerColliderHakoniwa* playerCollider,
                                   PlayerAnimator* playerAnimator, al::LiveActor* playerModelHolder)
    : al::ActorStateBase("奈落死", player), mPlayerConst(playerConst),
      mPlayerRecoverySafetyPoint(recoverySafetyPoint), mPlayerAnimator(playerAnimator) {
    initNerve(&Fall, 1);
    mPlayerStateRecoveryDead =
        new PlayerStateRecoveryDead(player, recoverySafetyPoint, playerCollider, playerAnimator,
                                    playerConst, playerModelHolder);
    al::initNerveState(this, mPlayerStateRecoveryDead, &Recovery, "奈落復帰");
}

void PlayerStateAbyss::appear() {
    al::NerveStateBase::appear();
    if (mPlayerRecoverySafetyPoint->isValid()) {
        al::offAreaTarget(mActor);
        al::setNerve(this, &Recovery);
        return;
    }

    if (rs::isPlayer2D(mActor))
        mPlayerAnimator->startAnim("Fall");

    al::offAreaTarget(mActor);
    al::setNerve(this, &Fall);
}

void PlayerStateAbyss::kill() {
    al::onAreaTarget(mActor);
    al::setNerve(this, &Fall);
    al::NerveStateBase::kill();
}

void PlayerStateAbyss::exeFall() {
    if (al::isFirstStep(this)) {
        if (rs::isPlayer3D(mActor)) {
            if (mPlayerAnimator->isSubAnimPlaying())
                mPlayerAnimator->endSubAnim();
            mPlayerAnimator->startAnim("DeadFall");
            al::startSe(mActor, "FallDown");
            al::startSe(mActor, "vDeadFallDown");
        } else {
            alSeFunction::startSeFromUpperLayerSeKeeper(mActor, "FallDown2D");
        }
    }

    al::addVelocityToGravityLimit(mActor, mPlayerConst->getGravityAir(),
                                  mPlayerConst->getFallSpeedMax());
}

void PlayerStateAbyss::exeRecovery() {
    if (al::isFirstStep(this)) {
        al::deactivateAudioEventController(mActor);
        al::banAudioEventActivation(mActor);
    }

    if (al::updateNerveState(this)) {
        al::allowAudioEventActivation(mActor);
        al::activateAudioEventController(mActor);
        al::startAndStopBgmInCurPosition(mActor, false);
        kill();
    }
}

bool PlayerStateAbyss::isRecovery() const {
    return al::isNerve(this, &Recovery);
}

bool PlayerStateAbyss::isRecoveryLandFall() const {
    if (!isDead() && al::isNerve(this, &Recovery))
        return mPlayerStateRecoveryDead->isLandFall();
    return false;
}

void PlayerStateAbyss::prepareRecovery() {
    al::setNerve(this, &Recovery);
}
