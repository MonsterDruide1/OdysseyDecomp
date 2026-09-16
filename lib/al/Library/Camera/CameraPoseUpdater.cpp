#include "Library/Camera/CameraPoseUpdater.h"

#include <gfx/seadCamera.h>
#include <math/seadVectorFwd.h>
#include <nn/oe.h>

#include "Library/Area/AreaObjUtil.h"
#include "Library/Camera/CameraParamTransfer.h"
#include "Library/Camera/CameraPoseInfo.h"
#include "Library/Camera/CameraPoser.h"
#include "Library/Camera/CameraPoserFlag.h"
#include "Library/Camera/CameraPoserFunction.h"
#include "Library/Camera/CameraShaker.h"
#include "Library/Camera/CameraStartInfo.h"
#include "Library/Camera/CameraStartParamCtrl.h"
#include "Library/Camera/CameraStopJudge.h"
#include "Library/Camera/CameraSwitcher.h"
#include "Library/Camera/CameraTicket.h"
#include "Library/Camera/CameraUtil.h"
#include "Library/Camera/CameraViewCtrlPause.h"
#include "Library/Camera/CameraViewCtrlScene.h"
#include "Library/Camera/CameraViewFlag.h"
#include "Library/Camera/CameraViewInfo.h"
#include "Library/Camera/SceneCameraInfo.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveExecutor.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Play/Camera/CameraVerticalAbsorber.h"
#include "Library/Projection/OrthoProjectionInfo.h"
#include "Library/Projection/Projection.h"
#include "Project/Camera/CameraAngleSwingInfo.h"
#include "Project/Camera/CameraInterpole.h"

namespace al {
namespace {
NERVE_IMPL(CameraPoseUpdater, Active);
NERVE_IMPL(CameraPoseUpdater, Deactive);
NERVE_IMPL(CameraPoseUpdater, Stop);
NERVE_IMPL(CameraPoseUpdater, Pause);
NERVE_END_IMPL(CameraPoseUpdater, SnapShot);
NERVE_IMPL(CameraPoseUpdater, SnapShotNoUpdate);

NERVES_MAKE_STRUCT(CameraPoseUpdater, Deactive, Pause, Stop, SnapShotNoUpdate, SnapShot, Active);
}  // namespace

CameraPoseUpdater::CameraPoseUpdater(SceneCameraInfo* sceneCamInfo, s32 viewIdx)
    : NerveExecutor("カメラの姿勢更新"), mSceneCamInfo(sceneCamInfo), mIsMainView(viewIdx == 0),
      mViewIdx(viewIdx) {
    mViewFlag = new CameraViewFlag();
    mProjection = new Projection();
    mOrthoProjectionInfo = new OrthoProjectionInfo();
    mViewInfo = new CameraViewInfo(mViewIdx, mLookAtCamera, *mProjection, *mViewFlag,
                                   *mOrthoProjectionInfo);

    if (!mIsMainView)
        mViewInfo->isValid = false;

    sceneCamInfo->initViewInfo(mViewInfo);
    initNerve(&NrvCameraPoseUpdater.Deactive, 0);
}

CameraPoseUpdater::~CameraPoseUpdater() {
    nn::oe::setScreenShotImageOrientation(nn::album::ImageOrientation_None);
}

void CameraPoseUpdater::init(const CameraParamTransfer* paramTransfer,
                             const CameraStopJudge* stopJudge,
                             CameraStartParamCtrl* startParamCtrl) {
    mParamTransfer = paramTransfer;
    mStopJudge = stopJudge;
    mStartParamCtrl = startParamCtrl;

    mShaker = new CameraShaker();
    mInterpole = new CameraInterpole();

    mLookAtCamera.updateViewMatrix();
    mProjection->setProj(getNearClipDistance(), mFarClipDistance, sead::Mathf::rad2deg(mFovyDegree),
                         mAspect);
}

void CameraPoseUpdater::update() {
    mViewFlag->resetAllFlag();
    *mOrthoProjectionInfo = {};

    if (!isNerve(this, &NrvCameraPoseUpdater.Pause) && !isNerve(this, &NrvCameraPoseUpdater.Stop) &&
        !isNerve(this, &NrvCameraPoseUpdater.SnapShot) &&
        !isNerve(this, &NrvCameraPoseUpdater.SnapShotNoUpdate)) {
        trySwitchCamera();
    }

    updateNerve();

    mViewInfo->isValid = mIsMainView;

    mShaker->update(mSceneCamView->get_10());
    mProjection->setOffset(mShaker->getOffset());
    mSceneCamView->set_10(nullptr);

    mLookAtCamera.updateViewMatrix();

    f32 fovy = mFovyDegree;
    if (mCtrlPausePtr && mCtrlPausePtr->isCameraPause())
        fovy = mCtrlPausePtr->get_4();

    mProjection->setProj(getNearClipDistance(), mFarClipDistance, sead::Mathf::rad2deg(fovy),
                         mAspect);
    mProjection->calcMtx();

    if (mCtrlPausePtr == nullptr || !mCtrlPausePtr->isCameraPause())
        mViewInfo->isFirstCalc = false;
}

bool CameraPoseUpdater::trySwitchCamera() {
    if (mSwitcher == nullptr)
        return false;

    mSwitcher->update();

    if (!mSwitcher->get_10())
        return false;

    bool hasNext = mSwitcher->isExistNextCamera();
    if (!hasNext) {
        mTicket = nullptr;

        bool didDeactivate = false;
        if (!isNerve(this, &NrvCameraPoseUpdater.Deactive)) {
            setNerve(this, &NrvCameraPoseUpdater.Deactive);
            didDeactivate = true;
        }

        return didDeactivate;
    }

    bool didActivate = false;
    if (!isNerve(this, &NrvCameraPoseUpdater.Active)) {
        setNerve(this, &NrvCameraPoseUpdater.Active);
        didActivate = true;
    }

    auto* currTicket = mTicket;
    auto* nextTicket = mSwitcher->getNextCamera();
    mTicket = nextTicket;

    CameraStartInfo camStartInfo;
    camStartInfo.prePriorityType = (CameraTicket::Priority)0xFFFFFFFF;
    camStartInfo.isExistAreaAngleH = false;
    camStartInfo.areaAngleH = 0.0f;
    camStartInfo.isExistAreaAngleV = false;
    camStartInfo.areaAngleV = 0.0f;
    camStartInfo.isExistNextPoseByPreCamera = false;
    camStartInfo.nextAngleHByPreCamera = 0.0f;
    camStartInfo.nextAngleVByPreCamera = 0.0f;
    camStartInfo.isInvalidCollidePreCamera = false;
    camStartInfo.isInvalidKeepPreCameraDistance = false;
    camStartInfo.isInvalidKeepPreCameraDistanceIfNoCollide = false;
    camStartInfo.isValidResetPreCameraPose = false;
    camStartInfo.isValidKeepPreSelfCameraPose = false;
    camStartInfo._25 = false;
    camStartInfo.preCameraName = nullptr;
    camStartInfo.preCameraSwingAngleH = 0.0f;
    camStartInfo.preCameraSwingAngleV = 0.0f;
    camStartInfo.preCameraMaxSwingAngleH = 0.0f;
    camStartInfo.preCameraMaxSwingAngleV = 0.0f;

    if (currTicket) {
        camStartInfo.prePriorityType = (CameraTicket::Priority)currTicket->getPriority();
        camStartInfo.preCameraName = currTicket->getPoser()->getName();
        camStartInfo.isInvalidCollidePreCamera =
            currTicket->getPoser()->getPoserFlag()->isInvalidCollider;
        camStartInfo.isInvalidKeepPreCameraDistance =
            currTicket->getPoser()->getPoserFlag()->isInvalidKeepDistanceNextCamera;
        camStartInfo.isInvalidKeepPreCameraDistanceIfNoCollide =
            currTicket->getPoser()->getPoserFlag()->isInvalidKeepDistanceNextCameraIfNoCollide;
        camStartInfo.isValidResetPreCameraPose = currTicket->getPoser()->getPoserFlag()->_3;

        if (!mSwitcher->isExistNextCamera()) {
            if (currTicket->getPoser()->getPoserFlag()->isValidKeepPreSelfPoseNextCamera())
                camStartInfo.isValidKeepPreSelfCameraPose = true;
        } else {
            camStartInfo.isValidKeepPreSelfCameraPose = false;
        }

        auto* verticalAbsorber = currTicket->getPoser()->getCameraVerticalAbsorber();
        if (verticalAbsorber && verticalAbsorber->isAbsorbing())
            camStartInfo._25 = true;

        if (currTicket->getPoser()->getAngleSwingInfo()) {
            camStartInfo.preCameraSwingAngleH =
                currTicket->getPoser()->getAngleSwingInfo()->currentAngle.x;
            camStartInfo.preCameraSwingAngleV =
                currTicket->getPoser()->getAngleSwingInfo()->currentAngle.y;
            camStartInfo.preCameraMaxSwingAngleH =
                currTicket->getPoser()->getAngleSwingInfo()->maxSwingDegreeH;
            camStartInfo.preCameraMaxSwingAngleV =
                currTicket->getPoser()->getAngleSwingInfo()->maxSwingDegreeV;
        }

        if (mSwitcher->isExistNextCamera()) {
            sead::Vector3f lookDir =
                mSwitcher->getNextPoseInfo()->pos - mSwitcher->getNextPoseInfo()->at;
            normalize(&lookDir);

            camStartInfo.isExistNextPoseByPreCamera = true;
            camStartInfo.nextAngleHByPreCamera =
                sead::Mathf::rad2deg(sead::Mathf::atan2(lookDir.x, lookDir.z));
            camStartInfo.nextAngleVByPreCamera = sead::Mathf::rad2deg(sead::Mathf::asin(lookDir.y));
        }

        mStartParamCtrl->tryApplyParam(&camStartInfo);
        nextTicket = mTicket;
    }

    mTicket->getPoser()->setViewInfo(mViewInfo);
    mTicket->getPoser()->appear(camStartInfo);

    auto* interpole = mInterpole;
    auto* ticket = mTicket;

    if (currTicket) {
        interpole->start(ticket, mFovyDegree, mSwitcher->getNextInterpoleStep());
        mParamTransfer->tryTransferParam(currTicket->getPoser(), mTicket->getPoser());
        return didActivate;
    }

    interpole->setTicket(ticket);
    return didActivate;
}

void CameraPoseUpdater::exeActive() {
    if (isFirstStep(this))
        mIsMainView = true;

    if (mCtrlPausePtr && mCtrlPausePtr->isCameraPause() &&
        !isNerve(this, &NrvCameraPoseUpdater.Pause)) {
        setNerve(this, &NrvCameraPoseUpdater.Pause);
        return;
    }
    if (mStopJudge->isStop()) {
        setNerve(this, &NrvCameraPoseUpdater.Stop);
        return;
    }

    sead::LookAtCamera lookAt;
    auto* orthoProjection = mOrthoProjectionInfo;
    auto* poser = mTicket->getPoser();

    poser->movement();
    poser->calcCameraPose(&lookAt);
    poser->tryCalcOrthoProjectionInfo(orthoProjection);

    mFovyDegree = poser->getFovyDegree();

    mInterpole->update(lookAt);
    mInterpole->makeLookAtCamera(&lookAt);

    if (mInterpole->isActive()) {
        mFovyDegree = mInterpole->getFovyDegree();
        mViewInfo->isActiveInterpole = true;
    } else {
        mViewInfo->isActiveInterpole = false;
    }

    mLookAtCamera.setPos(lookAt.getPos());
    mLookAtCamera.setAt(lookAt.getAt());
    mLookAtCamera.setUp(lookAt.getUp());
    mLookAtCamera.normalizeUp();

    bool invalidBlur = mTicket->getPoser()->getPoserFlag()->isInvalidCameraBlur;
    mViewFlag->setInvalidCameraBlur(invalidBlur);
}

void CameraPoseUpdater::exeDeactive() {
    if (isFirstStep(this))
        mIsMainView = false;

    if (mCtrlPausePtr && mCtrlPausePtr->isCameraPause() &&
        !isNerve(this, &NrvCameraPoseUpdater.Pause)) {
        setNerve(this, &NrvCameraPoseUpdater.Pause);
        return;
    }

    const auto* viewInfo = mSceneCamInfo->getViewAt(0);
    mLookAtCamera.setPos(viewInfo->lookAtCam.getPos());
    mLookAtCamera.setAt(viewInfo->lookAtCam.getAt());
    mLookAtCamera.setUp(viewInfo->lookAtCam.getUp());
    mLookAtCamera.normalizeUp();

    mFovyDegree = sead::Mathf::rad2deg(viewInfo->projection.getFovy());
}

void CameraPoseUpdater::exeStop() {
    if (mCtrlPausePtr && mCtrlPausePtr->isCameraPause() &&
        !isNerve(this, &NrvCameraPoseUpdater.Pause)) {
        setNerve(this, &NrvCameraPoseUpdater.Pause);
        return;
    }

    if (mStopJudge->isStop())
        return;

    if (mTicket) {
        startInterpole(60);
        setNerve(this, &NrvCameraPoseUpdater.Active);
    } else {
        setNerve(this, &NrvCameraPoseUpdater.Deactive);
    }
}

void CameraPoseUpdater::exePause() {
    if (mCtrlPausePtr->isCameraPause())
        return;

    if (mStopJudge->isStop())
        setNerve(this, &NrvCameraPoseUpdater.Stop);
    else if (mTicket)
        setNerve(this, &NrvCameraPoseUpdater.Active);
    else
        setNerve(this, &NrvCameraPoseUpdater.Deactive);
}

void CameraPoseUpdater::exeSnapShot() {
    auto* lookAt = &mLookAtCamera;
    auto* orthoProjection = mOrthoProjectionInfo;
    auto* poser = mTicket->getPoser();

    poser->movement();
    poser->movement();  // Why is it called twice?
    poser->calcCameraPose(lookAt);
    poser->tryCalcOrthoProjectionInfo(orthoProjection);

    mFovyDegree = poser->getFovyDegree();

    nn::album::ImageOrientation orientiation = nn::album::ImageOrientation_None;
    sead::Vector3f lookDir = mLookAtCamera.getAt() - mLookAtCamera.getPos();
    if (tryNormalizeOrZero(&lookDir)) {
        sead::Vector3f worldUp = sead::Vector3f::ey;
        sead::Vector3f cameraUp = mLookAtCamera.getUp();

        verticalizeVec(&worldUp, lookDir, worldUp);
        verticalizeVec(&cameraUp, lookDir, cameraUp);
        if (tryNormalizeOrZero(&worldUp) && tryNormalizeOrZero(&cameraUp)) {
            f32 rotation = calcAngleOnPlaneDegree(worldUp, cameraUp, lookDir);
            orientiation = rotation > 60.0f  ? nn::album::ImageOrientation_Rotate270 :
                           rotation < -60.0f ? nn::album::ImageOrientation_Rotate90 :
                                               nn::album::ImageOrientation_None;
        }
    }

    nn::oe::setScreenShotImageOrientation(orientiation);
    mSnapShotOrientation = orientiation;
}

void CameraPoseUpdater::endSnapShot() {
    nn::oe::setScreenShotImageOrientation(nn::album::ImageOrientation_None);
    mSnapShotOrientation = nn::album::ImageOrientation_None;
}

void CameraPoseUpdater::exeSnapShotNoUpdate() {}

bool CameraPoseUpdater::isActiveInterpole() const {
    return mInterpole->isActive();
}

void CameraPoseUpdater::startInterpole(s32 step) {
    mInterpole->start(mTicket, mFovyDegree, step);
}

void CameraPoseUpdater::requestCancelInterpole() {
    mInterpole->requestCancel();
}

bool CameraPoseUpdater::calcCameraPoseWithoutInterpole(sead::LookAtCamera* camera) const {
    if (mTicket == nullptr)
        return false;

    mTicket->getPoser()->calcCameraPose(camera);
    return true;
}

void CameraPoseUpdater::startSnapShotMode(bool lockCamera) {
    if (mTicket == nullptr)
        return;

    if (isNerve(this, &NrvCameraPoseUpdater.SnapShot) ||
        isNerve(this, &NrvCameraPoseUpdater.SnapShotNoUpdate)) {
        return;
    }

    if (lockCamera || mInterpole->isActive() || isNerve(this, &NrvCameraPoseUpdater.Stop)) {
        setNerve(this, &NrvCameraPoseUpdater.SnapShotNoUpdate);
        return;
    }

    sead::Vector3f translation = {0.0f, 0.0f, 0.0f};
    alCameraPoserFunction::calcTargetTrans(&translation, mTicket->getPoser());

    auto* areaObj = tryFindAreaObj(mTicket->getPoser(), "SnapShotInvalidCtrlArea", translation);
    if (areaObj) {
        bool validCtrl = false;
        if (!tryGetAreaObjArg(&validCtrl, areaObj, "IsValidCtrl") || !validCtrl) {
            setNerve(this, &NrvCameraPoseUpdater.SnapShotNoUpdate);
            return;
        }
    }

    mTicket->getPoser()->startSnapShotModeCore();
    setNerve(this, &NrvCameraPoseUpdater.SnapShot);
}

void CameraPoseUpdater::endSnapShotMode() {
    if (isNerve(this, &NrvCameraPoseUpdater.SnapShot))
        mTicket->getPoser()->endSnapShotModeCore();
    else if (!isNerve(this, &NrvCameraPoseUpdater.SnapShotNoUpdate))
        return;

    if (mStopJudge->isStop())
        setNerve(this, &NrvCameraPoseUpdater.Stop);
    else
        setNerve(this, &NrvCameraPoseUpdater.Active);
}

bool CameraPoseUpdater::isSnapShotOrientationRotate90() const {
    return mSnapShotOrientation == nn::album::ImageOrientation_Rotate90;
}

bool CameraPoseUpdater::isSnapShotOrientationRotate270() const {
    return mSnapShotOrientation == nn::album::ImageOrientation_Rotate270;
}

bool CameraPoseUpdater::isCurrentCameraPriority(s32 priority) const {
    return mTicket && mTicket->getPriority() == priority;
}

bool CameraPoseUpdater::isCurrentCameraZooming() const {
    return mTicket && mTicket->getPoser()->isZooming();
}

bool CameraPoseUpdater::isCurrentCameraEnableRotateByPad() const {
    return mTicket && mTicket->getPoser()->isEnableRotateByPad();
}

bool CameraPoseUpdater::isInvalidChangeSubjectiveCamera() const {
    return mTicket && mTicket->getPoser()->getPoserFlag()->isInvalidChangeSubjective;
}

bool CameraPoseUpdater::tryReceiveCameraRequestFromObject(const CameraObjectRequestInfo& info) {
    return mTicket && mTicket->getPoser()->receiveRequestFromObjectCore(info);
}

bool CameraPoseUpdater::tryRequestCameraTurnToDirection(const CameraTurnInfo* info) {
    return mTicket && mTicket->getPoser()->requestTurnToDirection(info);
}

f32 CameraPoseUpdater::getNearClipDistance() const {
    return mTicket && mTicket->getPoser()->getNearClipDistance() > 0.0f ?
               mTicket->getPoser()->getNearClipDistance() :
               mNearClipDistance;
}

}  // namespace al
