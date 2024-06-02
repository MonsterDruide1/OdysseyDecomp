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
#include "Util/Sensor.h"
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

struct {
    NERVE_MAKE(AnagramAlphabetCharacter, Wait);
    NERVE_MAKE(AnagramAlphabetCharacter, WaitHack);
    NERVE_MAKE(AnagramAlphabetCharacter, HackEnd);
    NERVE_MAKE(AnagramAlphabetCharacter, HackStart);
    NERVE_MAKE(AnagramAlphabetCharacter, Complete);
    NERVE_MAKE(AnagramAlphabetCharacter, HackWait);
    NERVE_MAKE(AnagramAlphabetCharacter, HackMove);
    NERVE_MAKE(AnagramAlphabetCharacter, HackFall);
    NERVE_MAKE(AnagramAlphabetCharacter, HackGoal);
    NERVE_MAKE(AnagramAlphabetCharacter, Set);
} NrvAnagramAlphabetCharacter;

}  // namespace

void AnagramAlphabetCharacter::init(al::ActorInitInfo const& info) {
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

inline bool isHack(AnagramAlphabetCharacter* actor) {
    return al::isNerve(actor, &NrvAnagramAlphabetCharacter.HackWait) ||
           al::isNerve(actor, &NrvAnagramAlphabetCharacter.HackMove) ||
           al::isNerve(actor, &NrvAnagramAlphabetCharacter.HackFall) ||
           al::isNerve(actor, &NrvAnagramAlphabetCharacter.HackGoal);
}

bool AnagramAlphabetCharacter::receiveMsg(const al::SensorMsg* message, al::HitSensor* source,
                                          al::HitSensor* target) {
    if (rs::tryReceiveMsgInitCapTargetAndSetCapTargetInfo(message, mCapTargetInfo))
        return true;

    if (rs::isMsgPlayerDisregardHomingAttack(message) ||
        rs::isMsgPlayerDisregardTargetMarker(message)) {
        return al::isSensorName(target, "Body");
    }

    if (rs::isMsgTargetMarkerPosition(message) && al::isSensorName(target, "PossessedHitMark")) {
        sead::Vector3f sensorPos = al::getSensorPos(this, "PossessedHitMark");
        sead::Vector3f test = (sead::Vector3f::ey * 60.0f) + sensorPos;
        rs::setMsgTargetMarkerPosition(message, test);
        return true;
    }

    if (rs::isMsgCapKeepLockOn(message)) {
        return al::isNerve(this, &NrvAnagramAlphabetCharacter.WaitHack);
    }

    if (rs::isMsgCapCancelLockOn(message)) {
        if (!al::isNerve(this, &NrvAnagramAlphabetCharacter.WaitHack)) {
            return false;
        }
        al::setNerve(this, &NrvAnagramAlphabetCharacter.Wait);
        return true;
    }

    if (rs::isMsgCancelHack(message)) {
        if (!isHack(this)) {
            return false;
        }
    }
    if (rs::isMsgHackMarioDead(message) || rs::isMsgHackMarioDemo(message) ||
        rs::isMsgHackMarioInWater(message) || rs::isMsgHackMarioCheckpointFlagWarp(message)) {
        CapTargetParts* capTargetParts = getCapTargetParts();
        rs::endHack(&mHackerParent);
        al::validateClipping(this);
        al::setNerve(this, &NrvAnagramAlphabetCharacter.HackEnd);
        capTargetParts->startNormal();
        return true;
    }

    if (rs::isMsgHackerDamageAndCancel(message)) {
        if (!isHack(this)) {
            return false;
        }

        sead::Vector3f test2;
        al::calcDirBetweenSensorsH(&test2, source, target);
        CapTargetParts* capTargetParts = getCapTargetParts();
        rs::endHackDir(&mHackerParent, test2);
        al::validateClipping(this);
        al::setNerve(this, &NrvAnagramAlphabetCharacter.HackEnd);
        capTargetParts->startNormal();
        return true;
    }

    if (rs::isMsgStartHack(message)) {
        if (!al::isNerve(this, &NrvAnagramAlphabetCharacter.WaitHack)) {
            return false;
        }

        al::setNerve(this, &NrvAnagramAlphabetCharacter.HackStart);
        al::invalidateClipping(this);
        mHackerParent = rs::startHack(target, source, nullptr);
        rs::startHackStartDemo(mHackerParent, this);
        return true;
    }

    if (rs::isMsgCapStartLockOn(message)) {
        if (al::isNerve(this, &NrvAnagramAlphabetCharacter.Complete)) {
            return false;
        }

        if (al::isSensorName(target, "PossessedHitMark")) {
            if (!al::isNerve(this, &NrvAnagramAlphabetCharacter.Wait)) {
                return true;
            }

            al::setNerve(this, &NrvAnagramAlphabetCharacter.WaitHack);
            return true;
        }

        return false;
    }

    if (rs::isMsgEnableMapCheckPointWarpCollidedGround(message, this)) {
        return true;
    }

    if (al::isNerve(this, &NrvAnagramAlphabetCharacter.HackWait) ||
        al::isNerve(this, &NrvAnagramAlphabetCharacter.HackMove) ||
        (al::isNerve(this, &NrvAnagramAlphabetCharacter.HackFall) &&
         al::tryReceiveMsgPushAndAddVelocityH(this, message, source, target, 10.0f))) {
        if (!al::isCollidedWall(this)) {
            return true;
        }

        al::limitVelocityDirSign(this, -al::getCollidedWallNormal(this), 0.0);
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
        if (!rs::updateJudgeAndResult((IJudge*)this->mHackerJudgeStartRun))
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
    sead::Vector3f pos;
    unkMtx->getBase(rot, 2);
    unkMtx->getBase(pos, 3);

    al::turnToDirection(this, rot, 20.0f);
    al::lerpVec(al::getTransPtr(this), al::getTrans(this), pos, 0.5f);

    if (al::isGreaterEqualStep(this, 0)) {
        al::updatePoseMtx(this, unkMtx);
        CapTargetParts* capTargetParts = getCapTargetParts();
        rs::endHackDir(&mHackerParent, rot);
        al::validateClipping(this);
        al::setNerve(this, &NrvAnagramAlphabetCharacter.HackEnd);
        capTargetParts->startNormal();
        mParent->testEndHack();
        al::setNerve(this, &NrvAnagramAlphabetCharacter.Set);
        al::startHitReaction(this, "ゴールデモ終了");
    }
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
