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
#include "Library/Camera/CameraPoserSceneInfo.h"
#include "Library/Camera/CameraStartInfo.h"
#include "Library/Camera/CameraTargetAreaLimitter.h"
#include "Library/Camera/CameraViewCtrlGyro.h"
#include "Library/Camera/SnapShotCameraCtrl.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveKeeper.h"
#include "Library/Play/Camera/CameraVerticalAbsorber.h"
#include "Library/Rail/RailKeeper.h"
#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/ByamlUtil.h"
#include "Project/Camera/CameraAngleCtrlInfo.h"
#include "Project/Camera/CameraAngleSwingInfo.h"
#include "Project/Camera/CameraObjectRequestInfo.h"

namespace al {

CameraPoser::CameraPoser(const char* name) : mPoserName(name) {
    mPoserFlag = new CameraPoserFlag();
    mActiveInterpoleParam = new CameraInterpoleParam();
    mEndInterpoleParam = new CameraInterpoleStep();
};

void CameraPoser::init() {}

void CameraPoser::initByPlacementObj(const PlacementInfo& mInfo) {}

void CameraPoser::endInit() {}

void CameraPoser::start(const CameraStartInfo& mInfo) {}

void CameraPoser::update() {}

void CameraPoser::end() {
    mActiveState = ActiveState::End;
}

void CameraPoser::loadParam(const ByamlIter& iter) {}

void CameraPoser::makeLookAtCamera(sead::LookAtCamera* cam) const {}

bool CameraPoser::receiveRequestFromObject(const CameraObjectRequestInfo& mInfo) {
    return false;
}

bool CameraPoser::isZooming() const {
    return false;
}

void CameraPoser::startSnapShotMode() {}

void CameraPoser::endSnapShotMode() {}

const char* CameraPoser::getName() const {
    return mPoserName;
}

NerveKeeper* CameraPoser::getNerveKeeper() const {
    return mNerveKeeper;
}

AudioKeeper* CameraPoser::getAudioKeeper() const {
    return mAudioKeeper;
}

bool CameraPoser::requestTurnToDirection(const CameraTurnInfo* mInfo) {
    return false;
}

bool CameraPoser::tryCalcOrthoProjectionInfo(OrthoProjectionInfo* projectionInfo) const {
    OrthoProjectionParam* param = mOrthoProjectionParam;
    if (param && param->isSetInfo && param->info.nearClipWidth > 0.1f &&
        param->info.nearClipHeight > 0.1f) {
        *projectionInfo = {param->info.nearClipWidth, param->info.nearClipHeight};
        return true;
    }
    return false;
}

bool CameraPoser::isEnableRotateByPad() const {
    if (mAngleCtrlInfo != nullptr)
        return !mAngleCtrlInfo->isFixByRangeHV();

    if (mAngleSwingInfo != nullptr)
        return !mAngleSwingInfo->isInvalidSwing();

    return false;
}

f32 CameraPoser::getFovyDegree() const {
    if (alCameraPoserFunction::isSnapShotMode(this) && mSnapShotCtrl)
        return mSnapShotCtrl->getFovyDegree();

    return mFovyDegree;
}

f32 CameraPoser::getSceneFovyDegree() const {
    return mSceneInfo->sceneFovyDegree;
}

AreaObjDirector* CameraPoser::getAreaObjDirector() const {
    return mSceneInfo->areaObjDirector;
}

CollisionDirector* CameraPoser::getCollisionDirector() const {
    return mSceneInfo->collisionDirector;
}

CameraInputHolder* CameraPoser::getInputHolder() const {
    return mSceneInfo->inputHolder;
}

CameraTargetHolder* CameraPoser::getTargetHolder() const {
    return mSceneInfo->targetHolder;
}

CameraFlagCtrl* CameraPoser::getFlagCtrl() const {
    return mSceneInfo->flagCtrl;
}

RailRider* CameraPoser::getRailRider() const {
    return mRailKeeper ? mRailKeeper->getRailRider() : nullptr;
}

bool CameraPoser::isInterpoleByCameraDistance() const {
    return mActiveInterpoleParam->stepType == CameraInterpoleStepType::ByCameraDistance;
}

s32 CameraPoser::getInterpoleStep() const {
    return mActiveInterpoleParam->stepNum < 0 ? 60 : mActiveInterpoleParam->stepNum;
}

void CameraPoser::setInterpoleStep(s32 step) {
    mActiveInterpoleParam->set(CameraInterpoleStepType::ByStep, step, true);
}

void CameraPoser::resetInterpoleStep() {
    mActiveInterpoleParam->set(CameraInterpoleStepType::ByCameraDistance, -1, false);
}

bool CameraPoser::isInterpoleEaseOut() const {
    return mActiveInterpoleParam->isEaseOut;
}

void CameraPoser::setInterpoleEaseOut() {
    mActiveInterpoleParam->isEaseOut = true;
}

bool CameraPoser::isEndInterpoleByStep() const {
    return mEndInterpoleParam->stepType == CameraInterpoleStepType::ByStep;
}

s32 CameraPoser::getEndInterpoleStep() const {
    return mEndInterpoleParam->stepNum;
}

void CameraPoser::initNerve(const Nerve* nerve, s32 maxStates) {
    mNerveKeeper = new NerveKeeper(this, nerve, maxStates);
}

void CameraPoser::initArrowCollider(CameraArrowCollider* arrowCollider) {
    mArrowCollider = arrowCollider;
    mPoserFlag->isInvalidCollider = false;
}

void CameraPoser::initAudioKeeper(const char* name) {
    mAudioKeeper =
        alAudioKeeperFunction::createAudioKeeper(mSceneInfo->audioDirector, name, nullptr);
}

void CameraPoser::initRail(const PlacementInfo& mInfo) {
    mRailKeeper = new RailKeeper(mInfo);
}

void CameraPoser::initLocalInterpole() {
    mLocalInterpole = new LocalInterpole();
}

void CameraPoser::initLookAtInterpole(f32 v) {
    mLookAtInterpole = new LookAtInterpole(v);
}

void CameraPoser::initOrthoProjectionParam() {
    mOrthoProjectionParam = new OrthoProjectionParam();
}

void CameraPoser::tryInitAreaLimitter(const PlacementInfo& mInfo) {
    mTargetAreaLimitter = CameraTargetAreaLimitter::tryCreate(mInfo);
}

inline void CameraPoser::CameraInterpoleParam::set(CameraInterpoleStepType type, s32 step,
                                                   bool is_interpolate_by_step) {
    stepType = type;
    stepNum = step;
    isInterpolateByStep = is_interpolate_by_step;
}

inline void CameraPoser::CameraInterpoleParam::load(const ByamlIter& iter) {
    tryGetByamlS32((s32*)&stepType, iter, "InterpoleStepType");

    const char* curveType = nullptr;
    if (tryGetByamlString(&curveType, iter, "InterpoleCurveType") != 0 && curveType &&
        isEqualString(curveType, "EaseOut"))
        isEaseOut = true;

    isInterpolateByStep = tryGetByamlS32(&stepNum, iter, "InterpoleStep");
    if (isInterpolateByStep)
        stepType = CameraInterpoleStepType::ByStep;
}

inline void CameraPoser::CameraInterpoleStep::load(const ByamlIter& iter) {
    ByamlIter newIter;
    if (tryGetByamlIterByKey(&newIter, iter, "EndInterpoleParam")) {
        if (isEqualString(getByamlKeyString(newIter, "Type"), "Step"))
            stepType = CameraInterpoleStepType::ByStep;
        if (stepType == CameraInterpoleStepType::ByStep)
            stepNum = getByamlKeyInt(newIter, "Step");
    }
}

inline void CameraPoser::OrthoProjectionParam::load(const ByamlIter& iter) {
    bool isExist = tryGetByamlBool(&isSetInfo, iter, "IsSetOrthoProjectionInfo");

    if (isExist && isSetInfo) {
        tryGetByamlF32(&info.nearClipWidth, iter, "OrthoProjectionNearClipWidth");
        tryGetByamlF32(&info.nearClipHeight, iter, "OrthoProjectionNearClipHeight");
    }
}

void CameraPoser::load(const ByamlIter& iter) {
    loadParam(iter);
    tryGetByamlF32(&mFovyDegree, iter, "FovyDegree");

    mPoserFlag->load(iter);
    mActiveInterpoleParam->load(iter);
    mEndInterpoleParam->load(iter);

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

bool CameraPoser::isFirstCalc() const {
    return mPoserFlag->isFirstCalc;
}

void CameraPoser::appear(const CameraStartInfo& mInfo) {
    mActiveState = ActiveState::Active;
    if (mAngleCtrlInfo != nullptr) {
        sead::Vector3f vec = {0, 0, 0};
        alCameraPoserFunction::calcPreCameraDir(&vec, this);
        mAngleCtrlInfo->start(sead::Mathf::rad2deg(asinf(vec.y)));
    }

    if (mAngleSwingInfo != nullptr)
        mAngleSwingInfo->setCurrentAngle({0, 0});

    start(mInfo);

    if (mArrowCollider != nullptr && !mPoserFlag->isInvalidCollider)
        mArrowCollider->start();

    if (mVerticalAbsorber != nullptr && !mPoserFlag->isOffVerticalAbsorb)
        mVerticalAbsorber->start(mTargetTrans, mInfo);

    if (mLookAtInterpole != nullptr)
        mLookAtInterpole->lookAtPos.set(mTargetTrans);
}

// TODO: CameraPoser::movement

inline void CameraPoser::LocalInterpole::interpolate(sead::LookAtCamera* cam) {
    if (step > -1) {
        f32 rate = hermiteRate(normalize(step, 0, end), 1.5f, 0.0f);

        sead::Vector3f camPosNext = sead::Vector3f(0, 0, 0);
        sead::Vector3f lookAtPosNext = sead::Vector3f(0, 0, 0);
        lerpVec(&camPosNext, prevCameraPos, cam->getPos(), rate);
        lerpVec(&lookAtPosNext, prevLookAtPos, cam->getAt(), rate);

        cam->setPos(camPosNext);
        cam->setAt(lookAtPosNext);
    }
}

void CameraPoser::makeLookAtCameraPrev(sead::LookAtCamera* cam) const {
    cam->setPos(mPosition);
    cam->setAt(mTargetTrans);
    cam->setUp(mCameraUp);
    cam->normalizeUp();

    if (mVerticalAbsorber != nullptr && !mPoserFlag->isOffVerticalAbsorb)
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
    if (!mPoserFlag->isInvalidCollider && mArrowCollider)
        mArrowCollider->makeLookAtCamera(cam);
}

void CameraPoser::calcCameraPose(sead::LookAtCamera* cam) const {
    makeLookAtCameraPrev(cam);
    makeLookAtCamera(cam);
    makeLookAtCameraPost(cam);
    makeLookAtCameraCollide(cam);
    makeLookAtCameraLast(cam);
}

bool CameraPoser::receiveRequestFromObjectCore(const CameraObjectRequestInfo& mInfo) {
    if (receiveRequestFromObject(mInfo))
        return true;

    if (mVerticalAbsorber != nullptr && mInfo.isStopVerticalAbsorb) {
        mVerticalAbsorber->liberateAbsorb();
        return true;
    }
    if (mAngleCtrlInfo != nullptr && mAngleCtrlInfo->receiveRequestFromObject(mInfo))
        return true;

    return false;
}

void CameraPoser::startSnapShotModeCore() {
    if (mSnapShotCtrl != nullptr)
        mSnapShotCtrl->start(mFovyDegree);

    startSnapShotMode();
}

void CameraPoser::endSnapShotModeCore() {
    endSnapShotMode();
}
}  // namespace al
