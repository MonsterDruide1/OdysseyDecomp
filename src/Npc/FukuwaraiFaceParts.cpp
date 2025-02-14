#include "Npc/FukuwaraiFaceParts.h"

#include "Library/Area/AreaObjUtil.h"
#include "Library/Base/StringUtil.h"
#include "Library/Collision/KCollisionServer.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Se/SeFunction.h"

#include "Player/PlayerHackStartShaderCtrl.h"
#include "Util/Hack.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(FukuwaraiFaceParts, Appear)
NERVE_IMPL(FukuwaraiFaceParts, Wait)
NERVE_IMPL(FukuwaraiFaceParts, CaptureStart)
NERVE_IMPL(FukuwaraiFaceParts, Place)
NERVE_IMPL(FukuwaraiFaceParts, Reset)
NERVE_IMPL_(FukuwaraiFaceParts, Answer, Appear)
NERVE_IMPL(FukuwaraiFaceParts, Hide)
NERVE_IMPL_(FukuwaraiFaceParts, Vanish, Hide)
NERVE_IMPL(FukuwaraiFaceParts, CaptureMove)
NERVE_IMPL_(FukuwaraiFaceParts, CaptureMoveFast, CaptureMove)
NERVE_IMPL(FukuwaraiFaceParts, CaptureWait)
NERVE_IMPL(FukuwaraiFaceParts, CaptureHackStart)

NERVES_MAKE_NOSTRUCT(FukuwaraiFaceParts, Appear, Vanish, CaptureHackStart)
NERVES_MAKE_STRUCT(FukuwaraiFaceParts, Wait, CaptureStart, Place, Reset, Answer, Hide, CaptureMove,
                   CaptureMoveFast, CaptureWait)
}  // namespace

FukuwaraiPart KuriboEyebrowLeftPart = {"FukuwaraiKuriboEyebrowLeft", 4.0f, 12.0f, 4.0f};
FukuwaraiPart KuriboEyebrowRightPart = {"FukuwaraiKuriboEyebrowRight", 4.0f, 12.0f, 4.0f};
FukuwaraiPart KuriboEyeLeftPart = {"FukuwaraiKuriboEyeLeft", 4.0f, 12.0f, 4.0f};
FukuwaraiPart KuriboEyeRightPart = {"FukuwaraiKuriboEyeRight", 4.0f, 12.0f, 4.0f};
FukuwaraiPart KuriboMouthAngryPart = {"FukuwaraiKuriboMouthAngry", 4.0f, 12.0f, 4.0f};

FukuwaraiPart MarioEyebrowLeftPart = {"FukuwaraiMarioEyebrowLeft", 3.0f, 9.0f, 3.0f};
FukuwaraiPart MarioEyebrowRightPart = {"FukuwaraiMarioEyebrowRight", 3.0f, 9.0f, 3.0f};
FukuwaraiPart MarioEyeLeftPart = {"FukuwaraiMarioEyeLeft", 3.0f, 9.0f, 3.0f};
FukuwaraiPart MarioEyeRightPart = {"FukuwaraiMarioEyeRight", 3.0f, 9.0f, 3.0f};
FukuwaraiPart MarioMouthPart = {"FukuwaraiMarioMouth", 3.0f, 9.0f, 3.0f};
FukuwaraiPart MarioMustachePart = {"FukuwaraiMarioMustache", 3.0f, 9.0f, 3.0f};
FukuwaraiPart MarioNosePart = {"FukuwaraiMarioNose", 2.0f, 6.0f, 2.0f};

FukuwaraiFaceParts::FukuwaraiFaceParts(const char* name, al::AreaObjGroup* group)
    : al::LiveActor(name), mFukuwaraiArea(group) {}

void FukuwaraiFaceParts::init(const al::ActorInitInfo& info) {
    const char* suffix = nullptr;
    al::tryGetStringArg(&suffix, info, "Suffix");

    al::initMapPartsActor(this, info, suffix);
    al::initNerve(this, &Appear, 0);

    mCapTargetInfo = rs::createCapTargetInfo(this, suffix);

    al::CollisionPartsFilterSpecialPurpose* filter =
        new al::CollisionPartsFilterSpecialPurpose("MoveLimit");
    al::setColliderFilterCollisionParts(this, filter);

    if (al::isExistLinkChild(info, "TargetPos", 0)) {
        sead::Vector3f rotation = sead::Vector3f::zero;
        al::getLinkTR(&mTargetPos, &rotation, info, "TargetPos");
        if (rotation.y < 0.0f)
            rotation.y += 360.0f;
        mTargetAngle = rotation.y;
    }

    if (al::getRotate(this).y < 0.0f)
        al::setRotateY(this, al::getRotate(this).y + 360.0f);

    mTrans.set(al::getTrans(this));
    mRotation.set(al::getRotate(this));

    mPlayerHackStartShaderCtrl = new PlayerHackStartShaderCtrl(this, nullptr);

    makeActorDead();
}

s32 calculateFukuwaraiPartPriority(const char* name) {
    if (al::isEqualString(name, "FukuwaraiKuriboMouthAngry"))
        return 0;
    if (al::isEqualString(name, "FukuwaraiMarioMouth"))
        return 1;
    if (al::isEqualString(name, "FukuwaraiKuriboEyeRight"))
        return 2;
    if (al::isEqualString(name, "FukuwaraiKuriboMarioEyeRight"))
        return 3;
    if (al::isEqualString(name, "FukuwaraiMarioEyeRight"))
        return 4;
    if (al::isEqualString(name, "FukuwaraiKuriboEyeLeft"))
        return 5;
    if (al::isEqualString(name, "FukuwaraiKuriboMarioEyeLeft"))
        return 6;
    if (al::isEqualString(name, "FukuwaraiMarioEyeLeft"))
        return 7;
    if (al::isEqualString(name, "FukuwaraiKuriboEyebrowRight"))
        return 8;
    if (al::isEqualString(name, "FukuwaraiMarioEyebrowRight"))
        return 9;
    if (al::isEqualString(name, "FukuwaraiKuriboEyebrowLeft"))
        return 10;
    if (al::isEqualString(name, "FukuwaraiMarioEyebrowLeft"))
        return 11;
    if (al::isEqualString(name, "FukuwaraiMarioMustache"))
        return 12;
    if (al::isEqualString(name, "FukuwaraiMarioNose"))
        return 13;
    return -1;
}

s32 FukuwaraiFaceParts::getPriority() const {
    return calculateFukuwaraiPartPriority(al::getModelName(this));
}

void FukuwaraiFaceParts::control() {}

bool FukuwaraiFaceParts::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                    al::HitSensor* self) {
    if (rs::tryReceiveMsgInitCapTargetAndSetCapTargetInfo(message, mCapTargetInfo))
        return true;

    if (rs::isMsgCapEnableLockOn(message))
        return al::isNerve(this, &NrvFukuwaraiFaceParts.Wait);

    if (al::isSensorName(self, "Body")) {
        if (al::isNerve(this, &NrvFukuwaraiFaceParts.Wait)) {
            if (al::isMsgPlayerDisregard(message) || rs::isMsgPlayerDisregardHomingAttack(message))
                return false;
        } else {
            if (al::isMsgPlayerDisregard(message) || rs::isMsgPlayerDisregardHomingAttack(message))
                return true;
        }

        if (rs::isMsgPlayerDisregardTargetMarker(message))
            return true;
    }

    if (al::isNerve(this, &NrvFukuwaraiFaceParts.Wait)) {
        if (rs::isMsgCapCancelLockOn(message))
            return true;

        if (rs::isMsgStartHack(message)) {
            al::invalidateClipping(this);
            mIUsePlayerHack = rs::startHack(self, other, nullptr);
            rs::startHackStartDemo(mIUsePlayerHack, this);
            al::setNerve(this, &NrvFukuwaraiFaceParts.CaptureStart);
            return true;
        }

        return false;
    }

    if (al::isNerve(this, &NrvFukuwaraiFaceParts.CaptureStart))
        return false;

    if (mIUsePlayerHack && (rs::isMsgCancelHack(message) || rs::isMsgHackMarioDemo(message) ||
                            rs::isMsgHackMarioDead(message))) {
        rs::endHack(&mIUsePlayerHack);
        al::validateClipping(this);
        if (al::isInAreaObj(mFukuwaraiArea, al::getTrans(this))) {
            sead::Vector3f trans = al::getTrans(this);
            trans.y = mTrans.y + calculateFukuwaraiPartPriority(al::getModelName(this)) * 0.25f;
            al::offCollide(this);
            al::resetPosition(this, trans);

            al::setNerve(this, &NrvFukuwaraiFaceParts.Place);
        } else {
            al::setNerve(this, &NrvFukuwaraiFaceParts.Reset);
        }
        return true;
    }

    return false;
}

// NON_MATCHING
f32 FukuwaraiFaceParts::calcScore(bool isMario) const {
    if (!isPlaced())
        return 0.0f;

    const char* name = al::getModelName(this);
    FukuwaraiPart bodyPart;

    if (isMario) {
        if (al::isEqualString(name, MarioEyebrowLeftPart.name))
            bodyPart = MarioEyebrowLeftPart;
        else if (al::isEqualString(name, MarioEyebrowRightPart.name))
            bodyPart = MarioEyebrowRightPart;
        else if (al::isEqualString(name, MarioEyeLeftPart.name))
            bodyPart = MarioEyeLeftPart;
        else if (!al::isEqualString(name, MarioEyeRightPart.name))
            bodyPart = MarioEyeRightPart;
        else if (al::isEqualString(name, MarioMouthPart.name))
            bodyPart = MarioMouthPart;
        else if (al::isEqualString(name, MarioMustachePart.name))
            bodyPart = MarioMustachePart;
        else if (al::isEqualString(name, MarioNosePart.name))
            bodyPart = MarioNosePart;
        else
            return -5.0f;
    } else {
        if (al::isEqualString(name, KuriboEyebrowLeftPart.name))
            bodyPart = KuriboEyebrowLeftPart;
        else if (al::isEqualString(name, KuriboEyebrowRightPart.name))
            bodyPart = KuriboEyebrowRightPart;
        else if (al::isEqualString(name, KuriboEyeLeftPart.name))
            bodyPart = KuriboEyeLeftPart;
        else if (al::isEqualString(name, KuriboEyeRightPart.name))
            bodyPart = KuriboEyeRightPart;
        else if (al::isEqualString(name, KuriboMouthAngryPart.name))
            bodyPart = KuriboMouthAngryPart;
        else
            return -5.0f;
    }

    return bodyPart.mBasePoints + bodyPart.mDistancePoints * calcScoreDistRate() +
           bodyPart.mAnglePoints * calcScoreAngleRate();
}

bool FukuwaraiFaceParts::isPlaced() const {
    return al::isNerve(this, &NrvFukuwaraiFaceParts.Place) ||
           al::isNerve(this, &NrvFukuwaraiFaceParts.Hide);
}

f32 FukuwaraiFaceParts::calcScoreAngleRate() const {
    f32 angle = al::getRotate(this).y;

    f32 difference;
    if (angle > mTargetAngle)
        difference = angle - mTargetAngle;
    else
        difference = mTargetAngle - angle;

    f32 score = difference > 180.0f ? 360.0f - difference : difference;

    if (al::isEqualString("FukuwaraiMarioNose", al::getModelName(this))) {
        f32 clampedScore = score > 90.0f ? 180.0f - score : score;

        return 1.0f - clampedScore / 90.0f;
    }

    return 1.0f - score / 180.0f;
}

f32 FukuwaraiFaceParts::calcScoreDistRate() const {
    sead::Vector3f out;
    al::verticalizeVec(&out, sead::Vector3f::ey, al::getTrans(this) - mTargetPos);

    f32 score = out.length() / 500.0f;
    score = sead::Mathf::clamp(score, 0.0f, 1.0f);

    return sead::Mathf::square(1.0f - score);
}

void FukuwaraiFaceParts::show() {
    appear();
    al::showModelIfHide(this);
    al::setNerve(this, &NrvFukuwaraiFaceParts.Answer);
}

void FukuwaraiFaceParts::reset() {
    appear();
    al::validateHitSensors(this);
    al::showModelIfHide(this);
    al::resetRotatePosition(this, mRotation, mTrans);
    al::setNerve(this, &Appear);
}

void FukuwaraiFaceParts::vanish() {
    al::setNerve(this, &Vanish);
}

void FukuwaraiFaceParts::exePlace() {
    if (al::isFirstStep(this)) {
        al::setVelocityZero(this);
        al::invalidateHitSensors(this);
        al::startAction(this, "Decide");
    }

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvFukuwaraiFaceParts.Hide);
}

void FukuwaraiFaceParts::exeReset() {
    if (al::isFirstStep(this)) {
        al::setVelocityZero(this);
        al::startAction(this, "SwoonStart");
    }
    if (al::isActionEnd(this)) {
        reset();
        al::setNerve(this, &Appear);
    }
}

void FukuwaraiFaceParts::exeAppear() {
    if (al::isFirstStep(this))
        al::startAction(this,
                        al::isNerve(this, &NrvFukuwaraiFaceParts.Answer) ? "Answer" : "Appear");

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvFukuwaraiFaceParts.Wait);
}

void FukuwaraiFaceParts::exeWait() {
    if (al::isFirstStep(this))
        al::setVelocityZero(this);
}

void FukuwaraiFaceParts::exeCaptureWait() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Wait");
        al::onCollide(this);
    }

    if (rs::isOnHackMoveStick(mIUsePlayerHack)) {
        al::setNerve(this, &NrvFukuwaraiFaceParts.CaptureMove);
        return;
    }

    al::scaleVelocity(this, 0.5f);
    al::setTransY(this, mTrans.y + 10.0f);
    if (rs::isHoldHackAction(mIUsePlayerHack)) {
        al::addRotateAndRepeatY(this, 2.0f);
        al::holdSe(this, "RotateRev_loop");
    }
    if (rs::isHoldHackJump(mIUsePlayerHack)) {
        al::addRotateAndRepeatY(this, -2.0f);
        al::holdSe(this, "Rotate_loop");
    }
}

void FukuwaraiFaceParts::exeCaptureMove() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Move");
        if (al::isNerve(this, &NrvFukuwaraiFaceParts.CaptureMoveFast))
            al::setSklAnimFrameRate(this, 1.3f, 0);
        else
            al::setSklAnimFrameRate(this, 1.0f, 0);
    }

    if (!rs::isOnHackMoveStick(mIUsePlayerHack)) {
        al::setNerve(this, &NrvFukuwaraiFaceParts.CaptureWait);
        return;
    }

    sead::Vector3f dir = {0, 0, 0};
    f32 accel = 7.0f;
    if (al::isNerve(this, &NrvFukuwaraiFaceParts.CaptureMoveFast))
        accel = 12.0f;
    rs::addHackActorAccelStick(this, mIUsePlayerHack, &dir, accel, sead::Vector3f::ey);

    al::scaleVelocity(this, 0.5f);
    al::setTransY(this, mTrans.y + 10.0f);

    if (rs::isHoldHackAction(mIUsePlayerHack)) {
        al::addRotateAndRepeatY(this, 2.0f);
        al::holdSe(this, "RotateRev_loop");
    }
    if (rs::isHoldHackJump(mIUsePlayerHack)) {
        al::addRotateAndRepeatY(this, -2.0f);
        al::holdSe(this, "Rotate_loop");
    }

    if (al::isGreaterStep(this, 30))
        al::setNerve(this, &NrvFukuwaraiFaceParts.CaptureMove);
    else if (rs::isTriggerHackSwing(mIUsePlayerHack))
        al::setNerve(this, &NrvFukuwaraiFaceParts.CaptureMoveFast);
}

void FukuwaraiFaceParts::exeHide() {
    if (al::isFirstStep(this)) {
        al::setVelocityZero(this);
        al::hideModelIfShow(this);
    }
}

void FukuwaraiFaceParts::exeCaptureStart() {
    if (rs::isHackStartDemoEnterMario(mIUsePlayerHack))
        al::setNerve(this, &CaptureHackStart);
}

void FukuwaraiFaceParts::exeCaptureHackStart() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "HackStart");
        mPlayerHackStartShaderCtrl->start();
    }

    mPlayerHackStartShaderCtrl->update();

    if (al::isActionEnd(this)) {
        mPlayerHackStartShaderCtrl->end();
        rs::endHackStartDemo(mIUsePlayerHack, this);
        al::setNerve(this, &NrvFukuwaraiFaceParts.CaptureWait);
    }
}

bool FukuwaraiFaceParts::isKuriboMarioParts() const {
    return al::isEqualString(al::getModelName(this), "FukuwaraiKuriboEyebrowLeft") ||
           al::isEqualString(al::getModelName(this), "FukuwaraiKuriboEyebrowRight") ||
           al::isEqualString(al::getModelName(this), "FukuwaraiKuriboMarioEyeLeft") ||
           al::isEqualString(al::getModelName(this), "FukuwaraiKuriboMarioEyeRight") ||
           al::isEqualString(al::getModelName(this), "FukuwaraiKuriboMouthAngry") ||
           al::isEqualString(al::getModelName(this), "FukuwaraiMarioMustache");
}
