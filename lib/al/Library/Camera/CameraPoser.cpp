#include "Library/Camera/CameraPoser.h"

#include <gfx/seadCamera.h>

#include "Library/Area/AreaObjDirector.h"
#include "Library/Audio/System/AudioKeeper.h"
#include "Library/Base/StringUtil.h"
#include "Library/Camera/CameraArrowCollider.h"
#include "Library/Camera/CameraOffsetCtrlPreset.h"
#include "Library/Camera/CameraParamMoveLimit.h"
#include "Library/Camera/CameraPoserFlag.h"
#include "Library/Camera/CameraPoserFunction.h"
#include "Library/Camera/CameraStartInfo.h"
#include "Library/Camera/CameraTargetAreaLimitter.h"
#include "Library/Camera/SnapShotCameraCtrl.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveKeeper.h"
#include "Library/Play/Camera/CameraVerticalAbsorber.h"
#include "Library/Projection/OrthoProjectionInfo.h"
#include "Library/Rail/RailKeeper.h"
#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/ByamlUtil.h"
#include "Project/Camera/CameraAngleCtrlInfo.h"
#include "Project/Camera/CameraAngleSwingInfo.h"
#include "Project/Camera/CameraObjectRequestInfo.h"

namespace al {

void CameraPoser::appear(const CameraStartInfo& mInfo) {
    mActiveState = ActiveState_Run;
    if (mAngleCtrlInfo != nullptr) {
        sead::Vector3f vec = {0, 0, 0};
        alCameraPoserFunction::calcPreCameraDir(&vec, this);
        mAngleCtrlInfo->start(asinf(vec.y) * 57.296f);
    }

    if (mAngleSwingInfo != nullptr)
        mAngleSwingInfo->setCurrentAngle({0, 0});

    start(mInfo);

    if (mArrowCollider != nullptr && !mPoserFlag->mIsInvalidCollider)
        mArrowCollider->start();

    if (mVerticalAbsorber != nullptr && !mPoserFlag->mIsOffVerticalAbsorb)
        mVerticalAbsorber->start(mTargetTrans, mInfo);

    if (mLookAtInterpole != nullptr)
        mLookAtInterpole->mLookAtPos = mTargetTrans;
}

bool CameraPoser::receiveRequestFromObjectCore(const CameraObjectRequestInfo& mInfo) {
    if (receiveRequestFromObject(mInfo))
        return true;

    if (mVerticalAbsorber != nullptr && mInfo.mIsStopVerticalAbsorb) {
        mVerticalAbsorber->liberateAbsorb();
        return true;
    }
    if (mAngleCtrlInfo != nullptr && mAngleCtrlInfo->receiveRequestFromObject(mInfo))
        return true;

    return false;
}

bool CameraPoser::isInterpoleByCameraDistance() const {
    return mInterpolate->mStepType == 1;
}

bool CameraPoser::isInterpoleEaseOut() const {
    return mInterpolate->mIsEaseOut != 0;
}

bool CameraPoser::isEndInterpoleByStep() const {
    return mInterpolateStep->mStepType == 0;
}

bool CameraPoser::isFirstCalc() const {
    return mPoserFlag->mIsFirstCalc;
}

void CameraPoser::initNerve(const Nerve* nerve, s32 maxStates) {
    mNerveKeeper = new NerveKeeper(this, nerve, maxStates);
}

void CameraPoser::initArrowCollider(CameraArrowCollider* arrowCollider) {
    mArrowCollider = arrowCollider;
    mPoserFlag->mIsInvalidCollider = false;
}

void CameraPoser::initAudioKeeper(const char* name) {
    mAudioKeeper = alAudioKeeperFunction::createAudioKeeper(mCtrls->mAudioDirector, name, NULL);
}

void CameraPoser::initRail(const PlacementInfo& mInfo) {
    mRailKeeper = new RailKeeper(mInfo);
}

void CameraPoser::initOrthoProjectionParam() {
    mOrthoProjectionParam = new OrthoProjectionParam(false, {-1.0f, -1.0f});
}

void CameraPoser::tryInitAreaLimitter(const PlacementInfo& mInfo) {
    mTargetAreaLimitter = CameraTargetAreaLimitter::tryCreate(mInfo);
}

bool CameraPoser::tryCalcOrthoProjectionInfo(OrthoProjectionInfo* projectionInfo) const {
    OrthoProjectionParam* param = mOrthoProjectionParam;
    if (!param || !param->mIsSetInfo || !(param->mInfo.mNearClipWidth > 0.1f) ||
        !(param->mInfo.mNearClipHeight > 0.1f))
        return false;

    *projectionInfo = {param->mInfo.mNearClipWidth, param->mInfo.mNearClipHeight};
    return true;
}

void CameraPoser::makeLookAtCameraPrev(sead::LookAtCamera* cam) const {
    cam->setPos(mPosition);
    cam->setAt(mTargetTrans);
    cam->setUp(mCameraUp);
    cam->normalizeUp();

    if (mVerticalAbsorber != nullptr && !mPoserFlag->mIsOffVerticalAbsorb)
        mVerticalAbsorber->makeLookAtCamera(cam);

    if (mLocalInterpole != nullptr)
        mLocalInterpole->interpolate(cam);

    if (mAngleSwingInfo != nullptr)
        mAngleSwingInfo->makeLookAtCamera(cam);

    if (mTargetAreaLimitter != nullptr) {
        sead::Vector3f camPosNext = cam->getAt();
        if (mTargetAreaLimitter->applyAreaLimit(&camPosNext, camPosNext)) {
            sead::Vector3f camDiff = camPosNext - cam->getAt();
            cam->setPos(camDiff + cam->getPos());
            cam->setAt(camDiff + cam->getAt());
        }
    }
}

void CameraPoser::makeLookAtCameraPost(sead::LookAtCamera* cam) const {
    if (alCameraPoserFunction::isSnapShotMode(this) && mSnapShotCtrl != nullptr)
        mSnapShotCtrl->makeLookAtCameraPost(cam);

    if (mParamMoveLimit != nullptr)
        mParamMoveLimit->apply(cam);
}

void CameraPoser::makeLookAtCameraLast(sead::LookAtCamera* cam) const {
    if (alCameraPoserFunction::isSnapShotMode(this) && mSnapShotCtrl != nullptr)
        mSnapShotCtrl->makeLookAtCameraLast((cam));
}

void CameraPoser::makeLookAtCameraCollide(sead::LookAtCamera* cam) const {
    if (!mPoserFlag->mIsInvalidCollider && mArrowCollider)
        mArrowCollider->makeLookAtCamera(cam);
}

s32 CameraPoser::getEndInterpoleStep() const {
    return mInterpolateStep->mStepNum;
}

s32 CameraPoser::getInterpoleStep() const {
    return mInterpolate->mStepNum < 0 ? 60 : mInterpolate->mStepNum;
}

void CameraPoser::setInterpoleStep(s32 step) {
    mInterpolate->set(0, step, true);
}

void CameraPoser::setInterpoleEaseOut() {
    mInterpolate->mIsEaseOut = true;
}

void CameraPoser::resetInterpoleStep() {
    mInterpolate->set(1, -1, false);
}

void CameraPoser::startSnapShotModeCore() {
    if (mSnapShotCtrl != nullptr)
        mSnapShotCtrl->start(mFovyDegree);

    startSnapShotMode();
}

void CameraPoser::endSnapShotModeCore() {
    endSnapShotMode();
}

f32 CameraPoser::getFovyDegree() const {
    if (alCameraPoserFunction::isSnapShotMode(this) && mSnapShotCtrl)
        return mSnapShotCtrl->getFovyDegree();

    return mFovyDegree;
}

f32 CameraPoser::getSceneFovyDegree() const {
    return mCtrls->mSceneFovyDegree;
}

CameraInputHolder* CameraPoser::getInputHolder() const {
    return mCtrls->mInputHolder;
}

CameraTargetHolder* CameraPoser::getTargetHolder() const {
    return mCtrls->mTargetHolder;
}

CameraFlagCtrl* CameraPoser::getFlagCtrl() const {
    return mCtrls->mFlagCtrl;
}

AreaObjDirector* CameraPoser::getAreaObjDirector() const {
    return mCtrls->mAreaObjDirector;
}

void CameraPoser::initLocalInterpole() {
    mLocalInterpole = new LocalInterpole();
}

void CameraPoser::initLookAtInterpole(f32 v) {
    mLookAtInterpole = new LookAtInterpole(v);
}

void CameraPoser::CameraInterpolateStep::load(const ByamlIter& iter) {
    ByamlIter newIter;
    if (tryGetByamlIterByKey(&newIter, iter, "EndInterpoleParam")) {
        if (isEqualString(getByamlKeyString(newIter, "Type"), "Step"))
            mStepType = 0;
        if (mStepType == 0)
            mStepNum = getByamlKeyInt(newIter, "Step");
    }
}

void CameraPoser::CameraInterpolateParams::load(const ByamlIter& iter) {
    tryGetByamlS32(&mStepType, iter, "InterpoleStepType");

    const char* curveType = nullptr;
    if (tryGetByamlString(&curveType, iter, "InterpoleCurveType") != 0 && curveType &&
        isEqualString(curveType, "EaseOut"))
        mIsEaseOut = true;

    bool isInterpolate = tryGetByamlS32(&mStepNum, iter, "InterpoleStep");
    mIsInterpolate = isInterpolate;
    if (isInterpolate)
        mStepType = 0;
}

void CameraPoser::CameraInterpolateParams::set(s32 type, s32 step, bool isInterpolate) {
    mStepType = type;
    mStepNum = step;
    mIsInterpolate = isInterpolate;
}

inline void CameraPoser::OrthoProjectionParam::load(const ByamlIter& iter) {
    bool isExist = tryGetByamlBool(&mIsSetInfo, iter, "IsSetOrthoProjectionInfo");

    if (isExist && mIsSetInfo) {
        tryGetByamlF32(&mInfo.mNearClipWidth, iter, "OrthoProjectionNearClipWidth");
        tryGetByamlF32(&mInfo.mNearClipHeight, iter, "OrthoProjectionNearClipHeight");
    }
}

void CameraPoser::LocalInterpole::interpolate(sead::LookAtCamera* cam) {
    if (_0 > -1) {
        f32 rate = hermiteRate(normalize(_0, 0L, _4), 1.5f, 0.0f);

        sead::Vector3f camPosNext = sead::Vector3f(0, 0, 0);
        sead::Vector3f lookAtPosNext = sead::Vector3f(0, 0, 0);
        lerpVec(&camPosNext, mCameraPos, cam->getPos(), rate);
        lerpVec(&lookAtPosNext, mLookAtPos, cam->getAt(), rate);

        cam->setPos(camPosNext);
        cam->setAt(lookAtPosNext);
    }
}

void CameraPoser::load(const ByamlIter& iter) {
    loadParam(iter);
    tryGetByamlF32(&mFovyDegree, iter, "FovyDegree");

    mPoserFlag->load(iter);
    mInterpolate->load(iter);
    mInterpolateStep->load(iter);

    if (mVerticalAbsorber != nullptr)
        mVerticalAbsorber->load(iter);

    if (mAngleCtrlInfo != nullptr)
        mAngleCtrlInfo->load(iter);

    if (mAngleSwingInfo != nullptr)
        mAngleSwingInfo->load(iter);

    if (mOffsetCtrlPreset != nullptr)
        mOffsetCtrlPreset->load(iter);

    if (mParamMoveLimit != nullptr)
        mParamMoveLimit->load(iter);

    if (mSnapShotCtrl != nullptr)
        mSnapShotCtrl->load(iter);

    if (mOrthoProjectionParam != nullptr)
        mOrthoProjectionParam->load(iter);
}

void CameraPoser::init() {}

void CameraPoser::initByPlacementObj(const PlacementInfo& mInfo) {}

void CameraPoser::endInit() {}

void CameraPoser::start(const CameraStartInfo& mInfo) {}

void CameraPoser::update() {}

void CameraPoser::end() {
    mActiveState = ActiveState_End;
}

void CameraPoser::loadParam(const ByamlIter& iter) {}

void CameraPoser::makeLookAtCamera(sead::LookAtCamera* cam) const {}

bool CameraPoser::receiveRequestFromObject(const CameraObjectRequestInfo& mInfo) {
    return false;
}

bool CameraPoser::isZooming() const {
    return false;
}

bool CameraPoser::isEnableRotateByPad() const {
    if (mAngleCtrlInfo != nullptr)
        return !mAngleCtrlInfo->isFixByRangeHV();

    return mAngleSwingInfo && !mAngleSwingInfo->isInvalidSwing();
}

void CameraPoser::startSnapShotMode() {}

void CameraPoser::endSnapShotMode() {}

const char* CameraPoser::getName() const {
    return mPoserName;
}

CollisionDirector* CameraPoser::getCollisionDirector() const {
    return mCtrls->mCollisionDirector;
}

NerveKeeper* CameraPoser::getNerveKeeper() const {
    return mNerveKeeper;
}

AudioKeeper* CameraPoser::getAudioKeeper() const {
    return mAudioKeeper;
}

RailRider* CameraPoser::getRailRider() const {
    return mRailKeeper ? mRailKeeper->getRailRider() : nullptr;
}

// NON-MATCHING
void CameraPoser::calcCameraPose(sead::LookAtCamera* cam) const {
    makeLookAtCameraPrev(cam);
    makeLookAtCamera(cam);

    if (alCameraPoserFunction::isSnapShotMode(this) && mSnapShotCtrl)
        mSnapShotCtrl->makeLookAtCameraPost(cam);

    if (mParamMoveLimit != nullptr)
        mParamMoveLimit->apply(cam);

    if (!mPoserFlag->mIsInvalidCollider && mArrowCollider != nullptr)
        mArrowCollider->makeLookAtCamera(cam);

    if (alCameraPoserFunction::isSnapShotMode(this) && mSnapShotCtrl)
        mSnapShotCtrl->makeLookAtCameraLast(cam);
}

bool CameraPoser::requestTurnToDirection(const CameraTurnInfo* mInfo) {
    return false;
}
}  // namespace al
