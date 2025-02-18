#include "MapObj/AnagramAlphabetCharacter.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
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
#include "Util/JudgeUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(AnagramAlphabetCharacter, Wait);
NERVE_IMPL(AnagramAlphabetCharacter, WaitHack);
NERVE_IMPL(AnagramAlphabetCharacter, HackEnd);
NERVE_IMPL(AnagramAlphabetCharacter, HackStart);
NERVE_IMPL(AnagramAlphabetCharacter, WaitHackStart);
NERVE_IMPL(AnagramAlphabetCharacter, Complete);
NERVE_IMPL(AnagramAlphabetCharacter, HackWait);
NERVE_IMPL(AnagramAlphabetCharacter, HackMove);
NERVE_IMPL(AnagramAlphabetCharacter, HackFall);
NERVE_IMPL(AnagramAlphabetCharacter, HackGoal);
NERVE_IMPL(AnagramAlphabetCharacter, Set);

NERVES_MAKE_NOSTRUCT(AnagramAlphabetCharacter, HackStart);
NERVES_MAKE_STRUCT(AnagramAlphabetCharacter, Wait, WaitHack, HackEnd, WaitHackStart, Complete,
                   HackWait, HackMove, HackFall, HackGoal, Set);
}  // namespace

AnagramAlphabetCharacter::AnagramAlphabetCharacter(const char* name) : al::LiveActor(name) {}

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

void AnagramAlphabetCharacter::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (mHackerParent)
        return rs::sendMsgHackerNoReaction(mHackerParent, other, self);
    if (!al::isSensorNpc(self))
        return;
    if (al::isSensorPlayer(other) && al::isSensorName(other, "Pushed"))
        al::sendMsgPush(other, self);
    else if (al::isSensorNpc(other))
        al::sendMsgPush(other, self);
}

bool AnagramAlphabetCharacter::isHack() {
    return al::isNerve(this, &NrvAnagramAlphabetCharacter.HackWait) ||
           al::isNerve(this, &NrvAnagramAlphabetCharacter.HackMove) ||
           al::isNerve(this, &NrvAnagramAlphabetCharacter.HackFall) ||
           al::isNerve(this, &NrvAnagramAlphabetCharacter.HackGoal);
}

bool AnagramAlphabetCharacter::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                          al::HitSensor* self) {
    if (rs::tryReceiveMsgInitCapTargetAndSetCapTargetInfo(message, mCapTargetInfo))
        return true;

    if (rs::isMsgPlayerDisregardHomingAttack(message) ||
        rs::isMsgPlayerDisregardTargetMarker(message))
        return al::isSensorName(self, "Body");

    if (rs::isMsgTargetMarkerPosition(message) && al::isSensorName(self, "PossessedHitMark")) {
        const sead::Vector3f& sensorPos = al::getSensorPos(this, "PossessedHitMark");
        rs::setMsgTargetMarkerPosition(message, sead::Vector3f::ey * 60.0f + sensorPos);
        return true;
    }

    if (rs::isMsgCapKeepLockOn(message))
        return al::isNerve(this, &NrvAnagramAlphabetCharacter.WaitHack);

    if (rs::isMsgCapCancelLockOn(message)) {
        if (al::isNerve(this, &NrvAnagramAlphabetCharacter.WaitHack)) {
            al::setNerve(this, &NrvAnagramAlphabetCharacter.Wait);
            return true;
        }
        return false;
    }

    if (rs::isMsgCancelHack(message)) {
        if (isHack()) {
            endHack();
            return true;
        }
        return false;
    }
    if (rs::isMsgHackMarioDead(message) || rs::isMsgHackMarioDemo(message) ||
        rs::isMsgHackMarioInWater(message) || rs::isMsgHackMarioCheckpointFlagWarp(message)) {
        endHack();
        return true;
    }

    if (rs::isMsgHackerDamageAndCancel(message)) {
        if (isHack()) {
            sead::Vector3f dir;
            al::calcDirBetweenSensorsH(&dir, other, self);
            endHackDir(dir);
            return true;
        }
        return false;
    }

    if (rs::isMsgStartHack(message)) {
        if (al::isNerve(this, &NrvAnagramAlphabetCharacter.WaitHack)) {
            al::setNerve(this, &NrvAnagramAlphabetCharacter.WaitHackStart);
            al::invalidateClipping(this);
            mHackerParent = rs::startHack(self, other, nullptr);
            rs::startHackStartDemo(mHackerParent, this);
            return true;
        } else
            return false;
    }

    if (rs::isMsgCapStartLockOn(message)) {
        if (al::isNerve(this, &NrvAnagramAlphabetCharacter.Complete))
            return false;

        if (al::isSensorName(self, "PossessedHitMark")) {
            if (al::isNerve(this, &NrvAnagramAlphabetCharacter.Wait))
                al::setNerve(this, &NrvAnagramAlphabetCharacter.WaitHack);

            return true;
        }

        return false;
    }

    if (rs::isMsgEnableMapCheckPointWarpCollidedGround(message, this))
        return true;

    if ((al::isNerve(this, &NrvAnagramAlphabetCharacter.HackWait) ||
         al::isNerve(this, &NrvAnagramAlphabetCharacter.HackMove) ||
         al::isNerve(this, &NrvAnagramAlphabetCharacter.HackFall)) &&
        al::tryReceiveMsgPushAndAddVelocityH(this, message, other, self, 10.0f)) {
        if (al::isCollidedWall(this))
            al::limitVelocityDirSign(this, -al::getCollidedWallNormal(this), 0.0f);

        return true;
    }

    return false;
}

void AnagramAlphabetCharacter::setComplete() {
    al::setNerve(this, &NrvAnagramAlphabetCharacter.Complete);
}

void AnagramAlphabetCharacter::killCapTarget() {
    mCapTargetParts->kill();
    al::invalidateHitSensors(this);
}

void AnagramAlphabetCharacter::exeWait() {
    if (al::isFirstStep(this)) {
        al::setVelocityZero(this);
        al::startAction(this, "Wait");
        mCapTargetParts->startNormal();
    }
}

void AnagramAlphabetCharacter::exeWaitHack() {
    if (al::isFirstStep(this))
        mCapTargetParts->startSwoon();
}

void AnagramAlphabetCharacter::exeWaitHackStart() {
    if (rs::isHackStartDemoEnterMario(mHackerParent))
        al::setNerve(this, &HackStart);
}

void AnagramAlphabetCharacter::exeHackStart() {
    if (al::isFirstStep(this)) {
        mCapTargetParts->startHack();
        al::startAction(this, "HackStart");
    }

    if (al::isStep(this, 10))
        mPlayerHackStartShaderCtrl->start();
    mPlayerHackStartShaderCtrl->update();

    if (al::isGreaterEqualStep(this, 60)) {
        mPlayerHackStartShaderCtrl->end();
        rs::endHackStartDemo(mHackerParent, this);
        al::onCollide(this);
        al::setNerve(this, &NrvAnagramAlphabetCharacter.HackWait);
    }
}

void AnagramAlphabetCharacter::exeHackWait() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Wait");
        rs::resetJudge(mHackerJudgeNormalFall);
    }

    al::addVelocityToGravity(this, 1.5f);
    al::reboundVelocityFromCollision(this, 0.0f, 0.0f, 0.0f);
    al::scaleVelocity(this, 0.7f);

    if (rs::updateJudgeAndResult(mHackerJudgeNormalFall))
        al::setNerve(this, &NrvAnagramAlphabetCharacter.HackFall);

    else if (mParent->testBase(this))
        al::setNerve(this, &NrvAnagramAlphabetCharacter.HackGoal);

    else if (rs::updateJudgeAndResult(mHackerJudgeStartRun))
        al::setNerve(this, &NrvAnagramAlphabetCharacter.HackMove);
}

void AnagramAlphabetCharacter::exeHackMove() {
    if (al::isFirstStep(this)) {
        rs::resetJudge(mHackerJudgeNormalFall);
        mSwingTimer = 0;
    }

    if (rs::isTriggerHackSwing(mHackerParent))
        mSwingTimer = 60;
    else if (mSwingTimer >= 1)
        mSwingTimer--;

    al::addVelocityToGravity(this, 1.5f);
    al::scaleVelocity(this, 0.7f);

    sead::Vector3f dir = sead::Vector3f(0.0f, 0.0f, 0.0f);
    f32 runningAccel = rs::isHoldHackAction(mHackerParent) ? 4.0f : 2.0f;

    f32 accel = mSwingTimer > 0 ? runningAccel + 2.0f : runningAccel;

    if (accel != 2.0f) {
        if (!al::isActionPlaying(this, "Dash"))
            al::startAction(this, "Dash");
    } else {
        if (!al::isActionPlaying(this, "Walk"))
            al::startAction(this, "Walk");
    }

    rs::addHackActorAccelStick(this, mHackerParent, &dir, accel, sead::Vector3f::ey);
    al::turnToDirection(this, dir, 5.0f);
    al::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);

    if (rs::updateJudgeAndResult(mHackerJudgeNormalFall))
        al::setNerve(this, &NrvAnagramAlphabetCharacter.HackFall);

    else if (mParent->testBase(this))
        al::setNerve(this, &NrvAnagramAlphabetCharacter.HackGoal);

    else if (rs::isHackerStopMove(this, mHackerParent, 1.5f))
        al::setNerve(this, &NrvAnagramAlphabetCharacter.HackWait);
}

void AnagramAlphabetCharacter::exeHackFall() {
    if (al::isFirstStep(this)) {
    }  // unused call

    al::addVelocityToGravity(this, 1.5f);
    al::scaleVelocity(this, 0.99f);

    if (al::isOnGround(this, 0)) {
        al::startHitReaction(this, "着地");
        al::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
        if (rs::updateJudgeAndResult(mHackerJudgeStartRun))
            al::setNerve(this, &NrvAnagramAlphabetCharacter.HackMove);
        else
            al::setNerve(this, &NrvAnagramAlphabetCharacter.HackWait);
    }
}

void AnagramAlphabetCharacter::exeHackEnd() {
    al::addVelocityToGravity(this, 1.5f);
    al::scaleVelocity(this, 0.99f);

    if (al::isOnGround(this, 0)) {
        al::offCollide(this);
        al::startHitReaction(this, "着地");
        al::setVelocityZero(this);
        al::setNerve(this, &NrvAnagramAlphabetCharacter.Wait);
    }
}

void AnagramAlphabetCharacter::exeHackGoal() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Wait");
        al::offCollide(this);
        al::setVelocityZero(this);
        al::startHitReaction(this, "ゴールデモ開始");
    }

    sead::Vector3f front = {mPoseMatrix->m[0][2], mPoseMatrix->m[1][2], mPoseMatrix->m[2][2]};

    sead::Vector3f pos;
    mPoseMatrix->getTranslation(pos);

    al::turnToDirection(this, front, 20.0f);
    al::lerpVec(al::getTransPtr(this), al::getTrans(this), pos, 0.5f);

    if (al::isGreaterEqualStep(this, 0)) {
        al::updatePoseMtx(this, mPoseMatrix);

        endHackDir(front);

        mParent->testEndHack();
        al::setNerve(this, &NrvAnagramAlphabetCharacter.Set);
        al::startHitReaction(this, "ゴールデモ終了");
    }
}

void AnagramAlphabetCharacter::exeSet() {
    if (al::isFirstStep(this))
        al::startAction(this, "Set");

    if (al::isActionEnd(this)) {
        if (!mParent->testComplete())
            al::setNerve(this, &NrvAnagramAlphabetCharacter.Wait);
    }
}

void AnagramAlphabetCharacter::exeComplete() {}

void AnagramAlphabetCharacter::endHack() {
    CapTargetParts* capTargetParts = mCapTargetParts;
    rs::endHack(&mHackerParent);
    al::validateClipping(this);
    al::setNerve(this, &NrvAnagramAlphabetCharacter.HackEnd);
    capTargetParts->startNormal();
}

void AnagramAlphabetCharacter::endHackDir(const sead::Vector3f& dir) {
    CapTargetParts* capTargetParts = mCapTargetParts;
    rs::endHackDir(&mHackerParent, dir);
    al::validateClipping(this);
    al::setNerve(this, &NrvAnagramAlphabetCharacter.HackEnd);
    capTargetParts->startNormal();
}
