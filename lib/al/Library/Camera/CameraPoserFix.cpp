#include "Library/Camera/CameraPoserFix.h"

#include "Library/Base/StringUtil.h"
#include "Library/Camera/CameraPoser.h"
#include "Library/Camera/CameraPoserFunction.h"
#include "Library/Camera/CameraStartInfo.h"
#include "Library/Math/MathUtil.h"
#include "Library/Yaml/ByamlUtil.h"

namespace al {

CameraPoserFix::CameraPoserFix(const char* name) : CameraPoser(name) {
    if (isEqualString(name, getFixAbsoluteCameraName()))
        alCameraPoserFunction::invalidateChangeSubjective(this);

    else if (isEqualString(name, getFixDoorwayCameraName())) {
        alCameraPoserFunction::invalidateChangeSubjective(this);
        alCameraPoserFunction::initAngleSwing(this);

    } else {
        alCameraPoserFunction::initAngleSwing(this);
        alCameraPoserFunction::validateCtrlSubjective(this);
    }
    initOrthoProjectionParam();
}

void CameraPoserFix::init() {
    alCameraPoserFunction::initSnapShotCameraCtrlZoomAutoReset(this);
}

void CameraPoserFix::initCameraPosAndLookAtPos(const sead::Vector3f& cameraPos,
                                               const sead::Vector3f& lookAtPos) {
    mLookAtPos.set(lookAtPos);
    mDistance = (lookAtPos - cameraPos).length();
    sead::Vector3f viewDir;
    viewDir.set(cameraPos - lookAtPos);
    normalize(&viewDir);
    mAngleV = sead::Mathf::rad2deg(asinf(viewDir.y));
    sead::Vector3f viewDirPlane = viewDir;
    viewDirPlane.y = 0;
    tryNormalizeOrDirZ(&viewDirPlane);
    mAngleH = calcAngleOnPlaneDegree(sead::Vector3f::ez, viewDirPlane, sead::Vector3f::ey);
}

void CameraPoserFix::loadParam(const ByamlIter& iter) {
    tryGetByamlV3f(&mLookAtPos, iter, "LookAtPos");
    tryGetByamlF32(&mDistance, iter, "Distance");
    tryGetByamlF32(&mAngleV, iter, "AngleV");
    tryGetByamlF32(&mAngleH, iter, "AngleH");
    tryGetByamlBool(&mIsCalcNearestAtFromPreAt, iter, "IsCalcNearestAtFromPreAt");
}

void CameraPoserFix::start(const CameraStartInfo& startInfo) {
    mPreLookAtPos.set(alCameraPoserFunction::getPreLookAtPos(this));
    update();
}

void CameraPoserFix::update() {
    mCameraUp.set(sead::Vector3f::ez);
    mTargetTrans.set(mLookAtPos);

    mTargetTrans *= mViewMtx;

    f32 angleH = alCameraPoserFunction::calcZoneRotateAngleH(mAngleH, this);
    f32 x = sinf(sead::Mathf::deg2rad(angleH)) * cosf(sead::Mathf::deg2rad(mAngleV));
    f32 y = sinf(sead::Mathf::deg2rad(mAngleV));
    f32 z = cosf(sead::Mathf::deg2rad(angleH)) * cosf(sead::Mathf::deg2rad(mAngleV));
    sead::Vector3f viewDir = {x, y, z};

    normalize(&viewDir);
    mPosition.set((mDistance * viewDir) + mTargetTrans);
    if (mIsCalcNearestAtFromPreAt) {
        sead::Vector3f offset = mPreLookAtPos - mPosition;
        parallelizeVec(&offset, viewDir, offset);
        if (!isNearZero(offset) && viewDir.dot(offset) < 0.0f)
            mTargetTrans.set(offset + mPosition);
    }
}

const char* CameraPoserFix::getFixAbsoluteCameraName() {
    return "完全固定";
}

const char* CameraPoserFix::getFixDoorwayCameraName() {
    return "出入口専用固定";
}
}  // namespace al
