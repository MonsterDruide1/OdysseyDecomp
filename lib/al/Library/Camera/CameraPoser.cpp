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
    if (mAngleCtrlInfo)
        return !mAngleCtrlInfo->isFixByRangeHV();

    if (mAngleSwingInfo)
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

void CameraPoser::initRail(const PlacementInfo& info) {
    mRailKeeper = new RailKeeper(info);
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

void CameraPoser::tryInitAreaLimitter(const PlacementInfo& info) {
    mTargetAreaLimitter = CameraTargetAreaLimitter::tryCreate(info);
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

    if (mVerticalAbsorber)
        mVerticalAbsorber->load(iter);

    if (mAngleCtrlInfo)
        mAngleCtrlInfo->load(iter);

    if (mAngleSwingInfo)
        mAngleSwingInfo->load(iter);

    if (mOffsetCtrlPreset)
        mOffsetCtrlPreset->load(iter);

    if (mParamMoveLimit)
        mParamMoveLimit->load(iter);

    if (mSnapShotCtrl)
        mSnapShotCtrl->load(iter);

    if (mOrthoProjectionParam)
        mOrthoProjectionParam->load(iter);
}

bool CameraPoser::isFirstCalc() const {
    return mPoserFlag->isFirstCalc;
}

void CameraPoser::appear(const CameraStartInfo& info) {
    mActiveState = ActiveState::Active;
    if (mAngleCtrlInfo) {
        sead::Vector3f vec = {0, 0, 0};
        alCameraPoserFunction::calcPreCameraDir(&vec, this);
        mAngleCtrlInfo->start(sead::Mathf::rad2deg(asinf(vec.y)));
    }

    if (mAngleSwingInfo)
        mAngleSwingInfo->setCurrentAngle({0, 0});

    start(info);

    if (mArrowCollider && !mPoserFlag->isInvalidCollider)
        mArrowCollider->start();

    if (mVerticalAbsorber && !mPoserFlag->isOffVerticalAbsorb)
        mVerticalAbsorber->start(mTargetTrans, info);

    if (mLookAtInterpole)
        mLookAtInterpole->lookAtPos.set(mTargetTrans);
}

void CameraPoser::LookAtInterpole::update(CameraPoser* camera, sead::Vector3f targetTrans) {
    lerpVec(camera->getTargetTransPtr(), lookAtPos, camera->getTargetTrans(), lookAtDistance);
    lookAtPos.set(camera->getTargetTrans());
    camera->addPosition(camera->getTargetTrans() - targetTrans);
}

void CameraPoser::LookAtInterpole::updateWithGravity(CameraPoser* camera,
                                                     const sead::Vector3f& targetGravity,
                                                     sead::Vector3f targetTrans) {
    sead::Vector3f lookDirection = camera->getTargetTrans() - lookAtPos;
    sead::Vector3f gravity = {0.0f, 0.0f, 0.0f};
    parallelizeVec(&gravity, targetGravity, lookDirection);
    sead::Vector3f dir = lookDirection - gravity;
    camera->getTargetTransPtr()->set(gravity + lookAtPos + dir * lookAtDistance);
    lookAtPos.set(camera->getTargetTrans());
    camera->addPosition(camera->getTargetTrans() - targetTrans);
}

void CameraPoser::LocalInterpole::update(const CameraPoser* camera) {
    if (alCameraPoserFunction::isChangeTarget(camera)) {
        step = 0;
        end = 30;
        prevCameraPos.set(alCameraPoserFunction::getPreCameraPos(camera));
        prevLookAtPos.set(alCameraPoserFunction::getPreLookAtPos(camera));
    }
}

void CameraPoser::movement() {
    if (mNerveKeeper)
        mNerveKeeper->update();

    if (mAngleCtrlInfo || mAngleSwingInfo) {
        sead::Vector2f stick = {0.0f, 0.0f};
        alCameraPoserFunction::calcCameraRotateStick(&stick, this);
        if (mAngleCtrlInfo) {
            bool isTriggerReset = alCameraPoserFunction::isTriggerCameraResetRotate(this);
            mAngleCtrlInfo->update(stick, alCameraPoserFunction::getStickSensitivityScale(this),
                                   isTriggerReset);
            if (alCameraPoserFunction::isSnapShotMode(this) && isTriggerReset) {
                s32 step = -1;
                if (mAngleCtrlInfo->isResetStartTiming())
                    step = mAngleCtrlInfo->getMaxResetStep();
                alCameraPoserFunction::startResetSnapShotCameraCtrl(this, step);
            }
        }
        if (mAngleSwingInfo)
            mAngleSwingInfo->update(stick, alCameraPoserFunction::getStickSensitivityScale(this));
    }

    if (mGyroCtrl && !alCameraPoserFunction::isStopUpdateGyro(this)) {
        sead::Vector3f side, up, front;
        alCameraPoserFunction::calcCameraGyroPose(this, &side, &up, &front);
        mGyroCtrl->setIsValidGyro(alCameraPoserFunction::isValidGyro(this));
        mGyroCtrl->setSensitivityScale(alCameraPoserFunction::getGyroSensitivityScale(this));
        mGyroCtrl->update(side, up, front);
    }

    update();

    if (mLookAtInterpole) {
        if (mVerticalAbsorber && !mPoserFlag->isOffVerticalAbsorb) {
            sead::Vector3f targetGravity = {0.0f, 0.0f, 0.0f};
            alCameraPoserFunction::calcTargetGravity(&targetGravity, this);
            mLookAtInterpole->updateWithGravity(this, targetGravity, mTargetTrans);
        } else {
            mLookAtInterpole->update(this, mTargetTrans);
        }
    }

    if (mVerticalAbsorber && !mPoserFlag->isOffVerticalAbsorb)
        mVerticalAbsorber->update();

    if (alCameraPoserFunction::isSnapShotMode(this) && mSnapShotCtrl)
        alCameraPoserFunction::updateSnapShotCameraCtrl(this);

    if (mLocalInterpole) {
        mLocalInterpole->update(this);
        if (mLocalInterpole->step > -1) {
            s32 nextStep = mLocalInterpole->step + 1;
            mLocalInterpole->step = mLocalInterpole->end > nextStep ? nextStep : -1;
        }
    }

    if (mArrowCollider && !mPoserFlag->isInvalidCollider) {
        sead::LookAtCamera camera;
        makeLookAtCameraPrev(&camera);
        makeLookAtCamera(&camera);

        if (alCameraPoserFunction::isSnapShotMode(this) && mSnapShotCtrl)
            mSnapShotCtrl->makeLookAtCameraPost(&camera);
        if (mParamMoveLimit)
            mParamMoveLimit->apply(&camera);

        if (alCameraPoserFunction::isSnapShotMode(this) && mSnapShotCtrl)
            mSnapShotCtrl->makeLookAtCameraLast(&camera);
        mArrowCollider->update(camera.getPos(), camera.getAt(), camera.getUp());
    }

    mPoserFlag->isFirstCalc = false;
}

inline void CameraPoser::LocalInterpole::interpolate(sead::LookAtCamera* camera) {
    if (step > -1) {
        f32 rate = hermiteRate(normalize(step, 0, end), 1.5f, 0.0f);

        sead::Vector3f camPosNext = {0.0f, 0.0f, 0.0f};
        sead::Vector3f lookAtPosNext = {0.0f, 0.0f, 0.0f};
        lerpVec(&camPosNext, prevCameraPos, camera->getPos(), rate);
        lerpVec(&lookAtPosNext, prevLookAtPos, camera->getAt(), rate);

        camera->setPos(camPosNext);
        camera->setAt(lookAtPosNext);
    }
}

void CameraPoser::makeLookAtCameraPrev(sead::LookAtCamera* camera) const {
    camera->setPos(mPosition);
    camera->setAt(mTargetTrans);
    camera->setUp(mCameraUp);
    camera->normalizeUp();

    if (mVerticalAbsorber && !mPoserFlag->isOffVerticalAbsorb)
        mVerticalAbsorber->makeLookAtCamera(camera);

    if (mLocalInterpole)
        mLocalInterpole->interpolate(camera);

    if (mAngleSwingInfo)
        mAngleSwingInfo->makeLookAtCamera(camera);

    if (mTargetAreaLimitter) {
        sead::Vector3f camPosNext = camera->getAt();
        if (mTargetAreaLimitter->applyAreaLimit(&camPosNext, camPosNext)) {
            sead::Vector3f camDiff = camPosNext - camera->getAt();
            camera->setPos(camDiff + camera->getPos());
            camera->setAt(camDiff + camera->getAt());
        }
    }
}

void CameraPoser::makeLookAtCameraPost(sead::LookAtCamera* camera) const {
    if (alCameraPoserFunction::isSnapShotMode(this) && mSnapShotCtrl)
        mSnapShotCtrl->makeLookAtCameraPost(camera);

    if (mParamMoveLimit)
        mParamMoveLimit->apply(camera);
}

void CameraPoser::makeLookAtCameraLast(sead::LookAtCamera* camera) const {
    if (alCameraPoserFunction::isSnapShotMode(this) && mSnapShotCtrl)
        mSnapShotCtrl->makeLookAtCameraLast(camera);
}

void CameraPoser::makeLookAtCameraCollide(sead::LookAtCamera* camera) const {
    if (!mPoserFlag->isInvalidCollider && mArrowCollider)
        mArrowCollider->makeLookAtCamera(camera);
}

void CameraPoser::calcCameraPose(sead::LookAtCamera* camera) const {
    makeLookAtCameraPrev(camera);
    makeLookAtCamera(camera);
    makeLookAtCameraPost(camera);
    makeLookAtCameraCollide(camera);
    makeLookAtCameraLast(camera);
}

bool CameraPoser::receiveRequestFromObjectCore(const CameraObjectRequestInfo& info) {
    if (receiveRequestFromObject(info))
        return true;

    if (mVerticalAbsorber && info.isStopVerticalAbsorb) {
        mVerticalAbsorber->liberateAbsorb();
        return true;
    }
    if (mAngleCtrlInfo && mAngleCtrlInfo->receiveRequestFromObject(info))
        return true;

    return false;
}

void CameraPoser::startSnapShotModeCore() {
    if (mSnapShotCtrl)
        mSnapShotCtrl->start(mFovyDegree);

    startSnapShotMode();
}

void CameraPoser::endSnapShotModeCore() {
    endSnapShotMode();
}
}  // namespace al
