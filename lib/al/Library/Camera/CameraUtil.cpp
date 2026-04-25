#include "Library/Camera/CameraUtil.h"

#include "Library/Camera/ActorCameraSubTarget.h"
#include "Library/Camera/ActorCameraTarget.h"
#include "Library/Camera/CameraDirector.h"
#include "Library/Camera/CameraDistanceCurve.h"
#include "Library/Camera/CameraFlagCtrl.h"
#include "Library/Camera/CameraPoseUpdater.h"
#include "Library/Camera/CameraPoserActorRailParallel.h"
#include "Library/Camera/CameraPoserAnim.h"
#include "Library/Camera/CameraPoserFix.h"
#include "Library/Camera/CameraPoserFixActor.h"
#include "Library/Camera/CameraPoserFixLook.h"
#include "Library/Camera/CameraPoserFixPoint.h"
#include "Library/Camera/CameraPoserFlag.h"
#include "Library/Camera/CameraPoserFunction.h"
#include "Library/Camera/CameraRequestParamHolder.h"
#include "Library/Camera/CameraShaker.h"
#include "Library/Camera/CameraSubTargetBase.h"
#include "Library/Camera/CameraSwitchRequester.h"
#include "Library/Camera/CameraTargetBase.h"
#include "Library/Camera/CameraTargetHolder.h"
#include "Library/Camera/CameraTicket.h"
#include "Library/Camera/CameraViewCtrlScene.h"
#include "Library/Camera/CameraViewInfo.h"
#include "Library/Camera/ICameraInput.h"
#include "Library/Camera/IUseCamera.h"
#include "Library/Camera/SceneCameraInfo.h"
#include "Library/Camera/SimpleCameraInput.h"
#include "Library/Camera/SnapShotCameraCtrl.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorResourceFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementId.h"
#include "Library/Play/Camera/ActorMatrixCameraTarget.h"
#include "Library/Play/Camera/CameraPoserBossBattle.h"
#include "Library/Play/Camera/CameraPoserCart.h"
#include "Library/Play/Camera/CameraPoserEntrance.h"
#include "Library/Play/Camera/CameraPoserFollowSimple.h"
#include "Library/Play/Camera/CameraPoserKinopioBrigade.h"
#include "Library/Play/Camera/CameraPoserLookDown.h"
#include "Library/Play/Camera/CameraPoserParallelSimple.h"
#include "Library/Play/Camera/CameraPoserProgramable.h"
#include "Library/Play/Camera/CameraPoserQuickTurn.h"
#include "Library/Play/Camera/CameraPoserRace.h"
#include "Library/Play/Camera/CameraPoserShooterSingle.h"
#include "Library/Play/Camera/CameraPoserSubjective.h"
#include "Library/Play/Camera/CameraPoserTower.h"
#include "Library/Play/Camera/CameraVerticalAbsorber.h"
#include "Library/Projection/Projection.h"
#include "Library/Yaml/ByamlIter.h"
#include "Project/Camera/CameraAngleSwingInfo.h"
#include "Project/Camera/CameraObjectRequestInfo.h"

namespace al {

inline CameraDirector* getCameraDirector(const IUseCamera* user) {
    return user->getCameraDirector();
}

inline CameraSwitchRequester* getCameraSwitchRequester(const IUseCamera* user, s32 viewIdx) {
    return getCameraDirector(user)
        ->getSceneCameraCtrl()
        ->getSceneViewAt(viewIdx)
        ->getSwitchRequester();
}

SceneCameraInfo* getSceneCameraInfo(const IUseCamera* user) {
    return getCameraDirector(user)->getSceneCameraInfo();
}

s32 getViewNumMax(const IUseCamera* user) {
    return getViewNumMax(getSceneCameraInfo(user));
}

s32 getViewNumMax(const SceneCameraInfo* info) {
    return info->getViewNumMax();
}

bool isValidView(const IUseCamera* user, s32 viewIdx) {
    return isValidView(getSceneCameraInfo(user), viewIdx);
}

bool isValidView(const SceneCameraInfo* info, s32 viewIdx) {
    return info->getViewAt(viewIdx)->isValid();
}

const char* getViewName(const IUseCamera* user, s32 viewIdx) {
    return getViewName(getSceneCameraInfo(user), viewIdx);
}

const char* getViewName(const SceneCameraInfo* info, s32 viewIdx) {
    return info->getViewName(viewIdx);
}

const sead::Matrix34f& getViewMtx(const IUseCamera* user, s32 viewIdx) {
    return getViewMtx(getSceneCameraInfo(user), viewIdx);
}

const sead::Matrix34f& getViewMtx(const SceneCameraInfo* info, s32 viewIdx) {
    return getLookAtCamera(info, viewIdx).getMatrix();
}

const sead::Matrix34f* getViewMtxPtr(const IUseCamera* user, s32 viewIdx) {
    return getViewMtxPtr(getSceneCameraInfo(user), viewIdx);
}

const sead::Matrix34f* getViewMtxPtr(const SceneCameraInfo* info, s32 viewIdx) {
    return &getViewMtx(info, viewIdx);
}

const sead::Matrix44f& getProjectionMtx(const IUseCamera* user, s32 viewIdx) {
    return getProjectionMtx(getSceneCameraInfo(user), viewIdx);
}

const sead::Matrix44f& getProjectionMtx(const SceneCameraInfo* info, s32 viewIdx) {
    return *info->getViewAt(viewIdx)->getProjMtx();
}

const sead::Matrix44f* getProjectionMtxPtr(const IUseCamera* user, s32 viewIdx) {
    return getProjectionMtxPtr(getSceneCameraInfo(user), viewIdx);
}

const sead::Matrix44f* getProjectionMtxPtr(const SceneCameraInfo* info, s32 viewIdx) {
    return info->getViewAt(viewIdx)->getProjMtx();
}

const sead::LookAtCamera& getLookAtCamera(const IUseCamera* user, s32 viewIdx) {
    return getLookAtCamera(getSceneCameraInfo(user), viewIdx);
}

const sead::LookAtCamera& getLookAtCamera(const SceneCameraInfo* info, s32 viewIdx) {
    return info->getViewAt(viewIdx)->getLookAtCam();
}

const sead::Projection& getProjectionSead(const IUseCamera* user, s32 viewIdx) {
    return getProjectionSead(getSceneCameraInfo(user), viewIdx);
}

const sead::Projection& getProjectionSead(const SceneCameraInfo* info, s32 viewIdx) {
    return info->getViewAt(viewIdx)->getProjectionSead();
}

const Projection& getProjection(const IUseCamera* user, s32 viewIdx) {
    return getProjection(getSceneCameraInfo(user), viewIdx);
}

const Projection& getProjection(const SceneCameraInfo* info, s32 viewIdx) {
    return info->getViewAt(viewIdx)->getProjection();
}

const sead::Vector3f& getCameraPos(const IUseCamera* user, s32 viewIdx) {
    return getCameraPos(getSceneCameraInfo(user), viewIdx);
}

const sead::Vector3f& getCameraPos(const SceneCameraInfo* info, s32 viewIdx) {
    return getLookAtCamera(info, viewIdx).getPos();
}

const sead::Vector3f& getCameraAt(const IUseCamera* user, s32 viewIdx) {
    return getCameraAt(getSceneCameraInfo(user), viewIdx);
}

const sead::Vector3f& getCameraAt(const SceneCameraInfo* info, s32 viewIdx) {
    return getLookAtCamera(info, viewIdx).getAt();
}

const sead::Vector3f& getCameraUp(const IUseCamera* user, s32 viewIdx) {
    return getCameraUp(getSceneCameraInfo(user), viewIdx);
}

const sead::Vector3f& getCameraUp(const SceneCameraInfo* info, s32 viewIdx) {
    return getLookAtCamera(info, viewIdx).getUp();
}

f32 getFovyDegree(const IUseCamera* user, s32 viewIdx) {
    return getFovyDegree(getSceneCameraInfo(user), viewIdx);
}

f32 getFovyDegree(const SceneCameraInfo* info, s32 viewIdx) {
    return sead::Mathf::rad2deg(getFovy(info, viewIdx));
}

f32 getFovy(const IUseCamera* user, s32 viewIdx) {
    return getFovy(getSceneCameraInfo(user), viewIdx);
}

f32 getFovy(const SceneCameraInfo* info, s32 viewIdx) {
    return info->getViewAt(viewIdx)->getProjection().getFovy();
}

f32 getNear(const IUseCamera* user, s32 viewIdx) {
    return getNear(getSceneCameraInfo(user), viewIdx);
}

f32 getNear(const SceneCameraInfo* info, s32 viewIdx) {
    return info->getViewAt(viewIdx)->getNear();
}

f32 getFar(const IUseCamera* user, s32 viewIdx) {
    return getFar(getSceneCameraInfo(user), viewIdx);
}

f32 getFar(const SceneCameraInfo* info, s32 viewIdx) {
    return info->getViewAt(viewIdx)->getFar();
}

f32 calcCameraDistance(const IUseCamera* user, s32 viewIdx) {
    sead::Vector3f diff = getCameraPos(user, viewIdx) - getCameraAt(user, viewIdx);

    return diff.length();
}

f32 calcFovxDegree(const IUseCamera* user, s32 viewIdx) {
    f32 aspect = getSceneCameraInfo(user)->getViewAt(viewIdx)->getAspect();

    return aspect * getFovyDegree(user, viewIdx);
}

f32 calcCurrentFovyRate(const IUseCamera* user, s32 viewIdx) {
    f32 fovy = getFovyDegree(user, viewIdx);
    f32 fovy2 = getCameraDirector(user)->getSceneFovyDegree();

    if (isNearZero(fovy) || isNearZero(fovy2))
        return 0;

    return fovy / fovy2;
}

void calcCameraFront(sead::Vector3f* front, const IUseCamera* user, s32 viewIdx) {
    const sead::Vector3f& at = getCameraAt(user, viewIdx);
    const sead::Vector3f& pos = getCameraPos(user, viewIdx);
    front->set(at - pos);
    normalize(front);
}

void setNearClipDistance(const IUseCamera* user, f32 distance, s32 updaterIdx) {
    getCameraDirector(user)->getPoseUpdater(updaterIdx)->setNearClipDistance(distance);
}

void setFarClipDistance(const IUseCamera* user, f32 distance, s32 updaterIdx) {
    getCameraDirector(user)->getPoseUpdater(updaterIdx)->setFarClipDistance(distance);
}

void setCurrentCameraPose(CameraPoseInfo* poseInfo, const IUseCamera* user) {
    const sead::Vector3f& cameraPos = getCameraPos(user, 0);
    poseInfo->cameraPos.set(cameraPos);

    const sead::Vector3f& lookAtPos = getCameraAt(user, 0);
    poseInfo->lookAtPos.set(lookAtPos);

    const sead::Vector3f& cameraUp = getCameraUp(user, 0);
    poseInfo->cameraUp.set(cameraUp);
}

void calcCameraDir(sead::Vector3f* dir, const IUseCamera* user, s32 viewIdx) {
    const sead::Matrix34f& cameraMtx = getLookAtCamera(user, viewIdx).getMatrix();
    dir->set(cameraMtx.m[2][0], cameraMtx.m[2][1], cameraMtx.m[2][2]);
}

void calcCameraLookDir(sead::Vector3f* lookDir, const IUseCamera* user, s32 viewIdx) {
    const sead::Matrix34f& cameraMtx = getLookAtCamera(user, viewIdx).getMatrix();
    lookDir->set(-cameraMtx.m[2][0], -cameraMtx.m[2][1], cameraMtx.m[2][2]);
    lookDir->z = -lookDir->z;
}

void calcCameraSideDir(sead::Vector3f* sideDir, const IUseCamera* user, s32 viewIdx) {
    getLookAtCamera(user, viewIdx).getRightVectorByMatrix(sideDir);
}

void calcCameraUpDir(sead::Vector3f* upDir, const IUseCamera* user, s32 viewIdx) {
    getLookAtCamera(user, viewIdx).getUpVectorByMatrix(upDir);
}

bool tryCalcCameraDir(sead::Vector3f* dir, const SceneCameraInfo* info, s32 viewIdx) {
    const sead::Matrix34f& cameraMtx = getLookAtCamera(info, viewIdx).getMatrix();
    dir->set(cameraMtx.m[2][0], cameraMtx.m[2][1], cameraMtx.m[2][2]);
    return tryNormalizeOrZero(dir);
}

bool tryCalcCameraDirH(sead::Vector3f* dirH, const SceneCameraInfo* info,
                       const sead::Vector3f& upDir, s32 viewIdx) {
    const sead::Matrix34f& cameraMtx = getLookAtCamera(info, viewIdx).getMatrix();
    sead::Vector3f dir(cameraMtx.m[2][0], cameraMtx.m[2][1], cameraMtx.m[2][2]);
    verticalizeVec(dirH, upDir, dir);
    return tryNormalizeOrZero(dirH);
}

bool tryCalcCameraLookDirH(sead::Vector3f* lookDirH, const SceneCameraInfo* info,
                           const sead::Vector3f& upDir, s32 viewIdx) {
    const sead::Matrix34f& cameraMtx = getLookAtCamera(info, viewIdx).getMatrix();
    sead::Vector3f dir(cameraMtx.m[2][0], cameraMtx.m[2][1], cameraMtx.m[2][2]);
    verticalizeVec(lookDirH, upDir, dir);

    if (!tryNormalizeOrZero(lookDirH))
        return false;

    lookDirH->negate();
    return true;
}

void startCamera(const IUseCamera* user, CameraTicket* ticket, s32 updaterIdx) {
    getCameraSwitchRequester(user, 0)->requestStart(ticket, updaterIdx);
}

void startCameraSub(const IUseCamera* user, CameraTicket* ticket, s32 updaterIdx) {
    getCameraSwitchRequester(user, 1)->requestStart(ticket, updaterIdx);
}

void startAnimCamera(const IUseCamera* user, CameraTicket* ticket, const char* animName,
                     s32 updaterIdx) {
    ticket->getAnimPoser()->setAnim(animName, -1, -1, -1);
    getCameraSwitchRequester(user, 0)->requestStart(ticket, updaterIdx);
}

void startAnimCameraAnim(CameraTicket* ticket, const char* animName, s32 startStep, s32 endStep,
                         s32 playStep) {
    ticket->getAnimPoser()->setAnim(animName, startStep, endStep, playStep);
}

void startAnimCameraWithStartStepAndEndStepAndPlayStep(const IUseCamera* user, CameraTicket* ticket,
                                                       const char* animName, s32 startStep,
                                                       s32 endStep, s32 playStep, s32 updaterIdx) {
    ticket->getAnimPoser()->setAnim(animName, startStep, endStep, playStep);
    getCameraSwitchRequester(user, 0)->requestStart(ticket, updaterIdx);
}

void endCamera(const IUseCamera* user, CameraTicket* ticket, s32 updaterIdx, bool isCollisionOff) {
    getCameraSwitchRequester(user, 0)->requestEnd(ticket, updaterIdx, isCollisionOff);
}

void endCameraWithNextCameraPose(const IUseCamera* user, CameraTicket* ticket,
                                 const CameraPoseInfo* poseInfo, s32 updaterIdx) {
    getCameraSwitchRequester(user, 0)->requestEndWithNextCameraPose(ticket, poseInfo, updaterIdx);
}

void endCameraSub(const IUseCamera* user, CameraTicket* ticket, s32 updaterIdx) {
    getCameraSwitchRequester(user, 1)->requestEnd(ticket, updaterIdx, false);
}

bool isActiveCamera(const CameraTicket* ticket) {
    return ticket->isActiveCamera();
}

bool isActiveCameraInterpole(const IUseCamera* user, s32 updaterIdx) {
    return getCameraDirector(user)->getPoseUpdater(updaterIdx)->isActiveInterpole();
}

bool isActiveCameraInterpole(const SceneCameraInfo* info, s32 viewIdx) {
    return info->getViewAt(viewIdx)->isActiveInterpole();
}

void startCameraInterpole(const IUseCamera* user, s32 updaterIdx, s32 step) {
    getCameraDirector(user)->getPoseUpdater(updaterIdx)->startInterpole(step);
}

void requestCancelCameraInterpole(const IUseCamera* user, s32 updaterIdx) {
    getCameraDirector(user)->getPoseUpdater(updaterIdx)->requestCancelInterpole();
}

bool tryCalcCameraPoseWithoutInterpole(sead::LookAtCamera* camera, const IUseCamera* user,
                                       s32 updaterIdx) {
    return getCameraDirector(user)
        ->getPoseUpdater(updaterIdx)
        ->calcCameraPoseWithoutInterpole(camera);
}

void invalidateCameraPoserVerticalAbsorber(CameraTicket* ticket) {
    CameraVerticalAbsorber* absorber = ticket->getPoser()->getVerticalAbsorber();
    if (absorber)
        absorber->invalidate();
}

void requestStopCameraVerticalAbsorb(IUseCamera* user) {
    CameraObjectRequestInfo info;
    info.angleV = 23.0f;
    info.isUpToTargetAngleBySpeed = false;
    info.isStopVerticalAbsorb = false;
    info.isResetPosition = false;
    info.isResetAngleV = false;
    info.isDownToDefaultAngleBySpeed = false;
    info.isMoveDownAngle = false;
    info.isSetAngleV = false;
    info.targetAngleV = 0.0f;
    info.angleSpeed = 0.0f;
    info.isStopVerticalAbsorb = true;
    getCameraDirector(user)->getPoseUpdater(0)->tryReceiveCameraRequestFromObject(info);
}

void validateSnapShotCameraZoomFovy(CameraTicket* ticket) {
    ticket->getPoser()->getSnapShotCtrl()->validateZoomFovy();
}

void validateSnapShotCameraRoll(CameraTicket* ticket) {
    ticket->getPoser()->getSnapShotCtrl()->validateRoll();
}

bool isSnapShotOrientationRotate90(const IUseCamera* user) {
    return getCameraDirector(user)->getPoseUpdater(0)->isSnapShotOrientationRotate90();
}

bool isSnapShotOrientationRotate270(const IUseCamera* user) {
    return getCameraDirector(user)->getPoseUpdater(0)->isSnapShotOrientationRotate270();
}

bool isValidCameraGyro(const IUseCamera* user) {
    return !getCameraDirector(user)->getFlagCtrl()->isInvalidCameraGyro;
}

bool isInvalidChangeSubjectiveCamera(const IUseCamera* user) {
    return getCameraDirector(user)->getPoseUpdater(0)->isInvalidChangeSubjectiveCamera();
}

bool isCurrentCameraZooming(const IUseCamera* user, s32 updaterIdx) {
    return getCameraDirector(user)->getPoseUpdater(updaterIdx)->isCurrentCameraZooming();
}

bool isExistAnimCameraData(const CameraTicket* ticket, const char* animName) {
    return ticket->getAnimPoser()->isExistAnim(animName);
}

bool isEndAnimCamera(const CameraTicket* ticket) {
    return ticket->getAnimPoser()->isAnimEnd();
}

bool isAnimCameraPlaying(const CameraTicket* ticket) {
    return ticket->getAnimPoser()->isPlaying();
}

bool isAnimCameraAnimPlaying(const CameraTicket* ticket, const char* animName) {
    return ticket->getAnimPoser()->isAnimPlaying(animName);
}

s32 getAnimCameraStepMax(const CameraTicket* ticket) {
    return ticket->getAnimPoser()->getStepMax();
}

s32 getAnimCameraStep(const CameraTicket* ticket) {
    return ticket->getAnimPoser()->getStep();
}

s32 calcAnimCameraAnimStepMax(const CameraTicket* ticket, const char* animName) {
    return ticket->getAnimPoser()->calcStepMax(animName);
}

void setAnimCameraRotateBaseUp(const CameraTicket* ticket) {
    ticket->getAnimPoser()->setRotateBaseUp();
}

void setQuickTurnCameraFollow(CameraTicket* ticket) {
    ticket->getQuickTurnPoser()->setFollow();
}

void setQuickTurnCameraRotateFast(CameraTicket* ticket) {
    ticket->getQuickTurnPoser()->setRotateFast();
}

void setRaceCameraFrontDirPtr(const CameraTicket* ticket, const sead::Vector3f* frontDirPtr) {
    ticket->getRacePoser()->setFrontDirPtr(frontDirPtr);
}

void setRaceCameraDistance(const CameraTicket* ticket, f32 distance) {
    ticket->getRacePoser()->setDistance(distance);
}

void setRaceCameraOffsetY(const CameraTicket* ticket, f32 offsetY) {
    ticket->getRacePoser()->setOffsetY(offsetY);
}

void setRaceCameraAngleDegreeV(const CameraTicket* ticket, f32 angleV) {
    ticket->getRacePoser()->setAngleDegreeV(angleV);
}

void stopCartCamera(const CameraTicket* ticket) {
    ticket->getCartPoser()->stop();
}

void restartCartCamera(const CameraTicket* ticket) {
    ticket->getCartPoser()->restart();
}

void setEntranceCameraParam(CameraTicket* ticket, f32 distance, const sead::Vector3f& cameraPos,
                            const sead::Vector3f& lookAtPos) {
    ticket->getEntrancePoser()->initParam(distance, cameraPos, lookAtPos);
}

void setEntranceCameraLookAt(CameraTicket* ticket, const sead::Vector3f& lookAt) {
    ticket->getEntrancePoser()->initLookAtPosDirect(lookAt);
}

void invalidateEndEntranceCameraWithName(IUseCamera* user, const char*) {
    getCameraDirector(user)->getFlagCtrl()->invalidateEndEntranceCamera();
}

void validateEndEntranceCamera(IUseCamera* user) {
    getCameraDirector(user)->getFlagCtrl()->validateEndEntranceCamera();
}

bool isPlayingEntranceCamera(const IUseCamera* user, s32 updaterIdx) {
    if (getCameraDirector(user)->getPoseUpdater(updaterIdx)->isCurrentCameraPriority(2))
        return true;

    return getCameraDirector(user)->getPoseUpdater(updaterIdx)->isCurrentCameraPriority(7);
}

void setCameraInterpoleStep(CameraTicket* ticket, s32 step) {
    ticket->getPoser()->setInterpoleStep(step);
}

void setCameraFovyDegree(CameraTicket* ticket, f32 fovy) {
    ticket->getPoser()->setFovyDegree(fovy);
}

SimpleCameraInput* createSimpleCameraInput(s32 port) {
    return new SimpleCameraInput(port);
}

void setCameraInput(IUseCamera* user, const ICameraInput* cameraInput) {
    getCameraDirector(user)->setCameraInput(cameraInput);
}

void setViewCameraInput(IUseCamera* user, const ICameraInput* cameraInput, s32 inputIdx) {
    getCameraDirector(user)->setViewCameraInput(cameraInput, inputIdx);
}

bool isExistCameraInputAtDisableTiming(const IUseCamera* user, s32 inputIdx) {
    if (getCameraDirector(user)->getPoseUpdater(0)->isCurrentCameraEnableRotateByPad())
        return false;

    sead::Vector2f inputStick = {0.0f, 0.0f};
    getCameraDirector(user)->getCameraInput(inputIdx)->calcInputStick(&inputStick);
    return !isNearZero(inputStick, 0.001f);
}

ActorCameraTarget* createActorCameraTarget(const LiveActor* actor, f32 distance) {
    return new ActorCameraTarget(actor, distance, nullptr);
}

ActorCameraTarget* createActorCameraTarget(const LiveActor* actor, const sead::Vector3f* offset) {
    return new ActorCameraTarget(actor, 0.0f, offset);
}

ActorMatrixCameraTarget* createActorJointCameraTarget(const LiveActor* actor,
                                                      const char* jointName) {
    return new ActorMatrixCameraTarget(actor, getJointMtxPtr(actor, jointName));
}

ActorMatrixCameraTarget* createActorMatrixCameraTarget(const LiveActor* actor,
                                                       const sead::Matrix34f* matrix) {
    return new ActorMatrixCameraTarget(actor, matrix);
}

bool isActiveCameraTarget(const CameraTargetBase* target) {
    return target->isActiveTarget();
}

void setCameraTarget(IUseCamera* user, CameraTargetBase* target) {
    getCameraDirector(user)->getTargetHolder()->addTarget(target);
}

void resetCameraTarget(IUseCamera* user, CameraTargetBase* target) {
    getCameraDirector(user)->getTargetHolder()->removeTarget(target);
}

ActorCameraSubTarget* createActorCameraSubTarget(const LiveActor* actor,
                                                 const sead::Vector3f* offset) {
    ActorCameraSubTarget* target = new ActorCameraSubTarget(actor);
    if (offset)
        target->setOffset(offset);
    return target;
}

ActorBackAroundCameraSubTarget* createActorBackAroundCameraSubTarget(const LiveActor* actor,
                                                                     const sead::Vector3f* offset) {
    ActorBackAroundCameraSubTarget* target = new ActorBackAroundCameraSubTarget(actor);
    if (offset)
        target->setOffset(offset);
    return target;
}

TransCameraSubTarget* createTransCameraSubTarget(const char* targetName,
                                                 const sead::Vector3f* trans) {
    return new TransCameraSubTarget(targetName, trans);
}

void initCameraSubTargetTurnParam(CameraSubTargetBase* target,
                                  const CameraSubTargetTurnParam* turnParam) {
    target->initTurnParam(turnParam);
}

bool isActiveCameraSubTarget(const CameraSubTargetBase* target) {
    return target->isActiveTarget();
}

void setCameraSubTarget(IUseCamera* user, CameraSubTargetBase* target) {
    getCameraDirector(user)->getTargetHolder()->addSubTarget(target);
}

void resetCameraSubTarget(IUseCamera* user, CameraSubTargetBase* target) {
    getCameraDirector(user)->getTargetHolder()->removeSubTarget(target);
}

void setCameraPlacementSubTarget(IUseCamera* user, CameraSubTargetBase* target) {
    getCameraDirector(user)->getTargetHolder()->addPlacementSubTarget(target);
}

void resetCameraPlacementSubTarget(IUseCamera* user, CameraSubTargetBase* target) {
    getCameraDirector(user)->getTargetHolder()->removePlacementSubTarget(target);
}

const CameraDistanceCurve* getCameraDistanceRocketFlowerCurve() {
    return CameraDistanceCurve::getRocketFlowerCurve();
}

void setViewCameraTarget(IUseCamera* user, CameraTargetBase* target, s32 viewIdx) {
    getCameraDirector(user)->getTargetHolder()->setViewTarget(target, viewIdx);
}

void startCameraShakeByAction(const LiveActor* actor, const char* shakeName, const char* actionName,
                              s32 step, s32 viewIdx) {
    const IUseCamera* user = actor;
    CameraDirector* director = getCameraDirector(user);
    if (viewIdx >= 0) {
        director->getPoseUpdater(viewIdx)->getShaker()->startShakeByAction(
            shakeName, actor->getName(), actionName, step);
        return;
    }

    s32 viewNum = director->getSceneCameraInfo()->getViewNumMax();
    if (viewNum >= 1) {
        for (s32 i = 0; i != viewNum; i++) {
            getCameraDirector(user)->getPoseUpdater(i)->getShaker()->startShakeByAction(
                shakeName, actor->getName(), actionName, step);
        }
    }
}

void startCameraShakeByHitReaction(const IUseCamera* user, const char* name, const char* unused1,
                                   const char* unused2, s32 step, s32 viewIdx) {
    CameraDirector* director = getCameraDirector(user);
    if (viewIdx >= 0) {
        director->getPoseUpdater(viewIdx)->getShaker()->startShakeByHitReaction(name, unused1,
                                                                                unused2, step);
        return;
    }

    s32 viewNum = director->getSceneCameraInfo()->getViewNumMax();
    if (viewNum >= 1) {
        for (s32 i = 0; i != viewNum; i++) {
            getCameraDirector(user)->getPoseUpdater(i)->getShaker()->startShakeByHitReaction(
                name, unused1, unused2, step);
        }
    }
}

void requestCameraLoopShakeWeak(const IUseCamera* user) {
    getCameraDirector(user)->getSceneCameraCtrl()->getSceneViewAt(0)->setCameraShakeLoopName("弱");
}

const char* TransCameraSubTarget::getTargetName() const {
    return mTargetName;
}

void TransCameraSubTarget::calcTrans(sead::Vector3f* trans) const {
    trans->set(*mTrans);
}

CameraTicket* initObjectCamera(const IUseCamera* user, const PlacementInfo& placementInfo,
                               const char* cameraName, const char* suffix) {
    sead::Matrix34f zoneMtx = sead::Matrix34f::ident;
    tryGetZoneMatrixTR(&zoneMtx, placementInfo);
    return getCameraDirector(user)->createObjectCamera(createPlacementId(placementInfo), cameraName,
                                                       suffix, CameraTicket::Priority_Object,
                                                       zoneMtx);
}

CameraTicket* initObjectCamera(const IUseCamera* user, const ActorInitInfo& actorInitInfo,
                               const char* cameraName, const char* suffix) {
    const PlacementInfo& placementInfo = getPlacementInfo(actorInitInfo);
    sead::Matrix34f zoneMtx = sead::Matrix34f::ident;
    tryGetZoneMatrixTR(&zoneMtx, placementInfo);
    return getCameraDirector(user)->createObjectCamera(createPlacementId(placementInfo), cameraName,
                                                       suffix, CameraTicket::Priority_Object,
                                                       zoneMtx);
}

CameraTicket* initObjectCameraNoPlacementInfo(const IUseCamera* user, const char* cameraName,
                                              const char* suffix) {
    return getCameraDirector(user)->createObjectCamera(
        nullptr, cameraName, suffix, CameraTicket::Priority_Object, sead::Matrix34f::ident);
}

CameraTicket* initFixCamera(const IUseCamera* user, const char* cameraName,
                            const sead::Vector3f& cameraPos, const sead::Vector3f& lookAtPos) {
    CameraPoserFix* poser = new CameraPoserFix("固定");
    CameraTicket* ticket = getCameraDirector(user)->createCamera(
        poser, nullptr, cameraName, CameraTicket::Priority_Object, sead::Matrix34f::ident, true);
    poser->initCameraPosAndLookAtPos(cameraPos, lookAtPos);
    return ticket;
}

CameraTicket* initFixDoorwayCamera(const IUseCamera* user, const char* cameraName,
                                   const sead::Vector3f& cameraPos,
                                   const sead::Vector3f& lookAtPos) {
    CameraPoserFix* poser = new CameraPoserFix(CameraPoserFix::getFixDoorwayCameraName());
    CameraTicket* ticket = getCameraDirector(user)->createCamera(
        poser, nullptr, cameraName, CameraTicket::Priority_Object, sead::Matrix34f::ident, true);
    poser->initCameraPosAndLookAtPos(cameraPos, lookAtPos);
    return ticket;
}

CameraTicket* initFixActorCamera(const LiveActor* actor, const ActorInitInfo& actorInitInfo,
                                 const char* cameraName, const sead::Vector3f& offset, f32 distance,
                                 f32 angleH, f32 angleV, bool isCalcNearestAtFromPreAt) {
    CameraPoserFixActor* poser = new CameraPoserFixActor(actor);
    poser->setOffset(offset);
    poser->setDistance(distance);
    poser->setAngleH(angleH);
    poser->setAngleV(angleV);
    if (isCalcNearestAtFromPreAt)
        poser->setCalcNearestAtFromPreAt();

    const IUseCamera* user = actor;
    const PlacementInfo& placementInfo = getPlacementInfo(actorInitInfo);
    sead::Matrix34f zoneMtx = sead::Matrix34f::ident;
    tryGetZoneMatrixTR(&zoneMtx, placementInfo);

    PlacementId placementId;
    const PlacementId* placementIdPtr = nullptr;
    if (tryGetPlacementId(&placementId, placementInfo))
        placementIdPtr = createPlacementId(placementInfo);

    return getCameraDirector(user)->createCamera(poser, placementIdPtr, cameraName,
                                                 CameraTicket::Priority_Object, zoneMtx, true);
}

CameraTicket* initFixLookCamera(LiveActor* actor, const ActorInitInfo& actorInitInfo,
                                const char* cameraName) {
    CameraPoserFixLook* poser = new CameraPoserFixLook();
    poser->setTransPtr(getTransPtr(actor));

    const IUseCamera* user = actor;
    const PlacementInfo& placementInfo = getPlacementInfo(actorInitInfo);
    sead::Matrix34f zoneMtx = sead::Matrix34f::ident;
    tryGetZoneMatrixTR(&zoneMtx, placementInfo);

    PlacementId placementId;
    const PlacementId* placementIdPtr = nullptr;
    if (tryGetPlacementId(&placementId, placementInfo))
        placementIdPtr = createPlacementId(placementInfo);

    return getCameraDirector(user)->createCamera(poser, placementIdPtr, cameraName,
                                                 CameraTicket::Priority_Object, zoneMtx, true);
}

CameraTicket* initFixTalkCamera(const LiveActor* actor, const ActorInitInfo& actorInitInfo,
                                const char* cameraName, const sead::Vector3f& offset, f32 distance,
                                f32 talkDistance, f32 angleV, bool isAutoAroundFront) {
    CameraPoserFixTalk* poser = new CameraPoserFixTalk(actor);
    poser->setOffset(offset);
    poser->setDistance(distance);
    poser->setTalkDistance(talkDistance);
    poser->setAngleV(angleV);
    if (isAutoAroundFront)
        poser->setAutoAroundFront();

    const IUseCamera* user = actor;
    const PlacementInfo& placementInfo = getPlacementInfo(actorInitInfo);
    sead::Matrix34f zoneMtx = sead::Matrix34f::ident;
    tryGetZoneMatrixTR(&zoneMtx, placementInfo);

    PlacementId placementId;
    const PlacementId* placementIdPtr = nullptr;
    if (tryGetPlacementId(&placementId, placementInfo))
        placementIdPtr = createPlacementId(placementInfo);

    return getCameraDirector(user)->createCamera(poser, placementIdPtr, cameraName,
                                                 CameraTicket::Priority_Object, zoneMtx, true);
}

CameraTicket* initFixFishingCamera(const LiveActor* actor, const ActorInitInfo& actorInitInfo,
                                   const char* cameraName, const sead::Vector3f& cameraPos,
                                   const sead::Vector3f& lookAtPos, f32 distance, f32 paramDistance,
                                   f32 angleV, bool isAutoAroundFront) {
    CameraPoserFixFishing* poser = new CameraPoserFixFishing(actor);
    poser->setDistance(distance);
    poser->setAngleV(angleV);
    poser->initParam(paramDistance, cameraPos, lookAtPos);
    if (isAutoAroundFront)
        poser->setAutoAroundFront();

    const IUseCamera* user = actor;
    const PlacementInfo& placementInfo = getPlacementInfo(actorInitInfo);
    sead::Matrix34f zoneMtx = sead::Matrix34f::ident;
    tryGetZoneMatrixTR(&zoneMtx, placementInfo);

    PlacementId placementId;
    const PlacementId* placementIdPtr = nullptr;
    if (tryGetPlacementId(&placementId, placementInfo))
        placementIdPtr = createPlacementId(placementInfo);

    return getCameraDirector(user)->createCamera(poser, placementIdPtr, cameraName,
                                                 CameraTicket::Priority_Object, zoneMtx, true);
}

CameraTicket* initFixPointCamera(const IUseCamera* user, const ActorInitInfo& actorInitInfo,
                                 const char* cameraName, bool isUsePreCameraPos) {
    CameraPoserFixPoint* poser = new CameraPoserFixPoint("定点");
    if (isUsePreCameraPos)
        poser->validateUsePreCameraPos();

    const PlacementInfo& placementInfo = getPlacementInfo(actorInitInfo);
    sead::Matrix34f zoneMtx = sead::Matrix34f::ident;
    tryGetZoneMatrixTR(&zoneMtx, placementInfo);

    PlacementId placementId;
    const PlacementId* placementIdPtr = nullptr;
    if (tryGetPlacementId(&placementId, placementInfo))
        placementIdPtr = createPlacementId(placementInfo);

    return getCameraDirector(user)->createCamera(poser, placementIdPtr, cameraName,
                                                 CameraTicket::Priority_Object, zoneMtx, true);
}

CameraTicket* initLookDownCamera(const LiveActor* actor, const ActorInitInfo& actorInitInfo,
                                 const char* cameraName) {
    CameraPoserLookDown* poser = new CameraPoserLookDown("見下ろし");

    const IUseCamera* user = actor;
    const PlacementInfo& placementInfo = getPlacementInfo(actorInitInfo);
    sead::Matrix34f zoneMtx = sead::Matrix34f::ident;
    tryGetZoneMatrixTR(&zoneMtx, placementInfo);

    PlacementId placementId;
    const PlacementId* placementIdPtr = nullptr;
    if (tryGetPlacementId(&placementId, placementInfo))
        placementIdPtr = createPlacementId(placementInfo);

    return getCameraDirector(user)->createCamera(poser, placementIdPtr, cameraName,
                                                 CameraTicket::Priority_Object, zoneMtx, true);
}

CameraTicket* initProgramableCamera(const IUseCamera* user, const ActorInitInfo& actorInitInfo,
                                    const char* cameraName, const sead::Vector3f* pos,
                                    const sead::Vector3f* at, const sead::Vector3f* up) {
    CameraPoserProgramable* poser = new CameraPoserProgramable(pos, at, up);

    const PlacementInfo& placementInfo = getPlacementInfo(actorInitInfo);
    sead::Matrix34f zoneMtx = sead::Matrix34f::ident;
    tryGetZoneMatrixTR(&zoneMtx, placementInfo);

    PlacementId placementId;
    const PlacementId* placementIdPtr = nullptr;
    if (tryGetPlacementId(&placementId, placementInfo))
        placementIdPtr = createPlacementId(placementInfo);

    return getCameraDirector(user)->createCamera(poser, placementIdPtr, cameraName,
                                                 CameraTicket::Priority_Object, zoneMtx, true);
}

CameraTicket* initProgramableCamera(const IUseCamera* user, const char* cameraName,
                                    const sead::Vector3f* pos, const sead::Vector3f* at,
                                    const sead::Vector3f* up) {
    CameraPoserProgramable* poser = new CameraPoserProgramable(pos, at, up);
    return getCameraDirector(user)->createCamera(
        poser, nullptr, cameraName, CameraTicket::Priority_Object, sead::Matrix34f::ident, true);
}

CameraTicket* initProgramableCameraWithCollider(const IUseCamera* user,
                                                const ActorInitInfo& actorInitInfo,
                                                const char* cameraName, const sead::Vector3f* pos,
                                                const sead::Vector3f* at,
                                                const sead::Vector3f* up) {
    CameraPoserProgramable* poser = new CameraPoserProgramable(pos, at, up);

    const PlacementInfo& placementInfo = getPlacementInfo(actorInitInfo);
    sead::Matrix34f zoneMtx = sead::Matrix34f::ident;
    tryGetZoneMatrixTR(&zoneMtx, placementInfo);

    PlacementId placementId;
    const PlacementId* placementIdPtr = nullptr;
    if (tryGetPlacementId(&placementId, placementInfo))
        placementIdPtr = createPlacementId(placementInfo);

    CameraTicket* ticket = getCameraDirector(user)->createCamera(
        poser, placementIdPtr, cameraName, CameraTicket::Priority_Object, zoneMtx, true);
    alCameraPoserFunction::initCameraArrowCollider(poser);
    return ticket;
}

CameraTicket* initProgramableAngleCamera(const IUseCamera* user, const PlacementInfo& placementInfo,
                                         const char* cameraName, const sead::Vector3f* pos,
                                         const f32* angleH, const f32* angleV,
                                         const f32* distance) {
    CameraPoserProgramableAngle* poser =
        new CameraPoserProgramableAngle(pos, angleH, angleV, distance);

    sead::Matrix34f zoneMtx = sead::Matrix34f::ident;
    tryGetZoneMatrixTR(&zoneMtx, placementInfo);

    PlacementId placementId;
    const PlacementId* placementIdPtr = nullptr;
    if (tryGetPlacementId(&placementId, placementInfo))
        placementIdPtr = createPlacementId(placementInfo);

    return getCameraDirector(user)->createCamera(poser, placementIdPtr, cameraName,
                                                 CameraTicket::Priority_Object, zoneMtx, true);
}

CameraTicket* initProgramableCameraKeepColliderPreCamera(
    const IUseCamera* user, const ActorInitInfo& actorInitInfo, const char* cameraName,
    const sead::Vector3f* pos, const sead::Vector3f* at, const sead::Vector3f* up) {
    CameraPoserProgramableKeepColliderPreCamera* poser =
        new CameraPoserProgramableKeepColliderPreCamera(pos, at, up);

    const PlacementInfo& placementInfo = getPlacementInfo(actorInitInfo);
    sead::Matrix34f zoneMtx = sead::Matrix34f::ident;
    tryGetZoneMatrixTR(&zoneMtx, placementInfo);

    PlacementId placementId;
    const PlacementId* placementIdPtr = nullptr;
    if (tryGetPlacementId(&placementId, placementInfo))
        placementIdPtr = createPlacementId(placementInfo);

    return getCameraDirector(user)->createCamera(poser, placementIdPtr, cameraName,
                                                 CameraTicket::Priority_Object, zoneMtx, true);
}

CameraTicket* initShooterCameraSingle(const IUseCamera* user, const char* cameraName) {
    CameraPoserShooterSingle* poser = new CameraPoserShooterSingle("シングルシュータカメラ");
    return getCameraDirector(user)->createCamera(
        poser, nullptr, cameraName, CameraTicket::Priority_Object, sead::Matrix34f::ident, true);
}

CameraTicket* initTowerCameraWithSave(const IUseCamera* user, const sead::Vector3f* axis,
                                      const ActorInitInfo& actorInitInfo, const char* cameraName) {
    CameraPoserTower* poser = new CameraPoserTower("塔", axis);

    const PlacementInfo& placementInfo = getPlacementInfo(actorInitInfo);
    sead::Matrix34f zoneMtx = sead::Matrix34f::ident;
    tryGetZoneMatrixTR(&zoneMtx, placementInfo);

    PlacementId placementId;
    const PlacementId* placementIdPtr = nullptr;
    if (tryGetPlacementId(&placementId, placementInfo))
        placementIdPtr = createPlacementId(placementInfo);

    return getCameraDirector(user)->createCamera(poser, placementIdPtr, cameraName,
                                                 CameraTicket::Priority_Object, zoneMtx, true);
}

CameraTicket* initTowerCamera(const IUseCamera* user, const sead::Vector3f* axis,
                              const ActorInitInfo& actorInitInfo, const char* cameraName) {
    CameraPoserTower* poser = new CameraPoserTower("塔", axis);

    const PlacementInfo& placementInfo = getPlacementInfo(actorInitInfo);
    sead::Matrix34f zoneMtx = sead::Matrix34f::ident;
    tryGetZoneMatrixTR(&zoneMtx, placementInfo);

    PlacementId placementId;
    const PlacementId* placementIdPtr = nullptr;
    if (tryGetPlacementId(&placementId, placementInfo))
        placementIdPtr = createPlacementId(placementInfo);

    return getCameraDirector(user)->createCamera(poser, placementIdPtr, cameraName,
                                                 CameraTicket::Priority_Object, zoneMtx, true);
}

CameraTicket* initBossBattleCamera(const IUseCamera* user, const sead::Vector3f* axis,
                                   const ActorInitInfo& actorInitInfo, const char* cameraName) {
    CameraPoserBossBattle* poser = new CameraPoserBossBattle("ボス戦カメラ", axis);

    const PlacementInfo& placementInfo = getPlacementInfo(actorInitInfo);
    sead::Matrix34f zoneMtx = sead::Matrix34f::ident;
    tryGetZoneMatrixTR(&zoneMtx, placementInfo);

    PlacementId placementId;
    const PlacementId* placementIdPtr = nullptr;
    if (tryGetPlacementId(&placementId, placementInfo))
        placementIdPtr = createPlacementId(placementInfo);

    return getCameraDirector(user)->createCamera(poser, placementIdPtr, cameraName,
                                                 CameraTicket::Priority_Object, zoneMtx, true);
}

void initProgramableCameraAngleSwing(CameraTicket* ticket) {
    alCameraPoserFunction::initAngleSwing(ticket->getPoser());
}

CameraTicket* initFollowCameraSimple(const IUseCamera* user, const char* cameraName) {
    CameraPoserFollowSimple* poser = new CameraPoserFollowSimple("フォロー");
    return getCameraDirector(user)->createCamera(
        poser, nullptr, cameraName, CameraTicket::Priority_Object, sead::Matrix34f::ident, true);
}

CameraTicket* initFollowCameraSimple(const IUseCamera* user, const ActorInitInfo& actorInitInfo,
                                     const char* cameraName) {
    CameraPoserFollowSimple* poser = new CameraPoserFollowSimple("フォロー");

    const PlacementInfo& placementInfo = getPlacementInfo(actorInitInfo);
    sead::Matrix34f zoneMtx = sead::Matrix34f::ident;
    tryGetZoneMatrixTR(&zoneMtx, placementInfo);

    PlacementId placementId;
    const PlacementId* placementIdPtr = nullptr;
    if (tryGetPlacementId(&placementId, placementInfo))
        placementIdPtr = createPlacementId(placementInfo);

    return getCameraDirector(user)->createCamera(poser, placementIdPtr, cameraName,
                                                 CameraTicket::Priority_Object, zoneMtx, true);
}

CameraTicket* initDemoObjectCamera(const IUseCamera* user, const ActorInitInfo& actorInitInfo,
                                   const char* cameraName, const char* suffix) {
    const PlacementInfo& placementInfo = getPlacementInfo(actorInitInfo);
    sead::Matrix34f zoneMtx = sead::Matrix34f::ident;
    tryGetZoneMatrixTR(&zoneMtx, placementInfo);
    return getCameraDirector(user)->createObjectCamera(
        createPlacementId(placementInfo), cameraName, suffix, CameraTicket::Priority_Demo, zoneMtx);
}

CameraTicket* initDemoProgramableCamera(const IUseCamera* user, const ActorInitInfo& actorInitInfo,
                                        const char* cameraName, const sead::Vector3f* pos,
                                        const sead::Vector3f* at, const sead::Vector3f* up) {
    CameraTicket* ticket = initProgramableCamera(user, actorInitInfo, cameraName, pos, at, up);
    ticket->setPriority(CameraTicket::Priority_Demo);
    return ticket;
}

CameraTicket* initDemoProgramableCameraKeepColliderPreCamera(
    const IUseCamera* user, const ActorInitInfo& actorInitInfo, const char* cameraName,
    const sead::Vector3f* pos, const sead::Vector3f* at, const sead::Vector3f* up) {
    CameraTicket* ticket =
        initProgramableCameraKeepColliderPreCamera(user, actorInitInfo, cameraName, pos, at, up);
    ticket->setPriority(CameraTicket::Priority_Demo);
    return ticket;
}

CameraTicket* initDemoAnimCamera(const IUseCamera* user, const ActorInitInfo& actorInitInfo,
                                 const Resource* resource, const sead::Matrix34f* baseMtx,
                                 const char* cameraName) {
    CameraTicket* ticket = initAnimCamera(user, actorInitInfo, resource, baseMtx, cameraName);
    ticket->setPriority(CameraTicket::Priority_Demo);
    return ticket;
}

CameraTicket* initAnimCamera(const IUseCamera* user, const ActorInitInfo& actorInitInfo,
                             const Resource* resource, const sead::Matrix34f* baseMtx,
                             const char* cameraName) {
    CameraPoserAnim* poser = new CameraPoserAnim();

    const PlacementInfo& placementInfo = getPlacementInfo(actorInitInfo);
    sead::Matrix34f zoneMtx = sead::Matrix34f::ident;
    tryGetZoneMatrixTR(&zoneMtx, placementInfo);

    PlacementId placementId;
    const PlacementId* placementIdPtr = nullptr;
    if (tryGetPlacementId(&placementId, placementInfo))
        placementIdPtr = createPlacementId(placementInfo);

    CameraTicket* ticket = getCameraDirector(user)->createCamera(
        poser, placementIdPtr, cameraName, CameraTicket::Priority_Object, zoneMtx, true);
    poser->initAnimResource(resource, baseMtx);
    return ticket;
}

CameraTicket* initDemoAnimCamera(const LiveActor* actor, const ActorInitInfo& actorInitInfo,
                                 const char* cameraName) {
    CameraTicket* ticket = initAnimCamera(actor, actorInitInfo, getAnimResource(actor),
                                          actor->getBaseMtx(), cameraName);
    ticket->setPriority(CameraTicket::Priority_Demo);
    return ticket;
}

void loadActorCameraParam(CameraTicket* ticket, const LiveActor* actor, const char* name,
                          const char* suffix) {
    if (isExistModelOrAnimResourceYaml(actor, name, suffix)) {
        ByamlIter iter(getModelOrAnimResourceYaml(actor, name, suffix));
        ticket->getPoser()->load(iter);
    }
}

void loadActorCameraParamInitFile(CameraTicket* ticket, const LiveActor* actor,
                                  const char* suffix) {
    ByamlIter iter;
    if (tryGetActorInitFileIter(&iter, actor, "InitCamera", suffix))
        ticket->getPoser()->load(iter);
}

void setFixActorCameraTarget(CameraTicket* ticket, const LiveActor* actor) {
    ticket->getFixActorPoser()->setTargetActor(actor);
}

void setFixActorCameraAngleH(CameraTicket* ticket, f32 angleH) {
    ticket->getFixActorPoser()->setAngleH(angleH);
}

void setTowerCameraDistance(CameraTicket* ticket, f32 distance) {
    ticket->getTowerPoser()->setDistance(distance);
}

void setTowerCameraStartAngleV(CameraTicket* ticket, f32 startAngleV) {
    alCameraPoserFunction::setCameraStartAngleV(ticket->getPoser(), startAngleV);
}

void setTowerCameraUserMarginAngleH(CameraTicket* ticket, f32 angleH) {
    ticket->getTowerPoser()->setUserMarginAngleH(angleH);
}

void resetTowerCameraUserMarginAngleH(CameraTicket* ticket) {
    ticket->getTowerPoser()->setUserMarginAngleH(-1.0f);
}

void resetTowerCameraInputRotate(CameraTicket* ticket, f32 angle, s32 step) {
    ticket->getTowerPoser()->resetInputRotate(angle, step);
}

CameraTicket* initSubjectiveCamera(const IUseCamera* user, const ActorInitInfo& actorInitInfo,
                                   const char* cameraName) {
    CameraPoserSubjective* poser = new CameraPoserSubjective("主観");

    const PlacementInfo& placementInfo = getPlacementInfo(actorInitInfo);
    sead::Matrix34f zoneMtx = sead::Matrix34f::ident;
    tryGetZoneMatrixTR(&zoneMtx, placementInfo);

    PlacementId placementId;
    const PlacementId* placementIdPtr = nullptr;
    if (tryGetPlacementId(&placementId, placementInfo))
        placementIdPtr = createPlacementId(placementInfo);

    return getCameraDirector(user)->createCamera(poser, placementIdPtr, cameraName,
                                                 CameraTicket::Priority_Player, zoneMtx, true);
}

CameraTicket* initSubjectiveCameraNoSave(const IUseCamera* user, const char* cameraName) {
    CameraPoserSubjective* poser = new CameraPoserSubjective("主観");
    return getCameraDirector(user)->createCamera(
        poser, nullptr, cameraName, CameraTicket::Priority_Player, sead::Matrix34f::ident, true);
}

f32 getSubjectiveCameraOffsetUp(const CameraTicket* ticket) {
    return ticket->getSubjectivePoser()->getCameraOffsetUp();
}

extern "C" f32 _ZN2al30getSubjectiveCameraOffsetFrontEv(CameraPoserSubjective* poser) {
    return poser->getCameraOffsetFront();
}

void setSubjectiveCameraStartAngleH(const CameraTicket* ticket, f32 angleH) {
    ticket->getSubjectivePoser()->setStartAngleH(angleH);
}

void validateSubjectiveCameraResetAngleH(CameraTicket* ticket) {
    ticket->getSubjectivePoser()->validateResetAngleH();
}

void requestSubjectiveCameraZoomIn(CameraTicket* ticket) {
    ticket->getSubjectivePoser()->requestZoomIn();
}

CameraTicket* initParallelCamera(const IUseCamera* user, const char* cameraName) {
    CameraPoserParallelSimple* poser = new CameraPoserParallelSimple("並行");
    return getCameraDirector(user)->createCamera(
        poser, nullptr, cameraName, CameraTicket::Priority_Object, sead::Matrix34f::ident, true);
}

CameraTicket* initParallelCamera(const IUseCamera* user, const ActorInitInfo& actorInitInfo,
                                 const char* cameraName) {
    CameraPoserParallelSimple* poser = new CameraPoserParallelSimple("並行");

    const PlacementInfo& placementInfo = getPlacementInfo(actorInitInfo);
    sead::Matrix34f zoneMtx = sead::Matrix34f::ident;
    tryGetZoneMatrixTR(&zoneMtx, placementInfo);

    PlacementId placementId;
    const PlacementId* placementIdPtr = nullptr;
    if (tryGetPlacementId(&placementId, placementInfo))
        placementIdPtr = createPlacementId(placementInfo);

    return getCameraDirector(user)->createCamera(poser, placementIdPtr, cameraName,
                                                 CameraTicket::Priority_Object, zoneMtx, true);
}

void setParallelCameraLookAtOffset(const CameraTicket* ticket, const sead::Vector3f& offset) {
    ticket->getParallelSimplePoser()->setLookAtOffset(offset);
}

void setParallelCameraDistance(const CameraTicket* ticket, f32 distance) {
    ticket->getParallelSimplePoser()->setDistance(distance);
}

void setParallelCameraAngleH(const CameraTicket* ticket, f32 angleH) {
    ticket->getParallelSimplePoser()->setAngleH(angleH);
}

void setParallelCameraAngleV(const CameraTicket* ticket, f32 angleV) {
    ticket->getParallelSimplePoser()->setAngleV(angleV);
}

CameraTicket* initQuickTurnCamera(const IUseCamera* user, const char* cameraName) {
    CameraPoserQuickTurn* poser = new CameraPoserQuickTurn("くるっとターン");
    return getCameraDirector(user)->createCamera(
        poser, nullptr, cameraName, CameraTicket::Priority_Object, sead::Matrix34f::ident, true);
}

CameraTicket* initRaceCamera(const IUseCamera* user, const ActorInitInfo& actorInitInfo,
                             const char* cameraName) {
    CameraPoserRace* poser = new CameraPoserRace("レース");

    const PlacementInfo& placementInfo = getPlacementInfo(actorInitInfo);
    sead::Matrix34f zoneMtx = sead::Matrix34f::ident;
    tryGetZoneMatrixTR(&zoneMtx, placementInfo);

    PlacementId placementId;
    const PlacementId* placementIdPtr = nullptr;
    if (tryGetPlacementId(&placementId, placementInfo))
        placementIdPtr = createPlacementId(placementInfo);

    return getCameraDirector(user)->createCamera(poser, placementIdPtr, cameraName,
                                                 CameraTicket::Priority_Object, zoneMtx, true);
}

CameraTicket* initCartCamera(const IUseCamera* user, const ActorInitInfo& actorInitInfo,
                             const char* cameraName) {
    CameraPoserCart* poser = new CameraPoserCart("カート");

    const PlacementInfo& placementInfo = getPlacementInfo(actorInitInfo);
    sead::Matrix34f zoneMtx = sead::Matrix34f::ident;
    tryGetZoneMatrixTR(&zoneMtx, placementInfo);

    PlacementId placementId;
    const PlacementId* placementIdPtr = nullptr;
    if (tryGetPlacementId(&placementId, placementInfo))
        placementIdPtr = createPlacementId(placementInfo);

    return getCameraDirector(user)->createCamera(poser, placementIdPtr, cameraName,
                                                 CameraTicket::Priority_Object, zoneMtx, true);
}

CameraTicket* initActorRailParallelCamera(const LiveActor* actor,
                                          const ActorInitInfo& actorInitInfo,
                                          const char* cameraName) {
    CameraPoserActorRailParallel* poser =
        new CameraPoserActorRailParallel("アクターレール並行", actor->getRailKeeper());

    const IUseCamera* user = actor;
    const PlacementInfo& placementInfo = getPlacementInfo(actorInitInfo);
    sead::Matrix34f zoneMtx = sead::Matrix34f::ident;
    tryGetZoneMatrixTR(&zoneMtx, placementInfo);

    PlacementId placementId;
    const PlacementId* placementIdPtr = nullptr;
    if (tryGetPlacementId(&placementId, placementInfo))
        placementIdPtr = createPlacementId(placementInfo);

    return getCameraDirector(user)->createCamera(poser, placementIdPtr, cameraName,
                                                 CameraTicket::Priority_Object, zoneMtx, true);
}

CameraTicket* initKinopioBrigadeCamera(const IUseCamera* user, const ActorInitInfo& actorInitInfo,
                                       const char* cameraName) {
    CameraPoserKinopioBrigade* poser = new CameraPoserKinopioBrigade("キノピオ探検隊");

    const PlacementInfo& placementInfo = getPlacementInfo(actorInitInfo);
    sead::Matrix34f zoneMtx = sead::Matrix34f::ident;
    tryGetZoneMatrixTR(&zoneMtx, placementInfo);

    PlacementId placementId;
    const PlacementId* placementIdPtr = nullptr;
    if (tryGetPlacementId(&placementId, placementInfo))
        placementIdPtr = createPlacementId(placementInfo);

    return getCameraDirector(user)->createCamera(poser, placementIdPtr, cameraName,
                                                 CameraTicket::Priority_Object, zoneMtx, true);
}

CameraTicket* initAnimCamera(const LiveActor* actor, const ActorInitInfo& actorInitInfo,
                             const char* cameraName) {
    return initAnimCamera(actor, actorInitInfo, getAnimResource(actor), actor->getBaseMtx(),
                          cameraName);
}

void validateAnimCameraAngleSwing(CameraTicket* ticket) {
    ticket->getPoser()->getAngleSwingInfo()->validateSwing();
}

void invalidateAnimCameraAngleSwing(CameraTicket* ticket) {
    ticket->getPoser()->getAngleSwingInfo()->invalidateSwing();
}

void setAnimCameraBaseMtxPtr(CameraTicket* ticket, const sead::Matrix34f* baseMtxPtr) {
    ticket->getAnimPoser()->setBaseMtxPtr(baseMtxPtr);
}

CameraTicket* initEntranceCamera(const IUseCamera* user, const PlacementInfo& placementInfo,
                                 const char* cameraName) {
    sead::Matrix34f zoneMtx = sead::Matrix34f::ident;
    tryGetZoneMatrixTR(&zoneMtx, placementInfo);
    return getCameraDirector(user)->createObjectEntranceCamera(createPlacementId(placementInfo),
                                                               cameraName, zoneMtx);
}

CameraTicket* initEntranceCamera(const IUseCamera* user, const ActorInitInfo& actorInitInfo,
                                 const char* cameraName) {
    const PlacementInfo& placementInfo = getPlacementInfo(actorInitInfo);
    sead::Matrix34f zoneMtx = sead::Matrix34f::ident;
    tryGetZoneMatrixTR(&zoneMtx, placementInfo);
    return getCameraDirector(user)->createObjectEntranceCamera(createPlacementId(placementInfo),
                                                               cameraName, zoneMtx);
}

CameraTicket* initEntranceCameraNoSave(const IUseCamera* user, const PlacementInfo& placementInfo,
                                       const char* cameraName) {
    sead::Matrix34f zoneMtx = sead::Matrix34f::ident;
    tryGetZoneMatrixTR(&zoneMtx, placementInfo);
    return getCameraDirector(user)->createObjectEntranceCamera(createPlacementId(placementInfo),
                                                               cameraName, zoneMtx);
}

void invalidateEndEntranceCamera(LiveActor* actor) {
    const IUseCamera* user = actor;
    actor->getName();
    getCameraDirector(user)->getFlagCtrl()->invalidateEndEntranceCamera();
}

void onCameraRideObj(const LiveActor* actor) {
    getCameraDirector(actor)->getSceneCameraCtrl()->getRequestParamHolder()->onRideObj(
        actor, actor->getName());
}

void offCameraRideObj(const LiveActor* actor) {
    getCameraDirector(actor)->getSceneCameraCtrl()->getRequestParamHolder()->offRideObj(
        actor, actor->getName());
}
}  // namespace al

void validateFixPointCameraUsePreCameraPos(al::CameraTicket* ticket) {
    ticket->getFixPointPoser()->validateUsePreCameraPos();
}

namespace alCameraFunction {
al::CameraTicket* initCameraNoPlacementInfoNoSave(al::CameraPoser* poser,
                                                  const al::IUseCamera* user,
                                                  const al::PlacementId* placementId,
                                                  const char* cameraName, s32 priority,
                                                  const sead::Matrix34f& baseMtx) {
    return al::getCameraDirector(user)->createCamera(poser, placementId, cameraName, priority,
                                                     baseMtx, true);
}

al::CameraTicket* initCameraNoSave(al::CameraPoser* poser, const al::IUseCamera* user,
                                   const al::ActorInitInfo& actorInitInfo, const char* cameraName,
                                   s32 priority) {
    const al::PlacementInfo& placementInfo = al::getPlacementInfo(actorInitInfo);
    sead::Matrix34f zoneMtx = sead::Matrix34f::ident;
    al::tryGetZoneMatrixTR(&zoneMtx, placementInfo);

    al::PlacementId placementId;
    const al::PlacementId* placementIdPtr = nullptr;
    if (al::tryGetPlacementId(&placementId, placementInfo))
        placementIdPtr = al::createPlacementId(placementInfo);

    return al::getCameraDirector(user)->createCamera(poser, placementIdPtr, cameraName, priority,
                                                     zoneMtx, true);
}

al::CameraTicket* initCamera(al::CameraPoser* poser, const al::IUseCamera* user,
                             const al::ActorInitInfo& actorInitInfo, const char* cameraName,
                             s32 priority) {
    const al::PlacementInfo& placementInfo = al::getPlacementInfo(actorInitInfo);
    sead::Matrix34f zoneMtx = sead::Matrix34f::ident;
    al::tryGetZoneMatrixTR(&zoneMtx, placementInfo);

    al::PlacementId placementId;
    const al::PlacementId* placementIdPtr = nullptr;
    if (al::tryGetPlacementId(&placementId, placementInfo))
        placementIdPtr = al::createPlacementId(placementInfo);

    return al::getCameraDirector(user)->createCamera(poser, placementIdPtr, cameraName, priority,
                                                     zoneMtx, true);
}

al::CameraTicket* initCameraNoSave(al::CameraPoser* poser, const al::IUseCamera* user,
                                   const al::PlacementInfo& placementInfo, const char* cameraName,
                                   s32 priority) {
    sead::Matrix34f zoneMtx = sead::Matrix34f::ident;
    al::tryGetZoneMatrixTR(&zoneMtx, placementInfo);

    al::PlacementId placementId;
    const al::PlacementId* placementIdPtr = nullptr;
    if (al::tryGetPlacementId(&placementId, placementInfo))
        placementIdPtr = al::createPlacementId(placementInfo);

    return al::getCameraDirector(user)->createCamera(poser, placementIdPtr, cameraName, priority,
                                                     zoneMtx, true);
}

al::CameraTicket* initCameraNoPlacementInfo(al::CameraPoser* poser, const al::IUseCamera* user,
                                            const al::PlacementId* placementId,
                                            const char* cameraName, s32 priority,
                                            const sead::Matrix34f& baseMtx) {
    return al::getCameraDirector(user)->createCamera(poser, placementId, cameraName, priority,
                                                     baseMtx, true);
}

al::CameraTicket* initCamera(al::CameraPoser* poser, const al::IUseCamera* user,
                             const al::PlacementInfo& placementInfo, const char* cameraName,
                             s32 priority) {
    sead::Matrix34f zoneMtx = sead::Matrix34f::ident;
    al::tryGetZoneMatrixTR(&zoneMtx, placementInfo);

    al::PlacementId placementId;
    const al::PlacementId* placementIdPtr = nullptr;
    if (al::tryGetPlacementId(&placementId, placementInfo))
        placementIdPtr = al::createPlacementId(placementInfo);

    return al::getCameraDirector(user)->createCamera(poser, placementIdPtr, cameraName, priority,
                                                     zoneMtx, true);
}

al::CameraTicket* initAreaCamera(const al::IUseCamera* user, const al::PlacementInfo& placementInfo,
                                 const char* cameraName) {
    if (al::isExistLinkChild(placementInfo, "TowerCameraAxis", 0)) {
        al::CameraPoserTower* poser = new al::CameraPoserTower("塔", nullptr);
        sead::Matrix34f zoneMtx = sead::Matrix34f::ident;
        al::tryGetZoneMatrixTR(&zoneMtx, placementInfo);
        al::CameraTicket* ticket = al::getCameraDirector(user)->createCamera(
            poser, al::createPlacementId(placementInfo), cameraName, 1, zoneMtx, true);
        ticket->getPoser()->initByPlacementObj(placementInfo);
        return ticket;
    }

    if (al::isExistLinkChild(placementInfo, "InnerTowerCameraAxis", 0)) {
        al::CameraPoserInnerTower* poser = new al::CameraPoserInnerTower("塔の内側");
        sead::Matrix34f zoneMtx = sead::Matrix34f::ident;
        al::tryGetZoneMatrixTR(&zoneMtx, placementInfo);
        al::CameraTicket* ticket = al::getCameraDirector(user)->createCamera(
            poser, al::createPlacementId(placementInfo), cameraName, 1, zoneMtx, true);
        ticket->getPoser()->initByPlacementObj(placementInfo);
        return ticket;
    }

    sead::Matrix34f zoneMtx = sead::Matrix34f::ident;
    al::tryGetZoneMatrixTR(&zoneMtx, placementInfo);
    al::CameraTicket* ticket = al::getCameraDirector(user)->createObjectCamera(
        al::createPlacementId(placementInfo), cameraName, nullptr, 1, zoneMtx);
    ticket->getPoser()->initByPlacementObj(placementInfo);
    return ticket;
}

al::CameraTicket* initForceAreaCamera(const al::IUseCamera* user,
                                      const al::PlacementInfo& placementInfo,
                                      const char* cameraName) {
    al::CameraTicket* ticket = initAreaCamera(user, placementInfo, cameraName);
    ticket->setPriority(al::CameraTicket::Priority_ForceArea);
    return ticket;
}

void initPriorityBossField(al::CameraTicket* ticket) {
    ticket->setPriority(al::CameraTicket::Priority_BossField);
}

void initPriorityCapture(al::CameraTicket* ticket) {
    ticket->setPriority(al::CameraTicket::Priority_Capture);
}

void initPriorityObject(al::CameraTicket* ticket) {
    ticket->setPriority(al::CameraTicket::Priority_Object);
}

void initPrioritySafetyPointRecovery(al::CameraTicket* ticket) {
    ticket->setPriority(al::CameraTicket::Priority_SafetyPointRecovery);
}

void initPriorityDemoTalk(al::CameraTicket* ticket) {
    ticket->setPriority(al::CameraTicket::Priority_DemoTalk);
}

void initPriorityDemo(al::CameraTicket* ticket) {
    ticket->setPriority(al::CameraTicket::Priority_Demo);
}

void initPriorityDemo2(al::CameraTicket* ticket) {
    ticket->setPriority(al::CameraTicket::Priority_Demo2);
}

void initCameraSettingCloudSea(al::IUseCamera* user, f32 distance) {
    al::getCameraDirector(user)->initSettingCloudSea(distance);
}

al::CameraTicket* initMirrorAreaCamera(const al::IUseCamera* user,
                                       const al::PlacementInfo& placementInfo,
                                       const char* cameraName) {
    sead::Matrix34f zoneMtx = sead::Matrix34f::ident;
    al::tryGetZoneMatrixTR(&zoneMtx, placementInfo);
    return al::getCameraDirector(user)->createMirrorObjectCamera(
        al::createPlacementId(placementInfo), cameraName, 1, zoneMtx);
}

void setPoserNearClipDistance(al::CameraTicket* ticket, f32 distance) {
    ticket->getPoser()->setNearClipDistance(distance);
}

f32 getNearClipDistance(const al::IUseCamera* user, s32 updaterIdx) {
    return al::getCameraDirector(user)->getPoseUpdater(updaterIdx)->getNearClipDistance();
}

bool isCurrentCameraPriorityPlayer(const al::IUseCamera* user, s32 updaterIdx) {
    return al::getCameraDirector(user)
        ->getPoseUpdater(updaterIdx)
        ->isCurrentCameraPriority(al::CameraTicket::Priority_Player);
}

void validateCameraArea2D(al::IUseCamera* user) {
    al::getCameraDirector(user)->validateCameraArea2D();
}

void invalidateCameraArea2D(al::IUseCamera* user) {
    al::getCameraDirector(user)->invalidateCameraArea2D();
}

void validateCameraAreaKids(al::IUseCamera* user) {
    al::getCameraDirector(user)->getFlagCtrl()->isValidCameraAreaKids = true;
}

bool isValidCameraAreaKids(const al::CameraFlagCtrl* flagCtrl) {
    return flagCtrl->isValidCameraAreaKids || flagCtrl->isSeparatePlayMode;
}

void onSeparatePlayMode(al::IUseCamera* user) {
    al::getCameraDirector(user)->getFlagCtrl()->isSeparatePlayMode = true;
}

void offSeparatePlayMode(al::IUseCamera* user) {
    al::getCameraDirector(user)->getFlagCtrl()->isSeparatePlayMode = false;
}

void validateResetPoseNextCamera(al::CameraTicket* ticket) {
    ticket->getPoser()->getPoserFlag()->validateResetPoseNextCamera();
}

void validateKeepPreSelfPoseNextCamera(al::CameraTicket* ticket) {
    ticket->getPoser()->getPoserFlag()->validateKeepPreSelfPoseNextCamera();
}

void validateCameraInterpoleEaseOut(al::CameraTicket* ticket) {
    ticket->getPoser()->setInterpoleEaseOut();
}

void onForceCollideAtStartInterpole(al::CameraTicket* ticket) {
    ticket->getPoser()->getPoserFlag()->onForceCollideAtStartInterpole();
}

void requestCameraShakeLoop(const al::IUseCamera* user, const char* shakeName) {
    al::getCameraDirector(user)->getSceneCameraCtrl()->getSceneViewAt(0)->setCameraShakeLoopName(
        shakeName);
}
}  // namespace alCameraFunction
