#include "Boss/Koopa/KoopaCameraCtrl.h"

#include "Camera/CameraPoserKoopaFireBeamStart.h"
#include "Library/Camera/CameraTicket.h"
#include "Library/Camera/CameraUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Project/Camera/CameraSubTargetTurnParam.h"

namespace {
// Turn param used by both ctors; matches the 0x30-byte constant in the original binary
// (turnSpeedRate1 0.3, turnSpeedRate2 0.1, turnBrakeEndDistance 250, turnBrakeStartDistance 1000,
// turnStopStartDistance -1, turnStopEndDistance -1, targetTurnRestartStep 90, isTurnV false,
// isResetAfterTurnV false, minTurnDegreeV -1, maxTurnDegreeV -1, validTurnDegreeRangeH 0,
// validFaceDegreeRangeH 0).
constexpr al::CameraSubTargetTurnParam sKoopaCameraTurnParam(
    0.3f, 0.1f, 250.0f, 1000.0f, -1.0f, -1.0f, 90, false, false, -1.0f, -1.0f, 0.0f, 0.0f);
}  // namespace

KoopaCameraCtrl::KoopaCameraCtrl(const al::LiveActor* actor, const al::ActorInitInfo& info)
    : mActor(actor) {
    mCameraDirector = actor->getCameraDirector();
    mCameraTicket = al::initDemoAnimCamera(actor, info, "Anim");
    al::getTrans(&mTrans, info);
    mTurnParam = const_cast<al::CameraSubTargetTurnParam*>(&sKoopaCameraTurnParam);
}

KoopaCameraCtrl::KoopaCameraCtrl(const al::LiveActor* actor, const al::LiveActor* actor2,
                                 const al::ActorInitInfo& info, const sead::Vector3f& trans)
    : mTrans(trans), mActor(actor), mActor2(actor2) {
    mCameraDirector = actor->getCameraDirector();
    mCameraTicket = al::initDemoAnimCamera(actor, info, "Anim");
    mTurnParam = const_cast<al::CameraSubTargetTurnParam*>(&sKoopaCameraTurnParam);
}

const char* KoopaCameraCtrl::getTargetName() const {
    if (mTargetType == 1)
        return "クッパ";
    if (mTargetType == 2)
        return "クッパ＋クッパ帽子";
    return "";
}

void KoopaCameraCtrl::calcTrans(sead::Vector3f* trans) const {
    trans->set(mTargetTrans);
}

void KoopaCameraCtrl::update() {
    updateTargetTrans();
}

al::CameraDirector* KoopaCameraCtrl::getCameraDirector() const {
    return mCameraDirector;
}

void KoopaCameraCtrl::startDemoCamera(const char* animName) {
    if (al::isActiveCamera(mCameraTicket))
        al::startAnimCameraAnim(mCameraTicket, animName, -1, -1, -1);
    else
        al::startAnimCamera(this, mCameraTicket, animName, 0);
}

void KoopaCameraCtrl::endDemoCamera() {
    if (mCameraPoseInfo) {
        al::endCameraWithNextCameraPose(this, mCameraTicket, mCameraPoseInfo, 0);
        mCameraPoseInfo = nullptr;
    } else {
        al::endCamera(this, mCameraTicket, 0, false);
    }
    al::requestCancelCameraInterpole(this, 0);
}

bool KoopaCameraCtrl::tryEndDemoCamera() {
    if (!al::isActiveCamera(mCameraTicket))
        return false;
    if (mCameraPoseInfo) {
        al::endCameraWithNextCameraPose(this, mCameraTicket, mCameraPoseInfo, 0);
        mCameraPoseInfo = nullptr;
    } else {
        al::endCamera(this, mCameraTicket, 0, false);
    }
    al::requestCancelCameraInterpole(this, 0);
    return true;
}

void KoopaCameraCtrl::initFireBeamStartCamera(const al::ActorInitInfo& info) {
    auto* poser = new CameraPoserKoopaFireBeamStart("クッパファイアビーム開始");
    mFireBeamStartTicket =
        alCameraFunction::initCameraNoSave(poser, this, info, "FireBeamStart", 5);
}

void KoopaCameraCtrl::tryStartFireBeamStartCamera(const sead::Vector3f& vec1,
                                                  const sead::Vector3f& vec2) {
    if (!mFireBeamStartTicket)
        return;
    static_cast<CameraPoserKoopaFireBeamStart*>(mFireBeamStartTicket->getPoser())
        ->prepareStart(vec1, vec2);
    al::startCamera(this, mFireBeamStartTicket, -1);
}

void KoopaCameraCtrl::resetAllCameraSubTarget() {
    if (mIsUseSubTarget) {
        mIsUseSubTarget = false;
        al::resetCameraSubTarget(this, this);
    }
    mTargetType = 0;
}

void KoopaCameraCtrl::switchKoopaSubTarget() {
    if (mTargetType == 1)
        return;
    if (mTargetType == 0)
        mInterpoleStep = 0;
    else {
        mPreTargetTrans.set(mTargetTrans);
        mInterpoleStep = 30;
    }
    mInterpoleStepCounter = mInterpoleStep;
    mTargetType = 1;
    if (!mIsUseSubTarget) {
        mIsUseSubTarget = true;
        al::setCameraSubTarget(this, this);
    }
}

void KoopaCameraCtrl::switchKoopaAndCapSubTarget() {
    if (mTargetType == 2)
        return;
    if (mTargetType == 0)
        mInterpoleStep = 0;
    else {
        mPreTargetTrans.set(mTargetTrans);
        mInterpoleStep = 90;
    }
    mInterpoleStepCounter = mInterpoleStep;
    mTargetType = 2;
    if (!mIsUseSubTarget) {
        mIsUseSubTarget = true;
        al::setCameraSubTarget(this, this);
    }
}

void KoopaCameraCtrl::startTargetInterpole(s32 targetType, s32 step) {
    if (mTargetType == targetType)
        return;
    s32 interpoleStep = step < 0 ? 30 : step;
    if (mTargetType == 0) {
        interpoleStep = 0;
        mInterpoleStep = 0;
    } else {
        mPreTargetTrans.set(mTargetTrans);
        mInterpoleStep = interpoleStep;
    }
    mInterpoleStepCounter = interpoleStep;
    mTargetType = targetType;
    if (!mIsUseSubTarget) {
        mIsUseSubTarget = true;
        al::setCameraSubTarget(this, this);
    }
}

void KoopaCameraCtrl::updateTargetTrans() {
    if (!mIsUseSubTarget)
        return;
    sead::Vector3f trans = {0.0f, 0.0f, 0.0f};
    if (mTargetType == 2)
        trans = al::getTrans(mActor) * 0.35f + al::getTrans(mActor2) * 0.65f;
    else if (mTargetType == 1)
        trans.set(al::getTrans(mActor));
    else
        return;
    if (mInterpoleStepCounter >= 1) {
        f32 rate = al::normalize(mInterpoleStep - mInterpoleStepCounter, 0, mInterpoleStep);
        al::lerpVec(&trans, mPreTargetTrans, trans, rate);
        mInterpoleStepCounter--;
    }
    mTargetTrans.set(trans);
}

bool KoopaCameraCtrl::isExistDemoCameraAnim(const char* animName) const {
    return al::isExistAnimCameraData(mCameraTicket, animName);
}

bool KoopaCameraCtrl::isPlayingDemoCameraAnim(const char* animName) const {
    return al::isAnimCameraAnimPlaying(mCameraTicket, animName);
}

void KoopaCameraCtrl::invalidateDemoCameraAngleSwing() {
    al::invalidateAnimCameraAngleSwing(mCameraTicket);
}

void KoopaCameraCtrl::validateDemoCameraAngleSwing() {
    al::validateAnimCameraAngleSwing(mCameraTicket);
}
