#include "Npc/KuriboGirl.h"

#include <math/seadBoundBox.h>

#include "Library/Area/AreaObj.h"
#include "Library/Area/AreaObjGroup.h"
#include "Library/Area/AreaObjUtil.h"
#include "Library/Area/AreaShape.h"
#include "Library/Camera/ActorCameraTarget.h"
#include "Library/Camera/CameraUtil.h"
#include "Library/Joint/JointControllerKeeper.h"
#include "Library/Joint/JointSpringControllerHolder.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/LiveActor/LiveActorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Player/PlayerUtil.h"

#include "Item/Shine.h"
#include "Util/DemoUtil.h"
#include "Util/ItemUtil.h"
#include "Util/PlayerUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(KuriboGirl, WaitLoveAfter);
NERVE_IMPL(KuriboGirl, Wait);
NERVE_IMPL(KuriboGirl, Surprise);
NERVE_IMPL(KuriboGirl, LoveLook);
NERVE_IMPL(KuriboGirl, LookLoveAfter);
NERVE_IMPL(KuriboGirl, Disappear);
NERVE_IMPL(KuriboGirl, Hide);
NERVE_IMPL(KuriboGirl, Appear);
NERVE_IMPL(KuriboGirl, LoveFind);
NERVE_IMPL(KuriboGirl, AppearShineStart);
NERVE_IMPL(KuriboGirl, AppearShine);
NERVE_IMPL(KuriboGirl, AppearShineEnd);
NERVE_IMPL(KuriboGirl, LoveLookTurn);
NERVE_IMPL(KuriboGirl, PreLoveFind);
NERVE_IMPL(KuriboGirl, Find);

NERVES_MAKE_STRUCT(KuriboGirl, WaitLoveAfter, Wait, Surprise, LoveLook, LookLoveAfter, Appear,
                   LoveLookTurn, PreLoveFind, Find);

NERVES_MAKE_NOSTRUCT(KuriboGirl, Disappear, Hide, LoveFind, AppearShineStart, AppearShine,
                     AppearShineEnd);
}  // namespace

KuriboGirl::KuriboGirl(const char* name) : al::LiveActor(name) {}

void KuriboGirl::init(const al::ActorInitInfo& actorInitInfo) {
    al::initActorWithArchiveName(this, actorInitInfo, "KuriboGirl", nullptr);
    mAreaObjGroup = al::createLinkAreaGroup(this, actorInitInfo, "FindArea", "子供エリアグループ",
                                            "子供エリア");
    mCamera = al::initObjectCamera(this, actorInitInfo, nullptr, nullptr);
    mCameraTarget = al::createActorCameraTarget(this, 0.0f);

    al::tryGetArg(&mIsMoveShine, actorInitInfo, "isMoveShine");
    mShineActor = rs::tryInitLinkShine(actorInitInfo, "ShineActor", 0);
    if (mShineActor == nullptr) {
        mIsAfterShineAppear = true;
        al::initNerve(this, &NrvKuriboGirl.WaitLoveAfter, 0);
    } else {
        mShineMoveDir = new sead::Vector3f(al::getFront(this));
        al::initNerve(this, &NrvKuriboGirl.Wait, 0);
    }

    al::setHitSensorPosPtr(this, "WatchStart", &mHitSensorPos);
    if (mAreaObjGroup == nullptr) {
        mHitSensorPos.set(al::getTrans(this));
        al::setSensorRadius(this, "WatchStart", 900.0f);
    } else {
        sead::BoundBox3f boundBox;
        mAreaObjGroup->getAreaObj(0)->getAreaShape()->calcLocalBoundingBox(&boundBox);
        sead::Vector3f areaShapeScale = mAreaObjGroup->getAreaObj(0)->getAreaShape()->getScale();
        boundBox.scaleX(areaShapeScale.x);
        boundBox.scaleY(areaShapeScale.y);
        boundBox.scaleZ(areaShapeScale.z);

        sead::Vector3f hitSensorPos = sead::Vector3f(0.0f, 0.0f, 0.0f);
        mAreaObjGroup->getAreaObj(0)->getAreaShape()->calcTrans(&hitSensorPos);
        hitSensorPos += sead::Vector3f(0.0f, boundBox.getHalfSizeY(), 0.0f);
        mHitSensorPos.set(hitSensorPos);
        al::setSensorRadius(
            this, "WatchStart",
            sead::Vector3f(boundBox.getSizeX(), boundBox.getSizeY(), boundBox.getSizeZ()).length() *
                0.5f);
    }
    al::initJointControllerKeeper(this, 2);
    mJointSpringControllerHolder = new al::JointSpringControllerHolder();
    mJointSpringControllerHolder->init(this, "InitJointSpringCtrl");
    al::initJointLocalYRotator(this, &mHeadRotator, "Head");
    mInitialFront = al::getFront(this);
    makeActorAlive();
}

void KuriboGirl::control() {
    if (!rs::isPlayerOnGround(this))
        mFramesOnGround = 0;
    else
        mFramesOnGround++;
    mIsPlayerNear = false;
    mIsLoveLook = false;
    mIsPlayerFar = true;
}

void KuriboGirl::exeWait() {
    if (al::isFirstStep(this) && !al::isActionPlaying(this, "Wait")) {
        al::validateClipping(this);
        al::startAction(this, "Wait");
    }
    lookFront();

    al::LiveActor* nearestPlayerActor = al::tryFindNearestPlayerActor(this);
    if (nearestPlayerActor != nullptr) {
        if (rs::isPlayerHack(this) && !rs::isPlayerHackKuriboAny(this)) {
            if (mAreaObjGroup == nullptr) {
                sead::Vector3f distance = al::getTrans(nearestPlayerActor) - al::getTrans(this);
                if (distance.length() < 900.0f) {
                    al::setNerve(this, &NrvKuriboGirl.Surprise);
                    return;
                }
            } else if (al::isInAreaObj(mAreaObjGroup, al::getTrans(nearestPlayerActor))) {
                al::setNerve(this, &NrvKuriboGirl.Surprise);
                return;
            }
        }
        if (rs::isPlayerHackTank(this)) {
            al::setNerve(this, &NrvKuriboGirl.Surprise);
            return;
        }
    }
}

void KuriboGirl::lookFront() {
    mHeadRotator += (0.0f - mHeadRotator) * 0.1f;
}

void KuriboGirl::exeFind() {
    if (al::isFirstStep(this))
        al::startAction(this, "Find");
    lookPlayer();
    if (al::isActionEnd(this))
        al::setNerve(this, &NrvKuriboGirl.LoveLook);
}

void KuriboGirl::lookPlayer() {
    f32 angle = 0.0f;
    sead::Vector3f nearestPlayerPos;
    if (al::tryFindNearestPlayerPos(&nearestPlayerPos, this)) {
        sead::Vector3f plane = nearestPlayerPos - al::getTrans(this);
        plane.y = 0;
        al::tryNormalizeOrDirZ(&plane);
        angle = al::calcAngleOnPlaneDegree(al::getFront(this), plane, sead::Vector3f::ey);

        if (angle > 45.0) {
            angle = 45.0;
            if (!al::isNerve(this, &NrvKuriboGirl.LoveLookTurn))
                al::setNerve(this, &NrvKuriboGirl.LoveLookTurn);
        } else if (angle < -45.0) {
            angle = -45.0;
            if (!al::isNerve(this, &NrvKuriboGirl.LoveLookTurn))
                al::setNerve(this, &NrvKuriboGirl.LoveLookTurn);
        }
    }
    mHeadRotator += (angle - mHeadRotator) * 0.1f;
}

void KuriboGirl::exeLoveLook() {
    if (al::isFirstStep(this) && !al::isActionPlaying(this, "LoveLook"))
        al::startAction(this, "LoveLook");
    lookPlayer();
    if (!mIsLoveLook)
        al::setNerve(this, &NrvKuriboGirl.Wait);
}

void KuriboGirl::exeLoveLookTurn() {
    if (al::isFirstStep(this) && !al::isActionPlaying(this, "LoveLookTurn"))
        al::startAction(this, "LoveLookTurn");

    sead::Vector3f nearestPlayer;
    if (al::tryFindNearestPlayerPos(&nearestPlayer, this)) {
        sead::Vector3f plane = nearestPlayer - al::getTrans(this);
        plane.y = 0;
        al::tryNormalizeOrDirZ(&plane);
        f32 angle = al::calcAngleOnPlaneDegree(al::getFront(this), plane, sead::Vector3f::ey);
        if (angle > 1.0f || angle < -1.0f) {
            sead::Vector3f new_front = al::getFront(this);
            al::turnVecToVecDegree(&new_front, new_front, plane, 2.5f);
            al::setFront(this, new_front);
        } else {
            if (mIsAfterShineAppear)
                al::setNerve(this, &NrvKuriboGirl.LookLoveAfter);
            else
                al::setNerve(this, &NrvKuriboGirl.LoveLook);
            return;
        }
    }
    lookPlayer();
    if (!mIsLoveLook) {
        if (mIsAfterShineAppear)
            al::setNerve(this, &NrvKuriboGirl.WaitLoveAfter);
        else
            al::setNerve(this, &NrvKuriboGirl.Wait);
    }
}

void KuriboGirl::exeSurprise() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Surprise");
        al::invalidateClipping(this);
    }
    lookFront();

    sead::Vector3f nearestPlayer;
    if (al::tryFindNearestPlayerPos(&nearestPlayer, this)) {
        sead::Vector3f new_front = nearestPlayer - al::getTrans(this);
        new_front.y = 0.0f;
        al::tryNormalizeOrDirZ(&new_front);
        new_front += al::getFront(this) * 3.0f;
        al::tryNormalizeOrDirZ(&new_front);
        al::setFront(this, new_front);
    }
    al::setNerveAtActionEnd(this, &Disappear);
}

void KuriboGirl::exeDisappear() {
    if (al::isFirstStep(this))
        al::startAction(this, "Disappear");
    if (al::isActionEnd(this))
        al::setNerve(this, &Hide);
}

void KuriboGirl::exeHide() {
    if (al::isFirstStep(this)) {
        al::hideModel(this);
        al::invalidateHitSensor(this, "Body");
        al::startHitReaction(this, "消滅");
    }
    if (mIsPlayerFar && al::isNearPlayer(this, al::getSensorRadius(this, "WatchStart")))
        mIsPlayerFar = false;
    if (al::isGreaterEqualStep(this, 120) && mIsPlayerFar && !rs::isPlayerHackTank(this)) {
        al::validateHitSensor(this, "Body");
        al::setFront(this, mInitialFront);
        al::setNerve(this, &NrvKuriboGirl.Appear);
    }
}

void KuriboGirl::exeAppear() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Appear");
        al::startHitReaction(this, "出現");
        al::showModel(this);
    }

    lookFront();

    if (al::isActionEnd(this)) {
        if (mIsAfterShineAppear)
            al::setNerve(this, &NrvKuriboGirl.WaitLoveAfter);
        else
            al::setNerve(this, &NrvKuriboGirl.Wait);
    }
}

void KuriboGirl::exePreLoveFind() {
    if (rs::requestStartDemoNormal(this, false)) {
        al::startCamera(this, mCamera, -1);
        al::setCameraTarget(this, mCameraTarget);
        al::setNerve(this, &LoveFind);
    }
}

void KuriboGirl::exeLoveFind() {
    if (al::isFirstStep(this))
        al::startAction(this, "Find");
    if (al::isActionEnd(this))
        al::setNerve(this, &AppearShineStart);
}

void KuriboGirl::exeAppearShineStart() {
    if (al::isFirstStep(this))
        al::startAction(this, "AppearShineStart");
    if (al::isActionEnd(this))
        al::setNerve(this, &AppearShine);
}

void KuriboGirl::exeAppearShine() {
    if (al::isFirstStep(this)) {
        if (mIsMoveShine) {
            sead::Vector3f shineActorPosDelta = al::getTrans(mShineActor) - al::getTrans(this);

            f32 angle;
            sead::Vector3f nearestPlayer;
            if (al::tryFindNearestPlayerPos(&nearestPlayer, this)) {
                angle = al::calcAngleOnPlaneDegree(
                    *mShineMoveDir, nearestPlayer - al::getTrans(this), sead::Vector3f::ey);
            } else {
                angle = al::calcAngleOnPlaneDegree(*mShineMoveDir, al::getFront(this),
                                                   sead::Vector3f::ey);
            }

            al::rotateVectorDegreeY(&shineActorPosDelta, angle);

            al::setTrans(mShineActor, al::getTrans(this) + shineActorPosDelta);

            sead::Vector3f shineActorPos = mShineActor->get_16c();
            sead::Vector3f local_80 = shineActorPos - al::getTrans(this);

            al::rotateVectorDegreeY(&local_80, angle);
            local_80 += al::getTrans(this);

            mShineActor->set_16c(local_80);
        }
        rs::appearPopupShineWithoutDemo(mShineActor);
        al::startAction(this, "AppearShine");
        mIsAfterShineAppear = true;
    }
    if (rs::isEndAppearShine(mShineActor))
        al::setNerve(this, &AppearShineEnd);
}

void KuriboGirl::exeAppearShineEnd() {
    if (al::isFirstStep(this))
        al::startAction(this, "AppearShineEnd");
    if (al::isActionEnd(this)) {
        rs::requestEndDemoNormal(this);
        al::endCamera(this, mCamera, -1, false);
        al::resetCameraTarget(this, mCameraTarget);
        al::setNerve(this, &NrvKuriboGirl.LookLoveAfter);
    }
}

void KuriboGirl::exeLookLoveAfter() {
    if (al::isFirstStep(this) && !al::isActionPlaying(this, "LoveAfter"))
        al::startAction(this, "LoveAfter");
    lookPlayer();

    if (!mIsPlayerNear)
        al::setNerve(this, &NrvKuriboGirl.WaitLoveAfter);
    else if (!rs::isPlayerHackKuriboAny(this))
        al::setNerve(this, &NrvKuriboGirl.Surprise);
}

void KuriboGirl::exeWaitLoveAfter() {
    if (al::isFirstStep(this) && !al::isActionPlaying(this, "LoveAfter"))
        al::startAction(this, "LoveAfter");
    lookFront();
}

void KuriboGirl::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isSensorName(self, "WatchStart")) {
        if (rs::sendMsgKuriboGirlLove(other, self) && isWatchStart(al::getSensorPos(other))) {
            mIsPlayerNear = true;
            mIsPlayerFar = false;
            if (al::isNerve(this, &NrvKuriboGirl.LoveLook) && mFramesOnGround >= 16)
                al::setNerve(this, &NrvKuriboGirl.PreLoveFind);
            else if (al::isNerve(this, &NrvKuriboGirl.WaitLoveAfter) && mFramesOnGround >= 16)
                al::setNerve(this, &NrvKuriboGirl.LookLoveAfter);
            return;
        }

        if (al::isSensorPlayerAll(other) && !rs::isPlayerHackKuriboAny(this) &&
            isWatchStart(al::getSensorPos(other))) {
            mIsPlayerNear = true;
            mIsPlayerFar = false;
            if (al::isNerve(this, &NrvKuriboGirl.Wait) ||
                al::isNerve(this, &NrvKuriboGirl.WaitLoveAfter))
                al::setNerve(this, &NrvKuriboGirl.Surprise);
            return;
        }
    }

    if (al::isSensorName(self, "WatchFar") && rs::sendMsgKuriboGirlLove(other, self)) {
        mIsLoveLook = true;
        if (al::isNerve(this, &NrvKuriboGirl.Wait))
            al::setNerve(this, &NrvKuriboGirl.Find);
        return;
    }
    if (al::isSensorEnemyBody(self)) {
        al::sendMsgPush(other, self);
        rs::sendMsgPushToPlayer(other, self);
        return;
    }
    if (al::isSensorEnemyAttack(self) &&
        (isNrvWait() || al::isNerve(this, &NrvKuriboGirl.Appear))) {
        if (rs::sendMsgKuriboGirlAttack(other, self) && !al::isNerve(this, &NrvKuriboGirl.Appear))
            al::setNerve(this, &NrvKuriboGirl.Surprise);
    }
}

bool KuriboGirl::isWatchStart(sead::Vector3f pos) {
    if (mAreaObjGroup != nullptr) {
        if (al::isInAreaObj(mAreaObjGroup, pos))
            return true;
    } else {
        f32 length = (pos - al::getTrans(this)).length();
        if (length < 900.0f)
            return true;
    }
    return false;
}

bool KuriboGirl::isNrvWait() {
    return al::isNerve(this, &NrvKuriboGirl.Wait) || al::isNerve(this, &NrvKuriboGirl.LoveLook) ||
           al::isNerve(this, &NrvKuriboGirl.LoveLookTurn) ||
           al::isNerve(this, &NrvKuriboGirl.LookLoveAfter) ||
           al::isNerve(this, &NrvKuriboGirl.WaitLoveAfter);
}

bool KuriboGirl::receiveMsg(const al::SensorMsg* msg, al::HitSensor* other, al::HitSensor* self) {
    if (rs::isMsgPlayerDisregardHomingAttack(msg) || rs::isMsgPlayerDisregardTargetMarker(msg) ||
        al::isMsgPlayerDisregard(msg))
        return true;

    if ((al::isSensorPlayerAttack(other) || rs::isMsgHosuiAttack(msg)) &&
        al::isSensorName(self, "EyeWarning")) {
        mIsPlayerFar = false;
        if (isNrvWait())
            al::setNerve(this, &NrvKuriboGirl.Surprise);
        return false;
    }
    if (al::isMsgPush(msg) && al::isSensorName(self, "Body"))
        return true;
    if (rs::isMsgCapAttack(msg) && al::isSensorName(self, "EyeWarning")) {
        mIsPlayerFar = false;
        if (isNrvWait())
            al::setNerve(this, &NrvKuriboGirl.Surprise);
        return false;
    }
    return false;
}
