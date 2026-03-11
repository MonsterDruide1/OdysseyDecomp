#include "Player/PlayerStateHack.h"

#include "Library/Camera/CameraUtil.h"
#include "Library/Effect/EffectSystemInfo.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathUtil.h"
#include "Library/Matrix/MatrixUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Player/HackCap.h"
#include "Player/IPlayerModelChanger.h"
#include "Player/PlayerAnimator.h"
#include "Player/PlayerCapFunction.h"
#include "Player/PlayerHackKeeper.h"

namespace {
NERVE_IMPL(PlayerStateHack, End);
NERVES_MAKE_NOSTRUCT(PlayerStateHack, End);

NERVE_IMPL(PlayerStateHack, Hack);
NERVE_IMPL(PlayerStateHack, HackDemo);
NERVE_IMPL(PlayerStateHack, HackDemoPuppetable);
NERVES_MAKE_STRUCT(PlayerStateHack, Hack, HackDemoPuppetable, HackDemo);
}  // namespace

PlayerStateHack::PlayerStateHack(al::LiveActor* player, PlayerHackKeeper* hackKeeper,
                                 IPlayerModelChanger* modelChanger, PlayerAnimator* animator,
                                 HackCap* hackCap)
    : ActorStateBase("ひょうい", player), mHackKeeper(hackKeeper), mModelChanger(modelChanger),
      mAnimator(animator), mHackCap(hackCap) {
    initNerve(&End, 0);
    al::setEffectFollowMtxPtr(player, "PossessTrace", &mPossessTraceMtx);
}

void PlayerStateHack::appear() {
    al::ActorStateBase::appear();

    if (mIsStageStartHack) {
        mIsStageStartHack = false;
        al::setNerve(this, &NrvPlayerStateHack.Hack);
    } else if (mHackKeeper->isPuppetable2()) {
        al::setNerve(this, &NrvPlayerStateHack.HackDemoPuppetable);
    } else {
        al::setNerve(this, &NrvPlayerStateHack.HackDemo);
    }
}

void PlayerStateHack::prepareStartHack(const al::HitSensor* source, const al::HitSensor* target) {
    mHackCap->startHack();

    sead::Vector3f upDir;
    al::calcUpDir(&upDir, mActor);

    f32 sensorDistance =
        sead::Mathf::clampMin(al::calcDistance(source, target) - 200.0f, 0.0f) / 30.0f;
    mHackDemoStartLength = sead::Mathi::clamp(sead::Mathf::round(sensorDistance) + 15, 15, 30);

    mStartTrans = al::getTrans(mActor);
    mStartSensorTrans = al::getSensorPos(target) + upDir * 0.0f;
    mCurSensorTrans = mStartSensorTrans;
    al::startHitReaction(mActor, "ひょうい開始");
}

void PlayerStateHack::prepareEndHack() {
    al::startHitReaction(mActor, "ひょうい終了");
    CapFunction::endHack(mHackCap, mAnimator);
    al::onCollide(mActor);
    mAnimator->startAnim("JumpEndHack");
    mModelChanger->showModel();
    mHackKeeper->killHackDemoModel();
    al::tryDeleteEffect(mActor, "PossessTrace");
    al::setNerve(this, &End);
}

void PlayerStateHack::prepareStageStartHack() {
    auto* player = mActor;
    mIsStageStartHack = true;
    al::offCollide(player);
    al::setVelocityZero(player);
    al::tryKillEmitterAndParticleAll(player);
    mModelChanger->hideModel();
    mHackKeeper->recordHack();
    mHackCap->recordHack();
}

bool PlayerStateHack::isIgnoreUpdateCollider() const {
    return isDead() || !al::isNerve(this, &End);
}

bool PlayerStateHack::isEnableChangeState() const {
    if (mHackKeeper->isHack())
        return false;
    if (mHackKeeper->getHackSensor() && mHackKeeper->isStartedHacking())
        return false;
    if (al::isNerve(this, &NrvPlayerStateHack.HackDemo))
        return false;
    if (al::isNerve(this, &NrvPlayerStateHack.HackDemoPuppetable))
        return false;
    return true;
}

bool PlayerStateHack::isEnableModelSyncShowHide() const {
    return !al::isNerve(this, &NrvPlayerStateHack.Hack) || mHackKeeper->isHack();
}

bool PlayerStateHack::isEnableCancelHack() const {
    if (!al::isNerve(this, &NrvPlayerStateHack.Hack))
        return false;
    if (mHackKeeper->getHackSensor() && mHackKeeper->isStartedHacking())
        return false;
    return al::isGreaterEqualStep(this, 13);
}

// TODO: adjust `Vector3f::rotate` in `sead`
inline void rotate(sead::Vector3f* o, const sead::Quatf& q, const sead::Vector3f& v) {
    sead::Quatf r;  // quat-multiplication with 0 on w for v
    r.x = (q.y * v.z) - (q.z * v.y) + (q.w * v.x);
    r.y = -(q.x * v.z) + (q.z * v.x) + (q.w * v.y);
    r.z = (q.x * v.y) - (q.y * v.x) + (q.w * v.z);
    r.w = -(q.x * v.x) - (q.y * v.y) - (q.z * v.z);

    r.w *= -1;

    const f32 qw = q.w, qx = q.x, qy = q.y, qz = q.z;
    const f32 rx = r.x, ry = r.y, rz = r.z, rw = r.w;

    // quat-multiplication
    o->x = (qw * rx) - (qz * ry) + (qy * rz) + (qx * rw);
    o->y = (qz * rx) + (qw * ry) - (qx * rz) + (qy * rw);
    o->z = (qw * rz) + (-(qy * rx) + (qx * ry)) + (qz * rw);
}

void PlayerStateHack::exeHackDemo() {
    sead::Vector3f frontDir;
    sead::Vector3f upDir;
    sead::Vector3f backDir;
    sead::Vector3f downDir;
    sead::Vector3f cameraFront;
    sead::Vector3f reverseCameraFront;

    auto* player = mActor;
    auto* hackActor = mHackKeeper->getHack();

    if (al::isFirstStep(this)) {
        mAnimator->startAnim("Wait");
        al::offCollide(player);
        al::setVelocityZero(player);
        sead::Matrix34f modelMtx = sead::Matrix34f::ident;
        sead::Vector3f playerFrontDir = {0.0f, 0.0f, 0.0f};
        sead::Vector3f playerUpDir = {0.0f, 0.0f, 0.0f};
        al::calcFrontDir(&playerFrontDir, player);
        al::calcUpDir(&playerUpDir, player);
        al::makeMtxSideFrontPos(&modelMtx, -playerUpDir, playerFrontDir, mStartSensorTrans);
        mHackKeeper->appearHackDemoModel(modelMtx, 0.02f);
    } else if (al::isStep(this, 1)) {
        mModelChanger->hideModel();
    }

    s32 startEaseIn = sead::Mathi::clampMin(mHackDemoStartLength - 4, 0);
    f32 modelScale;
    if (al::isFirstStep(this))
        modelScale = 1.0f;
    else if (al::isLessStep(this, startEaseIn))
        modelScale = al::calcNerveEaseOutValue(this, 0, 5, 0.02f, 1.0f);
    else
        modelScale = al::calcNerveEaseInValue(this, startEaseIn, startEaseIn + 5, 1.0f, 0.02f);

    if (al::isStep(this, mHackDemoStartLength + 9))
        mHackCap->noticeHackMarioEnter();
    if (al::isStep(this, sead::Mathi::clampMin(mHackDemoStartLength - 15, 0)))
        mHackCap->emitHackStartEffect();

    if (!al::isLessStep(this, mHackDemoStartLength)) {
        mHackKeeper->updateHackDemoModel(mDemoModelMtx, modelScale);
        mHackKeeper->deleteHackDemoModelEffect();
        al::tryDeleteEffect(mActor, "PossessTrace");
        al::copyPose(mActor, mHackKeeper->getHack());
        if (!mHackKeeper->getHackSensor() || !mHackKeeper->isHackDemoStarted()) {
            mHackKeeper->killHackDemoModel();
            mHackKeeper->recordHack();
            mHackCap->recordHack();
            al::startHitReaction(mHackCap, "ひょうい完了");
            al::setNerve(this, &NrvPlayerStateHack.Hack);
        }
        return;
    }

    f32 demoRate = al::calcNerveRate(this, mHackDemoStartLength);
    const sead::Vector3f& capTrans = al::getTrans(mHackCap);
    sead::Vector3f lerpedTrans = {0.0f, 0.0f, 0.0f};
    al::lerpVec(&lerpedTrans, mStartTrans, al::getTrans(hackActor), demoRate);
    al::resetPosition(mActor, lerpedTrans);

    sead::Vector3f beforeLerpSensorTrans = mCurSensorTrans;
    al::lerpVec(&mCurSensorTrans, mStartSensorTrans, capTrans, demoRate);
    mCurSensorTrans.setScaleAdd(sead::Mathf::sin(sead::Mathf::deg2rad(demoRate * 180.0f)) * -400.0f,
                                al::getGravity(mActor), mCurSensorTrans);

    frontDir.setSub(mCurSensorTrans, beforeLerpSensorTrans);
    sead::Matrix34f* mtx = &mPossessTraceMtx;
    sead::Matrix34f* mtx2 = &mDemoModelMtx;
    if (!al::tryNormalizeOrZero(&frontDir)) {
        frontDir.setSub(capTrans, al::getTrans(player));
        if (!al::tryNormalizeOrZero(&frontDir))
            al::calcFrontDir(&frontDir, player);
    }

    upDir = -al::getGravity(player);
    if (al::isParallelDirection(upDir, frontDir)) {
        al::calcFrontDir(&upDir, player);
        if (al::isParallelDirection(upDir, frontDir))
            al::calcUpDir(&upDir, player);
    }

    al::makeMtxFrontUpPos(mtx, frontDir, upDir, mCurSensorTrans);

    backDir = -frontDir;
    downDir = -upDir;
    cameraFront = {0.0f, 0.0f, 0.0f};
    al::calcCameraFront(&cameraFront, player, 0);
    if (al::isParallelDirection(downDir, cameraFront)) {
        if (al::isReverseDirection(downDir, cameraFront))
            downDir *= -1;
    } else if (!al::isParallelDirection(cameraFront, backDir)) {
        sead::Quatf quat = sead::Quatf::unit;
        reverseCameraFront = -cameraFront;

        al::makeQuatAxisRotation(&quat, downDir, reverseCameraFront, backDir, 1.0f);
        rotate(&downDir, quat, downDir);
    }

    al::makeMtxSideFrontPos(mtx2, backDir, downDir, mCurSensorTrans);

    if (al::isStep(this, mHackDemoStartLength - 1)) {
        mHackKeeper->setPuppetable(true);
        al::startHitReactionHitEffect(mActor, "ひょうい先に入る", al::getTrans(mHackCap));
    }
    if (!al::isFirstStep(this))
        mHackKeeper->updateHackDemoModel(mDemoModelMtx, modelScale);
}

void PlayerStateHack::exeHackDemoPuppetable() {
    auto* player = mActor;
    if (al::isFirstStep(this)) {
        mAnimator->startAnim("Wait");
        mModelChanger->hideModel();
        al::offCollide(player);
        al::setVelocityZero(player);
        al::tryKillEmitterAndParticleAll(player);
    }

    if (!mHackKeeper->getHackSensor() || !mHackKeeper->isHackDemoStarted()) {
        mHackCap->noticeHackDemoPuppetableEnd();
        mHackKeeper->recordHack();
        mHackCap->recordHack();
        al::setNerve(this, &NrvPlayerStateHack.Hack);
    }
}

void PlayerStateHack::exeHack() {
    if (mHackKeeper->isHack())
        return;
    al::copyPose(mActor, mHackKeeper->getHack());
    al::setVelocity(mActor, al::getVelocity(mHackKeeper->getHack()));
}

void PlayerStateHack::exeEnd() {}
