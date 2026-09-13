#include "Enemy/Jango/JangoCapSeparateHelpClaimUpdater.h"

#include "Library/Camera/CameraUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Player/PlayerUtil.h"
#include "Library/Stage/StageSwitchUtil.h"

#include "Enemy/Jango/Jango.h"
#include "Enemy/Jango/JangoCap.h"
#include "Layout/BalloonIcon.h"
#include "System/GameDataFunction.h"
#include "Util/InputInterruptTutorialUtil.h"
#include "Util/PlayerUtil.h"
#include "Util/StageInputFunction.h"

namespace {
NERVE_IMPL(JangoCap, Wait)
NERVE_IMPL(JangoCap, TalkDemo)
NERVE_IMPL(JangoCap, Fall)
NERVE_IMPL(JangoCap, Catched)
NERVE_IMPL(JangoCap, Carried)
NERVE_IMPL(JangoCap, ReleaseDemoWait)

NERVES_MAKE_NOSTRUCT(JangoCap, ReleaseDemoWait)
NERVES_MAKE_STRUCT(JangoCap, Wait, TalkDemo, Fall, Catched, Carried)
}  // namespace

void JangoCapSeparateHelpClaimUpdater::update() {
    if (rs::isSeparatePlay(mActor)) {
        if (rs::isPlayerInputTriggerSeparateCapJangoHelp(mActor)) {
            al::LiveActor* actor = mActor;
            if (al::isNerve(actor, &NrvJangoCap.Catched) ||
                al::isNerve(actor, &NrvJangoCap.Carried)) {
                if (mHelpReactionCoolTime == 0) {
                    al::startHitReaction(mActor, "さけぶ");
                    mHelpReactionCoolTime = 120;
                }
            }
        }
    } else if (mSinglePlayReactionTimer++ >= 239) {
        mSinglePlayReactionTimer = 0;
        al::startHitReaction(mActor, "さけぶ(シングルプレイ)");
    }

    if (mHelpReactionCoolTime > 0)
        --mHelpReactionCoolTime;
}

void JangoCap::demoEnd() {
    mBalloonIcon->startUpdateDraw();
}

void JangoCap::hideBalloon() {
    mBalloonIcon->hideAndStopUpdate();
}

void JangoCap::startBlowDown() {
    al::startHitReaction(this, "ヒット");
    al::startAction(this, "DemoJangoBlowDown");
    mIsBlowDown = true;
}

void JangoCap::released(const sead::Vector3f& trans, const sead::Quatf& quat) {
    mReleaseTrans = trans;
    mReleaseQuat = quat;
    al::onCollide(this);
    al::setNerve(this, &ReleaseDemoWait);
}

void JangoCap::catched() {
    mIsBlowDown = false;
    al::setNerve(this, &NrvJangoCap.Catched);
}

void JangoCap::directCatched() {
    BalloonIcon* balloonIcon = mBalloonIcon;
    mIsBlowDown = false;
    balloonIcon->startUpdateDraw();
    al::setNerve(this, &NrvJangoCap.Catched);
}

void JangoCap::startStruggle() {
    al::startAction(this, "DemoJangoStruggle");
}

void JangoCap::exeCatched() {
    if (al::isFirstStep(this))
        al::startAction(this, "DemoJangoHunted");

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvJangoCap.Carried);
}

void JangoCap::exeCarried() {
    if (al::isFirstStep(this))
        al::startAction(this, "DemoJangoCarried");
}

void JangoCap::exeReleaseDemoWait() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "DemoJangoLand");
        al::setQuat(this, mReleaseQuat);
        al::setVelocityZero(this);
        al::resetPosition(this, mReleaseTrans);
    }

    if (al::isActionPlaying(this, "DemoJangoLand") && al::isActionEnd(this))
        al::startAction(this, "DemoJangoWaitPlayer");

    if (al::isGreaterEqualStep(this, 60))
        al::setNerve(this, &NrvJangoCap.Fall);
}

void JangoCap::exeFall() {
    if (al::isActionPlaying(this, "DemoJangoLand") && al::isActionEnd(this))
        al::startAction(this, "DemoJangoWaitPlayer");

    al::scaleVelocity(this, 0.95f);
    al::addVelocityToGravity(this, 2.0f);

    if (al::isOnGround(this, 0))
        al::setNerve(this, &NrvJangoCap.Wait);
}

void JangoCap::exeWait() {
    if (al::isActionPlaying(this, "DemoJangoLand") && al::isActionEnd(this))
        al::startAction(this, "DemoJangoWaitPlayer");

    al::turnToTarget(this, al::getPlayerPos(this, 0), 1.0f);
}

void JangoCap::exeTalkDemo() {
    if (al::isFirstStep(this)) {
        al::setVelocityZero(this);
        rs::changeEnableCapTutorial(this);
    }

    if (al::updateNerveState(this)) {
        GameDataFunction::disableJangoTrans(this);
        mJango->tryOffCapCatchedSwitch();
        al::tryOnStageSwitch(this, "CapGetOn");
        GameDataFunction::addJangoCount(this);

        if (mEntranceCameraTicket)
            al::startCamera(this, mEntranceCameraTicket, -1);

        kill();
    }
}

JangoCapSeparateHelpClaimUpdater::JangoCapSeparateHelpClaimUpdater(al::LiveActor* actor)
    : mActor(actor) {}
