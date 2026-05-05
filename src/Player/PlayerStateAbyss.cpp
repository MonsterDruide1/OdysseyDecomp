#include "Player/PlayerStateAbyss.h"

#include "Library/Bgm/BgmLineFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Se/SeFunction.h"

#include "Player/PlayerAnimator.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerRecoverySafetyPoint.h"
#include "Player/PlayerStateRecoveryDead.h"
#include "Util/PlayerUtil.h"

namespace al {
void activateAudioEventController(const IUseAudioKeeper* audioKeeper);
void allowAudioEventActivation(const IUseAudioKeeper* audioKeeper);
void banAudioEventActivation(const IUseAudioKeeper* audioKeeper);
void deactivateAudioEventController(const IUseAudioKeeper* audioKeeper);
}  // namespace al

namespace alSeFunction {
void startSeFromUpperLayerSeKeeper(const al::IUseAudioKeeper* audioKeeper, const char* name);
}  // namespace alSeFunction

namespace {
NERVE_IMPL(PlayerStateAbyss, Fall);
NERVE_IMPL(PlayerStateAbyss, Recovery);

NERVES_MAKE_NOSTRUCT(PlayerStateAbyss, Fall, Recovery);
}  // namespace

PlayerStateAbyss::PlayerStateAbyss(al::LiveActor* player, const PlayerConst* pConst,
                                   PlayerRecoverySafetyPoint* recoverySafetyPoint,
                                   PlayerColliderHakoniwa* collider, PlayerAnimator* animator,
                                   al::LiveActor* capActor)
    : al::ActorStateBase("奈落死", player), mConst(pConst),
      mRecoverySafetyPoint(recoverySafetyPoint), mAnimator(animator) {
    initNerve(&Fall, 1);
    mStateRecoveryDead = new PlayerStateRecoveryDead(player, recoverySafetyPoint, collider,
                                                     animator, pConst, capActor);
    al::initNerveState(this, mStateRecoveryDead, &Recovery, "奈落復帰");
}

void PlayerStateAbyss::appear() {
    al::ActorStateBase::appear();

    if (mRecoverySafetyPoint->isValid()) {
        al::offAreaTarget(mActor);
        al::setNerve(this, &Recovery);
        return;
    }

    if (rs::isPlayer2D(mActor))
        mAnimator->startAnim("Fall");

    al::offAreaTarget(mActor);
    al::setNerve(this, &Fall);
}

void PlayerStateAbyss::kill() {
    al::onAreaTarget(mActor);
    al::setNerve(this, &Fall);
    al::ActorStateBase::kill();
}

void PlayerStateAbyss::prepareRecovery() {
    al::setNerve(this, &Recovery);
}

bool PlayerStateAbyss::isRecovery() const {
    return al::isNerve(this, &Recovery);
}

bool PlayerStateAbyss::isRecoveryLandFall() const {
    return !isDead() && al::isNerve(this, &Recovery) && mStateRecoveryDead->isLandFall();
}

void PlayerStateAbyss::exeFall() {
    if (al::isFirstStep(this)) {
        if (rs::isPlayer3D(mActor)) {
            if (mAnimator->isSubAnimPlaying())
                mAnimator->endSubAnim();

            mAnimator->startAnim("DeadFall");
            al::startSe(mActor, "FallDown");
            al::startSe(mActor, "vDeadFallDown");
        } else {
            alSeFunction::startSeFromUpperLayerSeKeeper(mActor, "FallDown2D");
        }
    }

    al::LiveActor* player = mActor;
    f32 gravity = mConst->getGravityAir();
    f32 fallSpeedMax = mConst->getFallSpeedMax();
    al::addVelocityToGravityLimit(player, gravity, fallSpeedMax);
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
