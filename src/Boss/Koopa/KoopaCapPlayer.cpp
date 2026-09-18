#include "Boss/Koopa/KoopaCapPlayer.h"

#include <math/seadQuat.h>

#include "Library/Effect/EffectSystemInfo.h"
#include "Library/Joint/JointControllerKeeper.h"
#include "Library/Joint/JointSpringControllerHolder.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Movement/EnemyStateBlowDown.h"
#include "Library/Movement/RumbleCalculator.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Boss/Koopa/KoopaCapPlayerBinder.h"
#include "Boss/Koopa/KoopaFunction.h"
#include "Player/PlayerEquipmentFunction.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolderAccessor.h"
#include "Util/InputInterruptTutorialUtil.h"
#include "Util/PlayerUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(KoopaCapPlayer, Start)
NERVE_END_IMPL(KoopaCapPlayer, BlowDown)
NERVE_END_IMPL_(KoopaCapPlayer, BlowDownWithoutHitReaction, BlowDown)
NERVE_IMPL(KoopaCapPlayer, HideChase)
NERVE_IMPL(KoopaCapPlayer, CatchPrepare)
NERVE_IMPL(KoopaCapPlayer, PunchFinish)
NERVE_IMPL(KoopaCapPlayer, PunchWait)
NERVE_END_IMPL(KoopaCapPlayer, Punch)
NERVE_IMPL(KoopaCapPlayer, Damage)
NERVE_IMPL(KoopaCapPlayer, Catch)
NERVE_IMPL(KoopaCapPlayer, Wait)
NERVE_IMPL(KoopaCapPlayer, WaitBubble)
NERVE_IMPL(KoopaCapPlayer, PunchStart)
NERVE_IMPL(KoopaCapPlayer, PunchFinishStart)
NERVE_IMPL(KoopaCapPlayer, PunchEnd)
NERVE_IMPL(KoopaCapPlayer, PunchFinishWait)
NERVE_IMPL(KoopaCapPlayer, PunchFinishEnd)

NERVES_MAKE_STRUCT(KoopaCapPlayer, Start, BlowDown, BlowDownWithoutHitReaction, HideChase,
                   CatchPrepare, PunchFinish, PunchWait, Punch, Damage, Catch, Wait, WaitBubble,
                   PunchStart, PunchFinishStart, PunchEnd, PunchFinishWait, PunchFinishEnd)

static const sead::Vector3f sRetargettingOffset = {0.75f, 0.75f, 0.75f};
static const sead::Vector3f sPlayerOffset = {0.0f, 130.0f, 0.0f};
static const sead::Vector3f sHeadJointOffset = {40.0f, 0.0f, 0.0f};

void updatePoseFromPlayerEquipment(KoopaCapPlayer* actor, const PlayerEquipmentUser* user) {
    sead::Quatf quat = sead::Quatf::unit;
    sead::Vector3f up = {0.0f, 0.0f, 0.0f};
    sead::Vector3f front = {0.0f, 0.0f, 0.0f};

    if (PlayerEquipmentFunction::isPlayerRolling(user)) {
        rs::calcPlayerUpDir(&up, actor);
        rs::calcPlayerFrontDir(&front, actor);
    } else if (!rs::isPlayerOnGround(actor) && rs::tryCalcPlayerModelHeadJointUp(&up, actor) &&
               rs::tryCalcPlayerModelHeadJointSide(&front, actor)) {
        up.negate();
    } else {
        rs::calcPlayerUpDir(&up, actor);
        if (rs::tryCalcPlayerModelHeadJointSide(&front, actor) &&
            !al::isParallelDirection(up, front, 0.01f)) {
            al::verticalizeVec(&front, up, front);
            al::normalize(&front);
        } else {
            rs::calcPlayerFrontDir(&front, actor);
        }
    }

    al::makeQuatFrontUp(&quat, front, up);
    al::updatePoseQuat(actor, quat);

    if (PlayerEquipmentFunction::isPlayerRolling(user) ||
        !rs::tryCalcPlayerModelHeadJointPos(al::getTransPtr(actor), actor)) {
        al::setTrans(actor, rs::getPlayerPos(actor));
        al::multVecPose(al::getTransPtr(actor), actor, sPlayerOffset);
    } else {
        al::multVecPose(al::getTransPtr(actor), actor, sHeadJointOffset);
    }
}

// NONMATCHING
bool tryStartNextPunchFromInput(KoopaCapPlayer* actor, KoopaCapPlayerPunchState* punchState,
                                KoopaCapPlayerBinder* binder) {
    if (rs::isPlayerCameraSubjective(actor))
        return false;

    if (!punchState->trySetupNextPunchFromInput(true))
        return false;

    if (!binder->isBinding())
        rs::requestBindPlayer(actor, al::getHitSensor(actor, "Bind"));

    if (punchState->finishState == 1)
        al::setNerve(actor, &NrvKoopaCapPlayer.PunchFinishStart);
    else if (punchState->isPunchFollowUp)
        al::setNerve(actor, &NrvKoopaCapPlayer.Punch);
    else
        al::setNerve(actor, &NrvKoopaCapPlayer.PunchStart);

    return true;
}

void updateTriggerInputs(KoopaCapPlayer* actor) {
    const PlayerEquipmentUser* user = actor->mEquipmentUser;
    if (PlayerEquipmentFunction::isTriggerSwingLeft(user))
        actor->mPunchState.isTriggerSwingLeft = true;
    if (PlayerEquipmentFunction::isTriggerSwingRight(user))
        actor->mPunchState.isTriggerSwingRight = true;
    if (PlayerEquipmentFunction::isTriggerCapAction(user))
        actor->mPunchState.isTriggerCapAction = true;
}

bool isPlayerLifeZero(const KoopaCapPlayer* actor) {
    return GameDataFunction::isPlayerLifeZero(GameDataHolderAccessor(actor));
}
}  // namespace

KoopaCapPlayer::KoopaCapPlayer(const char* name) : al::LiveActor(name) {}

void KoopaCapPlayer::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "KoopaCapPlayer", nullptr);
    al::initNerve(this, &NrvKoopaCapPlayer.Start, 2);
    al::invalidateHitSensor(this, "Bind");
    al::hideSilhouetteModelIfShow(this);
    al::initAndBindSklAnimRetargetting(this, "KoopaCap", sRetargettingOffset);
    al::validateSklAnimRetargetting(this);

    KoopaCapPlayerRumble* rumble = new KoopaCapPlayerRumble;
    rumble->handScaleL = 1.0f;
    rumble->handScaleR = 1.0f;
    rumble->clearKeepScale();
    rumble->left = new al::RumbleCalculatorCosMultLinear(5.5f, 2.0f, 0.5f, 45);
    rumble->right = new al::RumbleCalculatorCosMultLinear(5.5f, 2.0f, 0.5f, 45);
    mRumble = rumble;

    mJointSpringControllerHolder = new al::JointSpringControllerHolder();
    mEquipmentInfo = PlayerEquipmentFunction::createEquipmentInfoKoopaCap(this);
    al::initJointControllerKeeper(this, 7);
    KoopaCapPlayerRumble* scaleRumble = mRumble;
    al::initJointLocalScaleControllerX(this, &scaleRumble->handScaleL, "HandL");
    al::initJointLocalScaleControllerX(this, &scaleRumble->handScaleR, "HandR");
    mJointSpringControllerHolder->init(this, "InitJointSpringCtrl");
    KoopaFunction::initCapJointSideRotator(this, &mCapBlowDownSideDegree);

    mBlowDownState =
        new al::EnemyStateBlowDown(this, &KoopaFunction::getCapBlowDownParam(), "吹き飛び状態");
    al::initNerveState(this, mBlowDownState, &NrvKoopaCapPlayer.BlowDown, "吹き飛び");
    al::initNerveState(this, mBlowDownState, &NrvKoopaCapPlayer.BlowDownWithoutHitReaction,
                       "吹き飛び[ヒットリアクションなし]");
    makeActorDead();
}

void KoopaCapPlayer::startHideChase() {
    if (al::isDead(this))
        appear();
    al::hideModelIfShow(this);
    al::resetPosition(this, *mHideChaseTrans);
    al::setNerve(this, &NrvKoopaCapPlayer.HideChase);
}

void KoopaCapPlayer::appear() {
    al::LiveActor::appear();
    mCapBlowDownSideDegree = 0.0f;
}

void KoopaCapPlayer::kill() {
    al::LiveActor::kill();
    mPunchState.isPunchHit = false;
    mPunchState.isPunchHitReaction = false;
    mPunchState.isTriggerSwingLeft = false;
    mPunchState.isTriggerSwingRight = false;
    mPunchState.isTriggerCapAction = false;
    mPunchState.finishState = 0;
    mPunchState.isFastPunchInput = false;
    mPunchState.fastPunchRate = 0.0f;
    if (mBinder->isBinding())
        mBinder->endPunchBind();
}

void KoopaCapPlayer::control() {
    if (mJointSpringControlRate > 0.0f) {
        mJointSpringControlRate = al::lerpValue(mJointSpringControlRate, 1.0f, 0.1f);
        mJointSpringControllerHolder->setControlRateAll(mJointSpringControlRate);
    } else {
        mJointSpringControllerHolder->offControlAll();
    }

    KoopaCapPlayerRumble* rumble = mRumble;

    if (rumble->left->isActive())
        rumble->left->calc();
    if (rumble->right->isActive())
        rumble->right->calc();

    rumble->handScaleL = rumble->left->isActive() ? rumble->left->getOutput().y + 1.0f : 1.0f;
    rumble->handScaleR = rumble->right->isActive() ? rumble->right->getOutput().y + 1.0f : 1.0f;
    u16 keepScaleFlags = rumble->keepScaleFlags;
    if ((keepScaleFlags & 0xff) != 0)
        rumble->handScaleL = 0.5f;
    if (keepScaleFlags >= 0x100)
        rumble->handScaleR = 0.5f;

    if (mEquipmentUser != nullptr) {
        if (!al::isNerve(this, &NrvKoopaCapPlayer.CatchPrepare)) {
            if (rs::isPlayerCameraSubjective(this))
                al::hideModelIfShow(this);
            else
                PlayerEquipmentFunction::syncEquipVisibility(this, mEquipmentUser);
        }
        if (mPunchState.damageCooldown > 0)
            mPunchState.damageCooldown--;
    }
}

void KoopaCapPlayer::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isNerve(this, &NrvKoopaCapPlayer.BlowDown) ||
        al::isNerve(this, &NrvKoopaCapPlayer.BlowDownWithoutHitReaction))
        return;

    if (al::isSensorPlayer(self)) {
        if (mEquipmentUser != nullptr)
            rs::sendMsgPlayerItemGetAll(other, self);
        return;
    }

    if (mPunchState.isPunchHit)
        return;

    if (al::isNerve(this, &NrvKoopaCapPlayer.Punch)) {
        if (al::isActionEnd(this))
            return;
    } else if (!al::isNerve(this, &NrvKoopaCapPlayer.PunchWait)) {
        return;
    }

    if (mPunchState.isPunchLeft) {
        if (al::isSensorName(self, "PunchL")) {
            if (mPunchState.finishState == 1) {
                if (rs::sendMsgKoopaCapPunchFinishL(other, self)) {
                    mPunchState.isPunchHit = true;
                    mBinder->startPrepareFinishPunchL(al::getHitSensor(this, "Bind"));
                    al::setNerve(this, &NrvKoopaCapPlayer.PunchFinish);
                    return;
                }
            } else if (rs::sendMsgKoopaCapPunchKnockBackL(other, self)) {
                mPunchState.isPunchHit = true;
                mPunchState.isPunchHitReaction = true;
                mBinder->startPrepareKnockBackPunchL(al::getHitSensor(this, "Bind"));
                al::setNerve(this, &NrvKoopaCapPlayer.PunchFinish);
                return;
            }

            if (rs::sendMsgKoopaCapPunchInvincibleL(other, self) ||
                rs::sendMsgKoopaCapPunchL(other, self))
                mPunchState.isPunchHit = true;
        }
    }

    if (!mPunchState.isPunchLeft && al::isSensorName(self, "PunchR")) {
        if (mPunchState.finishState == 1) {
            if (rs::sendMsgKoopaCapPunchFinishR(other, self)) {
                mPunchState.isPunchHit = true;
                mBinder->startPrepareFinishPunchR(al::getHitSensor(this, "Bind"));
                al::setNerve(this, &NrvKoopaCapPlayer.PunchFinish);
                return;
            }
        } else if (rs::sendMsgKoopaCapPunchKnockBackR(other, self)) {
            mPunchState.isPunchHit = true;
            mPunchState.isPunchHitReaction = true;
            mBinder->startPrepareKnockBackPunchR(al::getHitSensor(this, "Bind"));
            al::setNerve(this, &NrvKoopaCapPlayer.PunchFinish);
            return;
        }

        if (rs::sendMsgKoopaCapPunchInvincibleR(other, self) ||
            rs::sendMsgKoopaCapPunchR(other, self))
            mPunchState.isPunchHit = true;
    }

    if (!mPunchState.isPunchHit)
        return;

    KoopaCapPlayerRumble* rumble = mRumble;
    if (mPunchState.isPunchLeft) {
        rumble->left->setParam(5.5f, 2.0f, 0.5f, 45);
        rumble->left->start(0);
    } else {
        rumble->right->setParam(5.5f, 2.0f, 0.5f, 45);
        rumble->right->start(0);
    }
    if (rs::sendMsgKoopaCapPlayerFocusTarget(other, self))
        mPunchState.focusTarget = al::getSensorHost(other);
    if (al::isNerve(this, &NrvKoopaCapPlayer.PunchWait))
        al::setNerve(this, &NrvKoopaCapPlayer.Punch);
}

bool KoopaCapPlayer::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                al::HitSensor* self) {
    if (mEquipmentUser != nullptr) {
        if (al::isMsgPlayerReleaseEquipment(message)) {
            if (mEquipmentUser != nullptr)
                PlayerEquipmentFunction::endEquip(&mEquipmentUser);
            rs::tryCloseKoopaCapTutorial(this);
            al::hideSilhouetteModelIfShow(this);
            al::tryKillEmitterAndParticleAll(this);
            kill();
            return true;
        }

        if (rs::isMsgNoticePlayerDamage(message)) {
            if (al::isNerve(this, &NrvKoopaCapPlayer.Damage))
                return false;
            mPunchState.damageCooldown = 100;
            al::setNerve(this, &NrvKoopaCapPlayer.Damage);
            return true;
        }
    }

    if (al::isMsgPlayerDisregard(message))
        return mEquipmentUser != nullptr;

    if (mEquipmentUser == nullptr && al::isSensorEnemyBody(self) &&
        al::isNerve(this, &NrvKoopaCapPlayer.HideChase) && al::isMsgPlayerPutOnEquipment(message)) {
        mEquipmentUser = PlayerEquipmentFunction::startEquip(self, other, mEquipmentInfo);
        al::hideModelIfShow(this);
        al::setVelocityZero(this);
        al::setNerve(this, &NrvKoopaCapPlayer.CatchPrepare);
        return true;
    }

    if (mBinder->isBinding() && al::isMsgBindDamage(message)) {
        mBinder->endPunchBind();
        mPunchState.damageCooldown = 100;
        al::setNerve(this, &NrvKoopaCapPlayer.Damage);
        return true;
    }

    if (al::isSensorPlayer(self))
        return false;
    if (!al::isSensorBindable(self))
        return false;

    if (mBinder->isBinding())
        return mBinder->tryCancelBind(message);

    if (mBinder->tryReceiveMsgBindStart(message))
        return true;

    if (mBinder->tryStartPuppetCatchKoopaCap(message, other, self)) {
        al::showModelIfHide(this);
        al::setNerve(this, &NrvKoopaCapPlayer.Catch);
        return true;
    }

    if (mBinder->tryStartPuppetKnockBackPunch(message, other, self) ||
        mBinder->tryStartPuppetFinishPunch(message, other, self))
        return true;

    if (mPunchState.finishState == 1)
        return false;
    if (mPunchState.focusTarget == nullptr)
        return false;
    if (al::isMsgBindStart(message))
        return rs::isPlayerOnGround(this);

    if (mBinder->tryStartPunchAndFocusTarget(message, other, self,
                                             al::getTrans(mPunchState.focusTarget))) {
        al::invalidateHitSensor(this, "Bind");
        return true;
    }

    return false;
}

void KoopaCapPlayer::endEquipAndKill() {
    if (mEquipmentUser != nullptr)
        PlayerEquipmentFunction::endEquip(&mEquipmentUser);
    rs::tryCloseKoopaCapTutorial(this);
    al::hideSilhouetteModelIfShow(this);
    al::tryKillEmitterAndParticleAll(this);
    kill();
}

bool KoopaCapPlayer::isPlayingCatchDemo() const {
    return al::isNerve(this, &NrvKoopaCapPlayer.CatchPrepare) ||
           al::isNerve(this, &NrvKoopaCapPlayer.Catch);
}

bool KoopaCapPlayer::isPlayerBinding() const {
    return mBinder->isBinding();
}

void KoopaCapPlayer::onFinish() {
    mPunchState.finishState = 1;
}

void KoopaCapPlayer::offFinish() {
    mPunchState.finishState = 0;
}

void KoopaCapPlayer::endEquipAndBlowDown() {
    if (mEquipmentUser != nullptr)
        PlayerEquipmentFunction::endEquip(&mEquipmentUser);
    rs::tryCloseKoopaCapTutorial(this);
    al::hideSilhouetteModelIfShow(this);

    al::EnemyStateBlowDown* blowDownState = mBlowDownState;
    const sead::Vector3f& actorFront = al::getFront(this);
    sead::Vector3f front = {-actorFront.x, -actorFront.y, -actorFront.z};
    blowDownState->start(front);
    al::setNerve(this, &NrvKoopaCapPlayer.BlowDown);
}

void KoopaCapPlayer::endEquipAndBlowDownWithoutHitReaction() {
    if (mEquipmentUser != nullptr)
        PlayerEquipmentFunction::endEquip(&mEquipmentUser);
    rs::tryCloseKoopaCapTutorial(this);
    al::hideSilhouetteModelIfShow(this);

    al::EnemyStateBlowDown* blowDownState = mBlowDownState;
    const sead::Vector3f& actorFront = al::getFront(this);
    sead::Vector3f front = {-actorFront.x, -actorFront.y, -actorFront.z};
    blowDownState->start(front);
    al::setNerve(this, &NrvKoopaCapPlayer.BlowDownWithoutHitReaction);
}

void KoopaCapPlayer::exeHideChase() {
    al::setTrans(this, *mHideChaseTrans);
}

void KoopaCapPlayer::exeCatchPrepare() {
    if (al::isFirstStep(this) && mBinder->startPrepareCatchKoopaCapAndTryStartPuppetImmediately(
                                     al::getHitSensor(this, "Bind"))) {
        al::showModelIfHide(this);
        al::setNerve(this, &NrvKoopaCapPlayer.Catch);
    }
}

void KoopaCapPlayer::exeCatch() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "CatchKoopaCap");
        al::showSilhouetteModelIfHide(this);
    }
    if (isPlayerLifeZero(this)) {
        endEquipAndBlowDown();
        return;
    }
    if (mBinder->isBinding())
        mBinder->copyPuppetQT(this);
    al::setNerveAtActionEnd(this, &NrvKoopaCapPlayer.Start);
}

void KoopaCapPlayer::exeStart() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "MarioStart");
        al::offCollide(this);
    }
    if (isPlayerLifeZero(this)) {
        endEquipAndBlowDown();
        return;
    }
    updatePoseFromPlayerEquipment(this, mEquipmentUser);
    if (al::isActionEnd(this)) {
        if (mIsAppearTutorialNoMovie)
            rs::tryAppearKoopaCapTutorialNoMovie(this);
        else
            rs::tryAppearKoopaCapTutorial(this);
        al::setNerve(this, &NrvKoopaCapPlayer.Wait);
    }
}

// NONMATCHING: https://decomp.me/scratch/esPLW
void KoopaCapPlayer::exeWait() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "MarioWait");
        mPunchState.focusTarget = nullptr;
        if (mBinder->isBinding())
            mBinder->endPunchBind();
        if (mJointSpringControlRate < 0.0f)
            mJointSpringControlRate = 0.01f;
    }

    if (isPlayerLifeZero(this)) {
        endEquipAndBlowDown();
        return;
    }

    updatePoseFromPlayerEquipment(this, mEquipmentUser);
    updateTriggerInputs(this);

    if (rs::isPlayerSafetyPointRecovery(this)) {
        al::setNerve(this, &NrvKoopaCapPlayer.WaitBubble);
        return;
    }

    if (rs::isPlayerCameraSubjective(this))
        return;

    if (!mPunchState.trySetupNextPunchFromInput(false))
        return;

    al::setNerve(this, &NrvKoopaCapPlayer.PunchStart);
}

void KoopaCapPlayer::exeWaitBubble() {
    if (al::isFirstStep(this))
        al::startAction(this, "WaitBubble");

    if (isPlayerLifeZero(this)) {
        endEquipAndBlowDown();
        return;
    }

    updatePoseFromPlayerEquipment(this, mEquipmentUser);
    if (!rs::isPlayerSafetyPointRecovery(this))
        al::setNerve(this, &NrvKoopaCapPlayer.Start);
}

void KoopaCapPlayer::exePunchStart() {
    if (al::isFirstStep(this)) {
        mPunchState.isPunchHit = false;
        mPunchState.isPunchHitReaction = false;
        mPunchState.isTriggerSwingLeft = false;
        mPunchState.isTriggerSwingRight = false;
        mPunchState.isTriggerCapAction = false;
        al::startAction(this, mPunchState.isPunchLeft ? "PunchLStart" : "PunchRStart");
        if (mPunchState.isPunchFollowUp)
            al::setActionFrameRate(this,
                                   al::lerpValue(0.7f, 0.9f, mPunchState.fastPunchRate) * 0.75f);
        else
            al::setActionFrameRate(this, mPunchState.isFastPunchInput ? 1.0f : 0.75f);
        mJointSpringControlRate = -1.0f;
    }

    if (isPlayerLifeZero(this)) {
        endEquipAndBlowDown();
        return;
    }
    updatePoseFromPlayerEquipment(this, mEquipmentUser);
    updateTriggerInputs(this);
    if (rs::isPlayerSafetyPointRecovery(this)) {
        al::setNerve(this, &NrvKoopaCapPlayer.WaitBubble);
    } else if (al::isActionEnd(this)) {
        if (mPunchState.isPunchFollowUp || mPunchState.isFastPunchInput)
            al::setNerve(this, &NrvKoopaCapPlayer.Punch);
        else
            al::setNerve(this, &NrvKoopaCapPlayer.PunchWait);
    }
}

void KoopaCapPlayer::exePunchWait() {
    if (isPlayerLifeZero(this)) {
        endEquipAndBlowDown();
        return;
    }
    updatePoseFromPlayerEquipment(this, mEquipmentUser);
    updateTriggerInputs(this);
    if (rs::isPlayerSafetyPointRecovery(this))
        al::setNerve(this, &NrvKoopaCapPlayer.WaitBubble);
    else
        al::setNerveAtGreaterEqualStep(this, &NrvKoopaCapPlayer.Punch, 3);
}

void KoopaCapPlayer::exePunch() {
    if (al::isFirstStep(this)) {
        al::startAction(this, mPunchState.isPunchLeft ? "PunchL" : "PunchR");
        al::setActionFrameRate(this,
                               mPunchState.isPunchFollowUp ?
                                   al::lerpValue(0.7f, 0.9f, mPunchState.fastPunchRate) * 2.0f :
                                   2.0f);
        mBinder->tryStartPunchAction(mPunchState.isPunchLeft);
    }

    if (isPlayerLifeZero(this)) {
        endEquipAndBlowDown();
        return;
    }
    updatePoseFromPlayerEquipment(this, mEquipmentUser);
    updateTriggerInputs(this);
    if (rs::isPlayerSafetyPointRecovery(this)) {
        al::setNerve(this, &NrvKoopaCapPlayer.WaitBubble);
        return;
    }

    if (mPunchState.isPunchHit) {
        s32 punchRestartStep = 8;
        if (mPunchState.isTriggerSwingLeft || mPunchState.isTriggerSwingRight)
            punchRestartStep = 7;
        if (al::isGreaterEqualStep(this, punchRestartStep) &&
            tryStartNextPunchFromInput(this, &mPunchState, mBinder))
            return;
    }

    if (al::isActionEnd(this)) {
        mPunchState.isPunchFollowUp = false;
        al::setNerve(this, &NrvKoopaCapPlayer.PunchEnd);
    }
}

void KoopaCapPlayer::endPunch() {
    if (mBinder->isBinding() && !mPunchState.isPunchFollowUp)
        mBinder->endPunchBind();
}

void KoopaCapPlayer::exePunchEnd() {
    if (al::isFirstStep(this))
        al::startAction(this, mPunchState.isPunchLeft ? "PunchLEnd" : "PunchREnd");

    if (isPlayerLifeZero(this)) {
        endEquipAndBlowDown();
        return;
    }
    updatePoseFromPlayerEquipment(this, mEquipmentUser);
    updateTriggerInputs(this);
    if (rs::isPlayerSafetyPointRecovery(this))
        al::setNerve(this, &NrvKoopaCapPlayer.WaitBubble);
    else if (!tryStartNextPunchFromInput(this, &mPunchState, mBinder))
        al::setNerveAtActionEnd(this, &NrvKoopaCapPlayer.Wait);
}

void KoopaCapPlayer::exePunchFinishStart() {
    if (al::isFirstStep(this)) {
        if (mBinder->isBinding())
            mBinder->startPunchFinishKeepBind(mPunchState.isPunchLeft);
        mPunchState.isPunchHit = false;
        mPunchState.isPunchHitReaction = false;
        mPunchState.isTriggerSwingLeft = false;
        mPunchState.isTriggerSwingRight = false;
        mPunchState.isTriggerCapAction = false;
        al::startAction(this, mPunchState.isPunchLeft ? "PunchLFinishStart" : "PunchRFinishStart");
        mJointSpringControlRate = -1.0f;
    }

    if (isPlayerLifeZero(this)) {
        endEquipAndBlowDown();
        return;
    }
    updatePoseFromPlayerEquipment(this, mEquipmentUser);
    updateTriggerInputs(this);
    if (rs::isPlayerSafetyPointRecovery(this)) {
        al::setNerve(this, &NrvKoopaCapPlayer.WaitBubble);
    } else if (al::isActionEnd(this)) {
        if (mPunchState.isPunchFollowUp || mPunchState.isFastPunchInput)
            al::setNerve(this, &NrvKoopaCapPlayer.Punch);
        else
            al::setNerve(this, &NrvKoopaCapPlayer.PunchWait);
    }
}

void KoopaCapPlayer::exePunchFinish() {
    if (al::isFirstStep(this)) {
        al::startAction(this, mPunchState.isPunchLeft ? "PunchLFinish" : "PunchRFinish");
        if (mPunchState.isPunchLeft)
            mRumble->keepScaleL = true;
        else
            mRumble->keepScaleR = true;
    }

    if (al::isStep(this, 2)) {
        mRumble->clearKeepScale();
        KoopaCapPlayerRumble* rumble = mRumble;
        if (mPunchState.isPunchLeft) {
            rumble->left->setParam(5.5f, 2.0f, 0.5f, 45);
            rumble->left->start(0);
        } else {
            rumble->right->setParam(5.5f, 2.0f, 0.5f, 45);
            rumble->right->start(0);
        }
    }

    if (isPlayerLifeZero(this)) {
        endEquipAndBlowDown();
        return;
    }
    updatePoseFromPlayerEquipment(this, mEquipmentUser);
    if (rs::isPlayerSafetyPointRecovery(this)) {
        al::setNerve(this, &NrvKoopaCapPlayer.WaitBubble);
    } else if (al::isActionEnd(this)) {
        bool hasHitReaction = mPunchState.isPunchHitReaction;
        mPunchState.isPunchFollowUp = false;
        if (!hasHitReaction) {
            endEquipAndBlowDownWithoutHitReaction();
            return;
        }
        mBinder->tryCancelPunchFinishBind();
        al::setNerve(this, &NrvKoopaCapPlayer.PunchFinishWait);
    }
}

void KoopaCapPlayer::exePunchFinishWait() {
    if (isPlayerLifeZero(this)) {
        endEquipAndBlowDown();
        return;
    }
    updatePoseFromPlayerEquipment(this, mEquipmentUser);
    if (rs::isPlayerSafetyPointRecovery(this))
        al::setNerve(this, &NrvKoopaCapPlayer.WaitBubble);
    else if (al::isGreaterEqualStep(this, mPunchState.isPunchHitReaction ? 0 : 60))
        al::setNerve(this, &NrvKoopaCapPlayer.PunchFinishEnd);
}

void KoopaCapPlayer::exePunchFinishEnd() {
    if (al::isFirstStep(this))
        al::startAction(this, mPunchState.isPunchLeft ? "PunchLFinishEnd" : "PunchRFinishEnd");

    if (isPlayerLifeZero(this)) {
        endEquipAndBlowDown();
        return;
    }
    updatePoseFromPlayerEquipment(this, mEquipmentUser);
    if (rs::isPlayerSafetyPointRecovery(this))
        al::setNerve(this, &NrvKoopaCapPlayer.WaitBubble);
    else if (al::isActionEnd(this))
        al::setNerve(this, &NrvKoopaCapPlayer.Wait);
}

void KoopaCapPlayer::exeDamage() {
    if (al::isFirstStep(this))
        al::startAction(this, "Damage");

    if (isPlayerLifeZero(this)) {
        endEquipAndBlowDown();
        return;
    }
    updatePoseFromPlayerEquipment(this, mEquipmentUser);
    if (rs::isPlayerSafetyPointRecovery(this))
        al::setNerve(this, &NrvKoopaCapPlayer.WaitBubble);
    else
        al::setNerveAtActionEnd(this, &NrvKoopaCapPlayer.Wait);
}

void KoopaCapPlayer::exeBlowDown() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "KoopaWait");
        if (al::isNerve(this, &NrvKoopaCapPlayer.BlowDown))
            al::startHitReactionBlowHit(this);
    }

    if (al::updateNerveState(this)) {
        al::offCollide(this);
        al::startHitReaction(this, "消滅");
        kill();
        return;
    }

    KoopaFunction::updateCapBlowDownSideDegree(&mCapBlowDownSideDegree, this);
}

void KoopaCapPlayer::endBlowDown() {
    mCapBlowDownSideDegree = 0.0f;
}
