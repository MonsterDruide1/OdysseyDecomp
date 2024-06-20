#include "MapObj/AnagramAlphabetCharacter.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/LiveActor/SubActorKeeper.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "MapObj/AnagramAlphabet.h"
#include "MapObj/CapTargetParts.h"
#include "Player/HackerJudge.h"
#include "Player/PlayerHackStartShaderCtrl.h"
#include "Util/Hack.h"
#include "Util/StageSceneFunction.h"

namespace {
NERVE_IMPL(AnagramAlphabetCharacter, Wait);
NERVE_IMPL(AnagramAlphabetCharacter, WaitHack);
NERVE_IMPL(AnagramAlphabetCharacter, HackEnd);
NERVE_IMPL_(AnagramAlphabetCharacter, HackStart, WaitHackStart);
NERVE_IMPL(AnagramAlphabetCharacter, Complete);
NERVE_IMPL(AnagramAlphabetCharacter, HackWait);
NERVE_IMPL(AnagramAlphabetCharacter, HackMove);
NERVE_IMPL(AnagramAlphabetCharacter, HackFall);
NERVE_IMPL(AnagramAlphabetCharacter, HackGoal);
NERVE_IMPL(AnagramAlphabetCharacter, Set);

NERVES_MAKE_STRUCT(AnagramAlphabetCharacter, Wait, WaitHack, HackEnd, HackStart, Complete, HackWait,
                   HackMove, HackFall, HackGoal, Set);
}  // namespace

void AnagramAlphabetCharacter::init(const al::ActorInitInfo& info) {
    al::initActorChangeModel(this, info);
    al::initNerve(this, &NrvAnagramAlphabetCharacter.Wait, 0);
    al::initSubActorKeeperNoFile(this, info, 1);
    mCapTargetInfo = rs::createCapTargetInfo(this, 0);
    mCapTargetParts = new CapTargetParts(this, info);
    al::invalidateClipping(mCapTargetParts);
    al::registerSubActorSyncClipping(this, mCapTargetParts);
    al::onSyncAlphaMaskSubActor(this, mCapTargetParts);
    al::createAndSetColliderSpecialPurpose(this, "Alphabet");
    mHackerJudgeNormalFall = new HackerJudgeNormalFall(this, 5);
    mHackerJudgeStartRun = new HackerJudgeStartRun(this, &mHackerParent);
    mPlayerHackStartShaderCtrl = new PlayerHackStartShaderCtrl(this, 0);
    al::startAction(this, "Wait");
    al::offCollide(this);
    makeActorAlive();
}

// NON_MATCHING
void AnagramAlphabetCharacter::attackSensor(al::HitSensor* target, al::HitSensor* source) {
    if (mHackerParent)
        return rs::sendMsgHackerNoReaction(mHackerParent, source, target);
    if (!al::isSensorNpc(target))
        return;
    if (al::isSensorPlayer(source) && al::isSensorName(source, "Pushed"))
        al::sendMsgPush(source, target);
    else if (al::isSensorNpc(source))
        al::sendMsgPush(source, target);
}

bool AnagramAlphabetCharacter::receiveMsg(const al::SensorMsg*, al::HitSensor*, al::HitSensor*) {
    return 0;
}

void AnagramAlphabetCharacter::setComplete() {
    al::setNerve(this, &NrvAnagramAlphabetCharacter.Complete);
}

void AnagramAlphabetCharacter::killCapTarget() {
    mCapTargetParts->kill();
    al::invalidateHitSensors(this);
}

void AnagramAlphabetCharacter::exeWait() {
    if (!al::isFirstStep(this))
        return;
    al::setVelocityZero(this);
    al::startAction(this, "Wait");
    mCapTargetParts->startNormal();
}

void AnagramAlphabetCharacter::exeWaitHack() {
    if (al::isFirstStep(this))
        mCapTargetParts->startSwoon();
}

// AnagramAlphabetCharacter::exeWaitHackStart()

void AnagramAlphabetCharacter::exeHackStart() {
    if (al::isFirstStep(this)) {
        mCapTargetParts->startHack();
        al::startAction(this, "HackStart");
    }
    if (al::isStep(this, 10))
        mPlayerHackStartShaderCtrl->start();
    mPlayerHackStartShaderCtrl->update();
    if (!al::isGreaterEqualStep(this, 60))
        return;
    mPlayerHackStartShaderCtrl->end();
    rs::endHackStartDemo(mHackerParent, this);
    al::onCollide(this);
    al::setNerve(this, &NrvAnagramAlphabetCharacter.HackWait);
}

void AnagramAlphabetCharacter::exeHackWait() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Wait");
        rs::resetJudge((IJudge*)mHackerJudgeNormalFall);
    }
    al::addVelocityToGravity(this, 1.5);
    al::reboundVelocityFromCollision(this, 0.0, 0.0, 0.0);
    al::scaleVelocity(this, 0.7);

    if (rs::updateJudgeAndResult((IJudge*)mHackerJudgeNormalFall))
        al::setNerve(this, &NrvAnagramAlphabetCharacter.HackFall);

    else if (mParent->testBase(this))
        al::setNerve(this, &NrvAnagramAlphabetCharacter.HackGoal);

    else {
        if (!rs::updateJudgeAndResult((IJudge*)mHackerJudgeStartRun))
            return;
        al::setNerve(this, &NrvAnagramAlphabetCharacter.HackMove);
    }
}

void AnagramAlphabetCharacter::exeHackMove() {}

void AnagramAlphabetCharacter::exeHackFall() {
    al::isFirstStep(this);
    al::addVelocityToGravity(this, 1.5);
    al::scaleVelocity(this, 0.99);
    if (!al::isOnGround(this, 0))
        return;
    al::startHitReaction(this, "着地");
    al::reboundVelocityFromCollision(this, 0.0, 0.0, 1.0);
    // TODO: Remove this cast
    if (rs::updateJudgeAndResult((IJudge*)mHackerJudgeStartRun))
        al::setNerve(this, &NrvAnagramAlphabetCharacter.HackMove);
    else
        al::setNerve(this, &NrvAnagramAlphabetCharacter.HackWait);
}

void AnagramAlphabetCharacter::exeHackEnd() {
    al::addVelocityToGravity(this, 1.5);
    al::scaleVelocity(this, 0.99);
    if (!al::isOnGround(this, 0))
        return;
    al::offCollide(this);
    al::startHitReaction(this, "着地");
    al::setVelocityZero(this);
    al::setNerve(this, &NrvAnagramAlphabetCharacter.Wait);
}

// NON_MATCHING
void AnagramAlphabetCharacter::exeHackGoal() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Wait");
        al::offCollide(this);
        al::setVelocityZero(this);
        al::startHitReaction(this, "ゴールデモ開始");
    }
    sead::Vector3f rot;
    unkMtx->getRotation(rot);

    sead::Vector3f pos;
    unkMtx->getTranslation(pos);

    al::turnToDirection(this, rot, 20.0);
    auto transPtr = al::getTransPtr(this);
    auto trans = al::getTrans(this);
    al::lerpVec(transPtr, trans, pos, 0.5);
    if (al::isGreaterEqualStep(this, 0LL))
        return;
    al::updatePoseMtx(this, unkMtx);
    rs::endHackDir(&mHackerParent, rot);
    al::validateClipping(this);
    al::setNerve(this, &NrvAnagramAlphabetCharacter.Wait);
    mCapTargetParts->startNormal();
    mParent->testEndHack();
    al::setNerve(this, &NrvAnagramAlphabetCharacter.Set);
    return al::startHitReaction(this, "ゴールデモ終了");
}

void AnagramAlphabetCharacter::exeSet() {
    if (al::isFirstStep(this))
        al::startAction(this, "Set");
    if (!al::isActionEnd(this))
        return;
    if (!mParent->testComplete())
        al::setNerve(this, &NrvAnagramAlphabetCharacter.Wait);
}

void AnagramAlphabetCharacter::exeComplete() {}
