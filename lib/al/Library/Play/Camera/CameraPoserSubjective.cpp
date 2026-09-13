#include "Library/Play/Camera/CameraPoserSubjective.h"

#include <math/seadMathCalcCommon.h>

#include "Library/Camera/CameraPoser.h"
#include "Library/Camera/CameraPoserFunction.h"
#include "Library/Controller/PadRumbleFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/IntervalTrigger.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Se/SeFunction.h"
#include "Library/Yaml/ByamlUtil.h"

namespace al {
namespace {
NERVE_IMPL(CameraPoserSubjective, Reset);
NERVE_IMPL(CameraPoserSubjective, Wait);
NERVES_MAKE_NOSTRUCT(CameraPoserSubjective, Wait, Reset)
}  // namespace

CameraPoserSubjective::CameraPoserSubjective(const char* name) : CameraPoser(name) {}

void CameraPoserSubjective::init() {
    initNerve(&Wait, 0);
    mFovyDegree = 45.0f;
    setInterpoleStep(10);
    alCameraPoserFunction::initGyroCameraCtrl(this);
    alCameraPoserFunction::initSnapShotCameraCtrl(this);
    mIntervalTrigger = new IntervalTrigger(15.0f);
}

void CameraPoserSubjective::loadParam(const ByamlIter& param) {
    tryGetByamlF32(&mMinAngleV, param, "MinAngleV");
    tryGetByamlF32(&mMaxAngleV, param, "MaxAngleV");
    tryGetByamlF32(&mCameraOffsetUp, param, "CameraOffsetUp");
    tryGetByamlF32(&mStartAngleV, param, "StartAngleV");
    tryGetByamlBool(&mIsSetStartAngleH, param, "IsSetStartAngleH");
    tryGetByamlF32(&mStartAngleH, param, "StartAngleH");
};

void CameraPoserSubjective::start(const CameraStartInfo& info) {
    mFovyDegree = 45.0f;
    if (alCameraPoserFunction::isPrePriorityDemoAll(info))
        mCurrentAngleV = -alCameraPoserFunction::calcPreCameraAngleV(this);
    else
        mCurrentAngleV = mStartAngleV;
    mTargetAngleV = mCurrentAngleV;
    if (!mIsSetStartAngleH || alCameraPoserFunction::isPrePriorityDemoAll(info)) {
        sead::Vector3f lookDir = {0.0f, 0.0f, 0.0f};
        alCameraPoserFunction::calcPreLookDirH(&lookDir, this);
        mInitialAngleH = sead::Mathf::rad2deg(sead::Mathf::atan2(lookDir.x, lookDir.z));
    } else {
        mInitialAngleH = mStartAngleH;
    }
    mCurrentAngleH = 0.0f;
    _160 = 0.0f;
    mTargetAngleH = 0.0f;
    mGyroAngleH = 0.0f;
    mGyroAngleV = 0.0f;
    _15c = 0.0f;
    alCameraPoserFunction::resetGyro(this);
    alCameraPoserFunction::reduceGyroSencitivity(this);
    setNerve(this, &Wait);
    update();
}

void CameraPoserSubjective::movement() {
    mIsZooming = false;
    CameraPoser::movement();
}

void CameraPoserSubjective::update() {
    sead::Vector3f newTarget = sead::Vector3f::ez;

    f32 horizontalAngle =
        modf(mCurrentAngleH + mGyroAngleH + mInitialAngleH + 360.0f, 360.0f) + 0.0f;
    f32 verticalAngle = sead::Mathf::clamp(mCurrentAngleV + mGyroAngleV, mMinAngleV, mMaxAngleV);

    // rotate horizontally
    rotateVectorDegree(&newTarget, newTarget, mCameraUp, horizontalAngle);
    // rotate vertically
    rotateVectorDegree(&newTarget, newTarget, newTarget.cross(mCameraUp), verticalAngle);

    alCameraPoserFunction::setCameraPosToTargetAddOffset(this, newTarget * 50.0f +
                                                                   mCameraOffsetUp * mCameraUp);

    mTargetTrans = newTarget * 100.0f + mPosition;

    if (!mActor || mIsSnapshotModeActive)
        return;

    f32 angleDelta = sead::Mathf::abs(verticalAngle - mPrevAngleV) +
                     sead::Mathf::abs(horizontalAngle - mPrevAngleH);

    mIntervalTrigger->update(angleDelta);

    if (mIntervalTrigger->isTriggered()) {
        f32 volumeLeft = sead::Mathf::clamp(angleDelta * 0.06f + 0.1f, 0.0f, 1.0f);
        f32 volumeRight = sead::Mathf::clamp(angleDelta * 0.06f + 0.25f, 0.0f, 1.0f);
        alPadRumbleFunction::startPadRumbleNo3DWithParam(mActor, "パルス（中）", volumeLeft,
                                                         volumeRight, 1.0, 1.0, -1);
        startSeWithParam(mActor, "PgCameraMoveTrig", angleDelta, "");
    }

    mPrevAngleV = verticalAngle;
    mPrevAngleH = horizontalAngle;
}

void CameraPoserSubjective::startSnapShotMode() {
    alCameraPoserFunction::stopUpdateGyro(this);
    mIsSnapshotModeActive = true;
}

void CameraPoserSubjective::endSnapShotMode() {
    alCameraPoserFunction::restartUpdateGyro(this);
    mIsSnapshotModeActive = false;
}

void CameraPoserSubjective::exeWait() {
    f32 fovyDegree = mFovyDegree;
    if (!alCameraPoserFunction::isSnapShotMode(this)) {
        if (mIsRequestZoomIn) {
            fovyDegree = lerpValue(fovyDegree, 10.0f, 0.05f);
            mIsZooming = true;
            mIsRequestZoomIn = false;
        } else {
            fovyDegree = lerpValue(fovyDegree, 45.0f, 0.15f);
        }
        mFovyDegree = fovyDegree;
    }

    f32 zoomLevel = 1.0f - normalize(fovyDegree, 10.0f, 45.0f);

    sead::Vector2f stick = {0.0f, 0.0f};
    alCameraPoserFunction::calcCameraRotateStick(&stick, this);

    mTargetAngleH = (mTargetAngleH - stick.x * lerpValue(0.2f, 0.035f, zoomLevel)) * 0.85f;
    mCurrentAngleH = modf(mCurrentAngleH + mTargetAngleH + 360.0f, 360.0f) + 0.0f;

    mTargetAngleV = sead::Mathf::clamp(
        lerpValue(mTargetAngleV, mTargetAngleV + stick.y * lerpValue(1.8f, 0.3f, zoomLevel), 0.7f),
        mMinAngleV, mMaxAngleV);
    mCurrentAngleV = lerpValue(mCurrentAngleV, mTargetAngleV, 0.1f);

    if (alCameraPoserFunction::isTriggerCameraResetRotate(this))
        setNerve(this, &Reset);
    else {
        alCameraPoserFunction::setGyroSensitivity(this, lerpValue(1.5f, 0.75f, zoomLevel),
                                                  lerpValue(1.75f, 1.0f, zoomLevel));
        mGyroAngleH = alCameraPoserFunction::getGyroAngleH(this);
        mGyroAngleV = alCameraPoserFunction::getGyroAngleV(this);
    }
}

void CameraPoserSubjective::exeReset() {
    if (isFirstStep(this)) {
        mResetAngleH = mCurrentAngleH = modf(mCurrentAngleH + mGyroAngleH + 360.0f, 360.0f) + 0.0f;
        mResetAngleV = mCurrentAngleV + mGyroAngleV;
        mTargetAngleV = mStartAngleV;
        alCameraPoserFunction::resetGyro(this);
        mGyroAngleH = 0.0f;
        _15c = 0.0f;
        _160 = 0.0f;
    }
    f32 time = calcNerveEaseOutRate(this, 15);
    if (mIsResetAngleHValid)
        mCurrentAngleH = lerpDegree(mResetAngleH, 0.0f, time);
    mGyroAngleV = alCameraPoserFunction::getGyroAngleV(this);
    mGyroAngleH = alCameraPoserFunction::getGyroAngleH(this);
    mCurrentAngleV = lerpValue(mResetAngleV, mStartAngleV, time);

    if (isGreaterEqualStep(this, 15))
        setNerve(this, &Wait);
}

f32 CameraPoserSubjective::getCameraOffsetFront() {
    return 50.0f;
}
}  // namespace al
