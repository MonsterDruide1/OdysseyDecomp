#include "Library/Camera/CameraPoserFunction.h"

#include "Library/Base/StringUtil.h"
#include "Library/Camera/CameraArrowCollider.h"
#include "Library/Camera/CameraFlagCtrl.h"
#include "Library/Camera/CameraInputHolder.h"
#include "Library/Camera/CameraLimitRailKeeper.h"
#include "Library/Camera/CameraOffsetCtrlPreset.h"
#include "Library/Camera/CameraParamMoveLimit.h"
#include "Library/Camera/CameraPoser.h"
#include "Library/Camera/CameraPoserFix.h"
#include "Library/Camera/CameraPoserFlag.h"
#include "Library/Camera/CameraPoserSceneInfo.h"
#include "Library/Camera/CameraRequestParamHolder.h"
#include "Library/Camera/CameraStartInfo.h"
#include "Library/Camera/CameraSubTargetBase.h"
#include "Library/Camera/CameraTargetBase.h"
#include "Library/Camera/CameraTargetCollideInfoHolder.h"
#include "Library/Camera/CameraTargetHolder.h"
#include "Library/Camera/CameraTriangleFilter.h"
#include "Library/Camera/CameraViewCtrlGyro.h"
#include "Library/Camera/CameraViewInfo.h"
#include "Library/Camera/SimpleCameraInput.h"
#include "Library/Camera/SnapShotCameraCtrl.h"
#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/Collision/CollisionPartsTriangle.h"
#include "Library/Collision/PartsInterpolator.h"
#include "Library/Math/MathUtil.h"
#include "Library/Obj/CameraRailHolder.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementId.h"
#include "Library/Placement/PlacementInfo.h"
#include "Library/Play/Camera/CameraVerticalAbsorber.h"
#include "Library/Projection/Projection.h"
#include "Library/Rail/RailUtil.h"
#include "Project/Camera/CameraAngleCtrlInfo.h"
#include "Project/Camera/CameraAngleSwingInfo.h"
#include "Project/Camera/CameraCollisionPartsFilter.h"
#include "Project/Camera/CameraObjectRequestInfo.h"
#include "Project/Camera/CameraSubTargetTurnParam.h"

namespace alCameraPoserFunction {
static al::CameraCollisionPartsFilter sPartsFiler;
static al::CameraTriangleFilter sTriangleFilter;
static al::CameraTriangleFilterOnlyCeiling sCeilFilter;
static sead::Vector3f sMtxX = {-1.0f, 0.0f, 0.0f};
static sead::Vector3f sMtxY = {0.0f, 1.0f, 0.0f};
static sead::Vector3f sMtxZ = {0.0f, 0.0f, -1.0f};

static inline s32 getViewInfoIndex(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getViewInfo()->getIndex();
}

static inline al::CameraTargetBase* getTarget(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getTargetHolder()->getViewTarget(getViewInfoIndex(cameraPoser));
}

static inline al::CameraTargetBase* tryGetTarget(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getTargetHolder()->tryGetViewTarget(getViewInfoIndex(cameraPoser));
}

static inline al::CameraVerticalAbsorber* getVerticalAbsorber(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getCameraVerticalAbsorber();
}

static inline al::CameraTargetCollideInfoHolder*
getTargetCollision(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getSceneInfo()->targetCollideInfoHolder;
}

static inline al::ICameraInput* getCameraInput(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getInputHolder()->getInput(getViewInfoIndex(cameraPoser));
}

static inline al::CameraSubTargetBase* getTopSubTarget(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getTargetHolder()->getTopSubTarget();
}

static inline al::CameraSubTargetTurnParam*
getSubTargetTurnParam(const al::CameraPoser* cameraPoser) {
    return getTopSubTarget(cameraPoser)->getSubTargetTurnParam();
}

s32 getViewIndex(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getCameraViewInfo()->getIndex();
}

const sead::LookAtCamera& getLookAtCamera(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getCameraViewInfo()->getLookAtCam();
}

const sead::Projection& getProjectionSead(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getCameraViewInfo()->getProjectionSead();
}

const al::Projection& getProjection(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getCameraViewInfo()->getProjection();
}

const sead::Matrix44f& getProjectionMtx(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getCameraViewInfo()->getProjMtx();
}

f32 getNear(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getCameraViewInfo()->getNear();
}

f32 getFar(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getCameraViewInfo()->getFar();
}

f32 getAspect(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getCameraViewInfo()->getAspect();
}

const sead::Vector3f& getPreCameraPos(const al::CameraPoser* cameraPoser) {
    return getLookAtCamera(cameraPoser).getPos();
}

const sead::Vector3f& getPreLookAtPos(const al::CameraPoser* cameraPoser) {
    return getLookAtCamera(cameraPoser).getAt();
}

const sead::Vector3f& getPreUpDir(const al::CameraPoser* cameraPoser) {
    return getLookAtCamera(cameraPoser).getUp();
}

f32 getPreFovyDegree(const al::CameraPoser* cameraPoser) {
    return sead::Mathf::rad2deg(getPreFovyRadian(cameraPoser));
}

f32 getPreFovyRadian(const al::CameraPoser* cameraPoser) {
    return getProjection(cameraPoser).getFovy();
}

bool isPrePriorityDemo(const al::CameraStartInfo& cameraStartInfo) {
    return cameraStartInfo.prePriorityType == al::CameraTicket::Priority_Demo;
}

bool isPrePriorityDemo2(const al::CameraStartInfo& cameraStartInfo) {
    return cameraStartInfo.prePriorityType == al::CameraTicket::Priority_Demo2;
}

bool isPrePriorityDemoTalk(const al::CameraStartInfo& cameraStartInfo) {
    return cameraStartInfo.prePriorityType == al::CameraTicket::Priority_DemoTalk;
}

bool isPrePriorityDemoAll(const al::CameraStartInfo& cameraStartInfo) {
    return isPrePriorityDemo(cameraStartInfo) || isPrePriorityDemo2(cameraStartInfo);
}

bool isPrePriorityEntranceAll(const al::CameraStartInfo& cameraStartInfo) {
    return cameraStartInfo.prePriorityType == al::CameraTicket::Priority_Entrance ||
           cameraStartInfo.prePriorityType == al::CameraTicket::Priority_Unknown;
}

bool isPrePriorityPlayer(const al::CameraStartInfo& cameraStartInfo) {
    return cameraStartInfo.prePriorityType == al::CameraTicket::Priority_Player;
}

bool isEqualPreCameraName(const al::CameraStartInfo& cameraStartInfo, const char* cameraName) {
    if (cameraStartInfo.preCameraName)
        return al::isEqualString(cameraName, cameraStartInfo.preCameraName);
    return false;
}

bool isPreCameraFixAbsolute(const al::CameraStartInfo& cameraStartInfo) {
    return isEqualPreCameraName(cameraStartInfo, al::CameraPoserFix::getFixAbsoluteCameraName());
}

bool isInvalidCollidePreCamera(const al::CameraStartInfo& cameraStartInfo) {
    return cameraStartInfo.isInvalidCollidePreCamera;
}

bool isInvalidKeepPreCameraDistance(const al::CameraStartInfo& cameraStartInfo) {
    return cameraStartInfo.isInvalidKeepPreCameraDistance;
}

bool isInvalidKeepPreCameraDistanceIfNoCollide(const al::CameraStartInfo& cameraStartInfo) {
    return cameraStartInfo.isInvalidKeepPreCameraDistanceIfNoCollide;
}

bool isValidResetPreCameraPose(const al::CameraStartInfo& cameraStartInfo) {
    return cameraStartInfo.isValidResetPreCameraPose;
}

bool isValidKeepPreSelfCameraPose(const al::CameraStartInfo& cameraStartInfo) {
    return cameraStartInfo.isValidKeepPreSelfCameraPose;
}

f32 getPreCameraSwingAngleH(const al::CameraStartInfo& cameraStartInfo) {
    return cameraStartInfo.preCameraSwingAngleH;
}

f32 getPreCameraSwingAngleV(const al::CameraStartInfo& cameraStartInfo) {
    return cameraStartInfo.preCameraSwingAngleV;
}

f32 getPreCameraMaxSwingAngleH(const al::CameraStartInfo& cameraStartInfo) {
    return cameraStartInfo.preCameraMaxSwingAngleH;
}

f32 getPreCameraMaxSwingAngleV(const al::CameraStartInfo& cameraStartInfo) {
    return cameraStartInfo.preCameraMaxSwingAngleV;
}

bool isExistAreaAngleH(const al::CameraStartInfo& cameraStartInfo) {
    return cameraStartInfo.isExistAreaAngleH;
}

bool isExistAreaAngleV(const al::CameraStartInfo& cameraStartInfo) {
    return cameraStartInfo.isExistAreaAngleV;
}

f32 getAreaAngleH(const al::CameraStartInfo& cameraStartInfo) {
    return cameraStartInfo.areaAngleH;
}

f32 getAreaAngleV(const al::CameraStartInfo& cameraStartInfo) {
    return cameraStartInfo.areaAngleV;
}

bool isExistNextPoseByPreCamera(const al::CameraStartInfo& cameraStartInfo) {
    return cameraStartInfo.isExistNextPoseByPreCamera;
}

f32 getNextAngleHByPreCamera(const al::CameraStartInfo& cameraStartInfo) {
    return cameraStartInfo.nextAngleHByPreCamera;
}

f32 getNextAngleVByPreCamera(const al::CameraStartInfo& cameraStartInfo) {
    return cameraStartInfo.nextAngleVByPreCamera;
}

void calcCameraPose(sead::Quatf* outPose, const al::CameraPoser* cameraPoser) {
    sead::Vector3f lookDir;
    calcLookDir(&lookDir, cameraPoser);
    al::makeQuatFrontUp(outPose, lookDir, cameraPoser->getUp());
}

void calcLookDir(sead::Vector3f* outDir, const al::CameraPoser* cameraPoser) {
    calcCameraDir(outDir, cameraPoser);
    outDir->negate();
}

void calcCameraDir(sead::Vector3f* outDir, const al::CameraPoser* cameraPoser) {
    outDir->setSub(cameraPoser->getEye(), cameraPoser->getAt());
    al::normalize(outDir);
}

bool calcCameraDirH(sead::Vector3f* outDirH, const al::CameraPoser* cameraPoser) {
    outDirH->setSub(cameraPoser->getEye(), cameraPoser->getAt());
    al::verticalizeVec(outDirH, cameraPoser->getUp(), *outDirH);
    return al::tryNormalizeOrZero(outDirH);
}

bool calcLookDirH(sead::Vector3f* outDirH, const al::CameraPoser* cameraPoser) {
    bool isValid = calcCameraDirH(outDirH, cameraPoser);
    outDirH->negate();
    return isValid;
}

void calcSideDir(sead::Vector3f* outDir, const al::CameraPoser* cameraPoser) {
    sead::Vector3f facingDir = cameraPoser->getEye() - cameraPoser->getAt();
    al::normalize(&facingDir);
    outDir->set(facingDir.cross(cameraPoser->getUp()));
    al::normalize(outDir);
}

void calcPreCameraDir(sead::Vector3f* outDir, const al::CameraPoser* cameraPoser) {
    const sead::LookAtCamera& lookAtCam = getLookAtCamera(cameraPoser);
    outDir->setSub(lookAtCam.getPos(), lookAtCam.getAt());
    al::normalize(outDir);
}

void calcPreCameraDirH(sead::Vector3f* outDirH, const al::CameraPoser* cameraPoser) {
    const sead::LookAtCamera& lookAtCam = getLookAtCamera(cameraPoser);
    outDirH->setSub(lookAtCam.getPos(), lookAtCam.getAt());
    al::verticalizeVec(outDirH, sead::Vector3f::ey, *outDirH);
    al::tryNormalizeOrZero(outDirH);
}

void calcPreLookDir(sead::Vector3f* outDir, const al::CameraPoser* cameraPoser) {
    calcPreCameraDir(outDir, cameraPoser);
    outDir->negate();
}

void calcPreLookDirH(sead::Vector3f* outDirH, const al::CameraPoser* cameraPoser) {
    const sead::LookAtCamera& lookAtCam = getLookAtCamera(cameraPoser);
    outDirH->set(lookAtCam.getAt() - lookAtCam.getPos());
    al::verticalizeVec(outDirH, getPreUpDir(cameraPoser), *outDirH);
    al::tryNormalizeOrZero(outDirH);
}

f32 calcPreCameraAngleH(const al::CameraPoser* cameraPoser) {
    sead::Vector3f cameraDirH = {0.0f, 0.0f, 0.0f};
    calcPreCameraDirH(&cameraDirH, cameraPoser);
    return sead::Mathf::rad2deg(sead::Mathf::atan2(cameraDirH.x, cameraDirH.z));
}

f32 calcPreCameraAngleV(const al::CameraPoser* cameraPoser) {
    sead::Vector3f cameraDir = {0.0f, 0.0f, 0.0f};
    calcPreCameraDir(&cameraDir, cameraPoser);
    al::parallelizeVec(&cameraDir, sead::Vector3f::ey, cameraDir);
    return sead::Mathf::rad2deg(sead::Mathf::asin(cameraDir.length()));
}

void setLookAtPosToTarget(al::CameraPoser* cameraPoser) {
    sead::Vector3f targetTrans = {0.0f, 0.0f, 0.0f};
    calcTargetTrans(&targetTrans, cameraPoser);
    cameraPoser->setAt(targetTrans);
}

void calcTargetTrans(sead::Vector3f* outTrans, const al::CameraPoser* cameraPoser) {
    getTarget(cameraPoser)->calcTrans(outTrans);
}

void setLookAtPosToTargetAddOffset(al::CameraPoser* cameraPoser, const sead::Vector3f& offset) {
    sead::Vector3f targetTrans = {0.0f, 0.0f, 0.0f};
    calcTargetTrans(&targetTrans, cameraPoser);
    cameraPoser->setAt(targetTrans + offset);
}

void setCameraPosToTarget(al::CameraPoser* cameraPoser) {
    sead::Vector3f targetTrans = {0.0f, 0.0f, 0.0f};
    calcTargetTrans(&targetTrans, cameraPoser);
    cameraPoser->setEye(targetTrans);
}

void setCameraPosToTargetAddOffset(al::CameraPoser* cameraPoser, const sead::Vector3f& offset) {
    sead::Vector3f targetTrans = {0.0f, 0.0f, 0.0f};
    calcTargetTrans(&targetTrans, cameraPoser);
    cameraPoser->setEye(targetTrans + offset);
}

void calcTargetTransWithOffset(sead::Vector3f* outTrans, const al::CameraPoser* cameraPoser) {
    calcTargetTrans(outTrans, cameraPoser);
    al::CameraOffsetCtrlPreset* cameraOffsetCtrlPreset = cameraPoser->getCameraOffsetCtrlPreset();
    if (cameraOffsetCtrlPreset)
        outTrans->add(cameraOffsetCtrlPreset->getOffset());
}

void calcTargetVelocity(sead::Vector3f* outVelocity, const al::CameraPoser* cameraPoser) {
    getTarget(cameraPoser)->calcVelocity(outVelocity);
}

void calcTargetVelocityH(sead::Vector3f* outVelocityH, const al::CameraPoser* cameraPoser) {
    calcTargetVelocity(outVelocityH, cameraPoser);
    sead::Vector3f up = sead::Vector3f::ey;
    calcTargetUp(&up, cameraPoser);
    al::verticalizeVec(outVelocityH, up, *outVelocityH);
}

void calcTargetUp(sead::Vector3f* outDir, const al::CameraPoser* cameraPoser) {
    getTarget(cameraPoser)->calcUp(outDir);
}

f32 calcTargetSpeedV(const al::CameraPoser* cameraPoser) {
    sead::Vector3f targetVelocity = {0.0f, 0.0f, 0.0f};
    sead::Vector3f targetUp = {0.0f, 0.0f, 0.0f};
    calcTargetVelocity(&targetVelocity, cameraPoser);
    calcTargetUp(&targetUp, cameraPoser);
    al::parallelizeVec(&targetVelocity, targetUp, targetVelocity);

    f32 direction = targetVelocity.dot(targetUp);
    f32 speedV = targetVelocity.length();

    if (direction > 0.0f)
        return speedV;
    else
        return -speedV;
}

void calcTargetPose(sead::Quatf* outPose, const al::CameraPoser* cameraPoser) {
    sead::Vector3f targetUp = {0.0f, 0.0f, 0.0f};
    sead::Vector3f targetFront = {0.0f, 0.0f, 0.0f};
    calcTargetUp(&targetUp, cameraPoser);
    calcTargetFront(&targetFront, cameraPoser);
    al::makeQuatFrontUp(outPose, targetFront, targetUp);
}

void calcTargetFront(sead::Vector3f* outDir, const al::CameraPoser* cameraPoser) {
    getTarget(cameraPoser)->calcFront(outDir);
}

void calcTargetSide(sead::Vector3f* outDir, const al::CameraPoser* cameraPoser) {
    getTarget(cameraPoser)->calcSide(outDir);
}

void calcTargetGravity(sead::Vector3f* outDir, const al::CameraPoser* cameraPoser) {
    getTarget(cameraPoser)->calcGravity(outDir);
}

f32 calcTargetSpeedH(const al::CameraPoser* cameraPoser) {
    sead::Vector3f targetGravity = {0.0f, 0.0f, 0.0f};
    sead::Vector3f targetVelocity = {0.0f, 0.0f, 0.0f};
    calcTargetGravity(&targetGravity, cameraPoser);
    calcTargetVelocity(&targetVelocity, cameraPoser);

    al::verticalizeVec(&targetVelocity, targetGravity, targetVelocity);
    return targetVelocity.length();
}

f32 calcTargetJumpSpeed(const al::CameraPoser* cameraPoser) {
    sead::Vector3f targetGravity = {0.0f, 0.0f, 0.0f};
    sead::Vector3f targetVelocity = {0.0f, 0.0f, 0.0f};
    calcTargetGravity(&targetGravity, cameraPoser);
    calcTargetVelocity(&targetVelocity, cameraPoser);

    al::parallelizeVec(&targetVelocity, targetGravity, targetVelocity);
    if (al::isNearZero(targetVelocity) || targetGravity.dot(targetVelocity) > 0.0f)
        return 0.0f;

    return targetVelocity.length();
}

f32 calcTargetFallSpeed(const al::CameraPoser* cameraPoser) {
    sead::Vector3f targetGravity = {0.0f, 0.0f, 0.0f};
    sead::Vector3f targetVelocity = {0.0f, 0.0f, 0.0f};
    calcTargetGravity(&targetGravity, cameraPoser);
    calcTargetVelocity(&targetVelocity, cameraPoser);

    al::parallelizeVec(&targetVelocity, targetGravity, targetVelocity);
    if (al::isNearZero(targetVelocity) || targetGravity.dot(targetVelocity) < 0.0f)
        return 0.0f;

    return targetVelocity.length();
}

bool isChangeTarget(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getTargetHolder()->isChangeViewTarget(getViewInfoIndex(cameraPoser));
}

bool tryGetTargetRequestDistance(f32* outDistance, const al::CameraPoser* cameraPoser) {
    al::CameraTargetBase* target = getTarget(cameraPoser);

    if (target->getRequestDistance() > 0.0f) {
        *outDistance = target->getRequestDistance();
        return true;
    }

    return false;
}

f32* tryGetBossDistanceCurve(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getSceneInfo()->requestParamHolder->get_60();
}

f32* tryGetEquipmentDistanceCurve(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getSceneInfo()->requestParamHolder->get_58();
}

bool isExistCollisionUnderTarget(const al::CameraPoser* cameraPoser) {
    return getTargetCollision(cameraPoser)->isExistCollisionUnderTarget();
}

const sead::Vector3f& getUnderTargetCollisionPos(const al::CameraPoser* cameraPoser) {
    return getTargetCollision(cameraPoser)->getTargetCollisionPos();
}

const sead::Vector3f& getUnderTargetCollisionNormal(const al::CameraPoser* cameraPoser) {
    return getTargetCollision(cameraPoser)->getTargetCollisionNormal();
}

bool isExistSlopeCollisionUnderTarget(const al::CameraPoser* cameraPoser) {
    return isExistCollisionUnderTarget(cameraPoser) &&
           getTargetCollision(cameraPoser)->isExistSlopeCollisionUnderTarget();
}

bool isExistWallCollisionUnderTarget(const al::CameraPoser* cameraPoser) {
    return getTargetCollision(cameraPoser)->isExistUnderWall();
}

bool tryCalcSlopeCollisionDownFrontDirH(sead::Vector3f* outDirH,
                                        const al::CameraPoser* cameraPoser) {
    return getTargetCollision(cameraPoser)->tryCalcSlopDownFrontDirH(outDirH);
}

f32 getSlopeCollisionUpSpeed(const al::CameraPoser* cameraPoser) {
    return getTargetCollision(cameraPoser)->getSlopeCollisionUpSpeed();
}

f32 getSlopeCollisionDownSpeed(const al::CameraPoser* cameraPoser) {
    return getTargetCollision(cameraPoser)->getSlopeCollisionDownSpeed();
}

bool isExistSubTarget(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getTargetHolder()->getTopSubTargetInfo().target;
}

bool checkValidTurnToSubTarget(const al::CameraPoser* cameraPoser) {
    if (!isExistSubTarget(cameraPoser))
        return false;

    al::CameraSubTargetTurnParam* subTargetTurnParam = getSubTargetTurnParam(cameraPoser);
    if (subTargetTurnParam->validTurnDegreeRangeH < 0.0f &&
        subTargetTurnParam->validFaceDegreeRangeH != 0.0f) {
        return true;
    }

    sead::Vector3f lookDirH = {};
    if (!calcLookDirH(&lookDirH, cameraPoser))
        return false;

    if (subTargetTurnParam->validFaceDegreeRangeH >= 0.0f) {
        sead::Vector3f targetBack = {0.0f, 0.0f, 0.0f};
        calcSubTargetBack(&targetBack, cameraPoser);
        if (subTargetTurnParam->validFaceDegreeRangeH < al::calcAngleDegree(targetBack, lookDirH))
            return false;
    }

    if (subTargetTurnParam->validTurnDegreeRangeH >= 0.0f) {
        sead::Vector3f targetTrans = {0.0f, 0.0f, 0.0f};
        sead::Vector3f lookDir = {0.0f, 0.0f, 0.0f};
        calcSubTargetTrans(&targetTrans, cameraPoser);
        lookDir = targetTrans - cameraPoser->getEye();
        al::verticalizeVec(&lookDir, cameraPoser->getUp(), lookDir);
        if (!al::tryNormalizeOrZero(&lookDir))
            return false;
        if (subTargetTurnParam->validTurnDegreeRangeH / 2 < al::calcAngleDegree(lookDirH, lookDir))
            return false;
    }

    return true;
}

void calcSubTargetBack(sead::Vector3f* outDir, const al::CameraPoser* cameraPoser) {
    calcSubTargetFront(outDir, cameraPoser);
    outDir->negate();
}

void calcSubTargetTrans(sead::Vector3f* outTrans, const al::CameraPoser* cameraPoser) {
    getTopSubTarget(cameraPoser)->calcTrans(outTrans);
}

bool isChangeSubTarget(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getTargetHolder()->getTopSubTargetInfo().hasTargetChanged;
}

void calcSubTargetFront(sead::Vector3f* outDir, const al::CameraPoser* cameraPoser) {
    getTopSubTarget(cameraPoser)->calcFront(outDir);
}

f32 getSubTargetRequestDistance(const al::CameraPoser* cameraPoser) {
    return getTopSubTarget(cameraPoser)->getRequestDistance();
}

f32 getSubTargetTurnSpeedRate1(const al::CameraPoser* cameraPoser) {
    return getSubTargetTurnParam(cameraPoser)->turnSpeedRate1;
}

f32 getSubTargetTurnSpeedRate2(const al::CameraPoser* cameraPoser) {
    return getSubTargetTurnParam(cameraPoser)->turnSpeedRate2;
}

s32 getSubTargetTurnRestartStep(const al::CameraPoser* cameraPoser) {
    return getSubTargetTurnParam(cameraPoser)->targetTurnRestartStep;
}

bool tryCalcSubTargetTurnBrakeDistanceRate(f32* outDistanceRate,
                                           const al::CameraPoser* cameraPoser) {
    al::CameraSubTargetTurnParam* turnParam = getSubTargetTurnParam(cameraPoser);

    sead::Vector3f targetTrans = {0.0f, 0.0f, 0.0f};
    sead::Vector3f subTargetTrans = {0.0f, 0.0f, 0.0f};
    calcTargetTrans(&targetTrans, cameraPoser);
    calcSubTargetTrans(&subTargetTrans, cameraPoser);

    f32 distance = sead::Mathf::sqrt(sead::Mathf::pow(targetTrans.x - subTargetTrans.x, 2) +
                                     sead::Mathf::pow(targetTrans.z - subTargetTrans.z, 2));

    if (turnParam->turnBrakeEndDistance > 0.0f &&
        turnParam->turnBrakeEndDistance < turnParam->turnBrakeStartDistance &&
        distance < turnParam->turnBrakeStartDistance) {
        *outDistanceRate = 1.0f - al::normalize(distance, turnParam->turnBrakeEndDistance,
                                                turnParam->turnBrakeStartDistance);
        return true;
    }

    if (turnParam->turnStopStartDistance > 0.0f &&
        turnParam->turnStopStartDistance < turnParam->turnStopEndDistance &&
        turnParam->turnStopStartDistance < distance) {
        *outDistanceRate = al::normalize(distance, turnParam->turnStopStartDistance,
                                         turnParam->turnStopEndDistance);
        return true;
    }

    return false;
}

bool isValidSubTargetTurnV(const al::CameraPoser* cameraPoser) {
    return getSubTargetTurnParam(cameraPoser)->isTurnV;
}

bool isValidSubTargetResetAfterTurnV(const al::CameraPoser* cameraPoser) {
    return getSubTargetTurnParam(cameraPoser)->isResetAfterTurnV;
}

void clampAngleSubTargetTurnRangeV(f32* outTurnRangeV, const al::CameraPoser* cameraPoser) {
    al::CameraSubTargetBase* topSubTarget = getTopSubTarget(cameraPoser);
    f32 min = topSubTarget->getSubTargetTurnParam()->minTurnDegreeV;
    f32 max = topSubTarget->getSubTargetTurnParam()->maxTurnDegreeV;

    *outTurnRangeV = sead::Mathf::clamp(*outTurnRangeV, min, max);
}

void initCameraVerticalAbsorber(al::CameraPoser* cameraPoser) {
    cameraPoser->setVerticalAbsorber(new al::CameraVerticalAbsorber(cameraPoser, false));
}

void initCameraVerticalAbsorberNoCameraPosAbsorb(al::CameraPoser* cameraPoser) {
    cameraPoser->setVerticalAbsorber(new al::CameraVerticalAbsorber(cameraPoser, true));
}

f32 getCameraVerticalAbsorbPosUp(const al::CameraPoser* cameraPoser) {
    return getVerticalAbsorber(cameraPoser)->getAbsorbScreenPosUp();
}

f32 getCameraVerticalAbsorbPosDown(const al::CameraPoser* cameraPoser) {
    return getVerticalAbsorber(cameraPoser)->getAbsorbScreenPosDown();
}

void liberateVerticalAbsorb(al::CameraPoser* cameraPoser) {
    getVerticalAbsorber(cameraPoser)->liberateAbsorb();
}

void stopUpdateVerticalAbsorb(al::CameraPoser* cameraPoser) {
    getVerticalAbsorber(cameraPoser)->setIsStopUpdate(true);
}

void stopUpdateVerticalAbsorbForSnapShotMode(al::CameraPoser* cameraPoser,
                                             const sead::Vector3f& absorbVec) {
    stopUpdateVerticalAbsorb(cameraPoser);
    getVerticalAbsorber(cameraPoser)->tryResetAbsorbVecIfInCollision(absorbVec);
}

void restartUpdateVerticalAbsorb(al::CameraPoser* cameraPoser) {
    getVerticalAbsorber(cameraPoser)->setIsStopUpdate(false);
}

void validateVerticalAbsorbKeepInFrame(al::CameraPoser* cameraPoser) {
    getVerticalAbsorber(cameraPoser)->setIsKeepInFrame(true);
}

void invalidateVerticalAbsorbKeepInFrame(al::CameraPoser* cameraPoser) {
    getVerticalAbsorber(cameraPoser)->setIsKeepInFrame(false);
}

void setVerticalAbsorbKeepInFrameScreenOffsetUp(al::CameraPoser* cameraPoser,
                                                f32 keepInFrameOffsetUp) {
    getVerticalAbsorber(cameraPoser)->setKeepInFrameOffsetUp(keepInFrameOffsetUp);
}

void setVerticalAbsorbKeepInFrameScreenOffsetDown(al::CameraPoser* cameraPoser,
                                                  f32 keepInFrameOffsetDown) {
    getVerticalAbsorber(cameraPoser)->setKeepInFrameOffsetDown(keepInFrameOffsetDown);
}

void initCameraArrowCollider(al::CameraPoser* cameraPoser) {
    al::CameraArrowCollider* cameraArrowCollider =
        new al::CameraArrowCollider(cameraPoser->getCollisionDirector());
    cameraPoser->initArrowCollider(cameraArrowCollider);
}

void initCameraArrowColliderWithoutThroughPassCollision(al::CameraPoser* cameraPoser) {
    al::CameraArrowCollider* cameraArrowCollider =
        new al::CameraArrowCollider(cameraPoser->getCollisionDirector());
    cameraArrowCollider->set_48c(true);
    cameraPoser->initArrowCollider(cameraArrowCollider);
}

void initCameraMoveLimit(al::CameraPoser* cameraPoser) {
    cameraPoser->setParamMoveLimit(al::CameraParamMoveLimit::create(cameraPoser));
}

void initCameraAngleCtrl(al::CameraPoser* cameraPoser) {
    cameraPoser->setAngleCtrlInfo(new al::CameraAngleCtrlInfo());
}

void initCameraAngleCtrlWithRelativeH(al::CameraPoser* cameraPoser) {
    cameraPoser->setAngleCtrlInfo(al::CameraAngleCtrlInfo::createWithRelativeH());
}

void initCameraDefaultAngleRangeV(al::CameraPoser* cameraPoser, f32 min, f32 max) {
    cameraPoser->getAngleCtrlInfo()->setDefaultAngleV(min, max);
}

void setCameraStartAngleV(al::CameraPoser* cameraPoser, f32 angle) {
    cameraPoser->getAngleCtrlInfo()->setStartAngleV(angle);
}

void setCameraAngleV(al::CameraPoser* cameraPoser, f32 angle) {
    cameraPoser->getAngleCtrlInfo()->setAngleV(angle);
}

f32 getCameraAngleH(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getAngleCtrlInfo()->getAngleH();
}

f32 getCameraAngleV(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getAngleCtrlInfo()->getAngleV();
}

void initAngleSwing(al::CameraPoser* cameraPoser) {
    cameraPoser->setAngleSwingInfo(new al::CameraAngleSwingInfo());
}

bool isValidAngleSwing(const al::CameraPoser* cameraPoser) {
    return !cameraPoser->getAngleSwingInfo()->isInvalidSwing();
}

void initCameraOffsetCtrlPreset(al::CameraPoser* cameraPoser) {
    cameraPoser->setOffsetCtrlPreset(new al::CameraOffsetCtrlPreset());
}

const sead::Vector3f& getOffset(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getOffsetCtrlPreset()->getOffset();
}

void initGyroCameraCtrl(al::CameraPoser* cameraPoser) {
    cameraPoser->setGyroCtrl(new al::GyroCameraCtrl());
}

void resetGyro(al::CameraPoser* cameraPoser) {
    sead::Vector3f gyroPoseX;
    sead::Vector3f gyroPoseY;
    sead::Vector3f gyroPoseZ;
    calcCameraGyroPose(cameraPoser, &gyroPoseX, &gyroPoseY, &gyroPoseZ);
    cameraPoser->getGyroCtrl()->reset(gyroPoseX, gyroPoseY, gyroPoseZ);
}

void calcCameraGyroPose(const al::CameraPoser* cameraPoser, sead::Vector3f* gyroPoseX,
                        sead::Vector3f* gyroPoseY, sead::Vector3f* gyroPoseZ) {
    getCameraInput(cameraPoser)->calcGyroPose(gyroPoseX, gyroPoseY, gyroPoseZ);
}

const sead::Vector3f& getGyroFront(al::CameraPoser* cameraPoser) {
    return cameraPoser->getGyroCtrl()->getFront();
}

f32 getGyroAngleV(al::CameraPoser* cameraPoser) {
    return cameraPoser->getGyroCtrl()->getAngleV();
}

f32 getGyroAngleH(al::CameraPoser* cameraPoser) {
    return cameraPoser->getGyroCtrl()->getAngleH();
}

void setGyroLimitAngleV(al::CameraPoser* cameraPoser, f32 min, f32 max) {
    cameraPoser->getGyroCtrl()->setLimitAngleV(min, max);
}

void setGyroSensitivity(al::CameraPoser* cameraPoser, f32 min, f32 max) {
    cameraPoser->getGyroCtrl()->setSensitivity(min, max);
}

void reduceGyroSencitivity(al::CameraPoser* cameraPoser) {
    cameraPoser->getGyroCtrl()->reduceSensitivity();
}

void stopUpdateGyro(al::CameraPoser* cameraPoser) {
    cameraPoser->getPoserFag()->isStopUpdateGyro = true;
}

void restartUpdateGyro(al::CameraPoser* cameraPoser) {
    cameraPoser->getPoserFag()->isStopUpdateGyro = false;
}

bool isStopUpdateGyro(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getPoserFag()->isStopUpdateGyro;
}

bool isTargetCollideGround(const al::CameraPoser* cameraPoser) {
    return getTarget(cameraPoser)->isCollideGround();
}

bool isTargetInWater(const al::CameraPoser* cameraPoser) {
    return getTarget(cameraPoser)->isInWater();
}

bool isTargetInMoonGravity(const al::CameraPoser* cameraPoser) {
    return getTarget(cameraPoser)->isInMoonGravity();
}

bool isTargetClimbPole(const al::CameraPoser* cameraPoser) {
    return getTarget(cameraPoser)->isClimbPole();
}

bool isTargetGrabCeil(const al::CameraPoser* cameraPoser) {
    return getTarget(cameraPoser)->isGrabCeil();
}

bool isTargetInvalidMoveByInput(const al::CameraPoser* cameraPoser) {
    return getTarget(cameraPoser)->isInvalidMoveByInput();
}

bool isTargetEnableEndAfterInterpole(const al::CameraPoser* cameraPoser) {
    return tryGetTarget(cameraPoser) && getTarget(cameraPoser)->isEnableEndAfterInterpole();
}

bool isTargetWallCatch(const al::CameraPoser* cameraPoser) {
    return getTarget(cameraPoser)->isWallCatch();
}

bool isSnapShotMode(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getFlagCtrl()->isSnapShotModeRunning;
}

void initSnapShotCameraCtrl(al::CameraPoser* cameraPoser) {
    al::SnapShotCameraSceneInfo* sceneInfo = cameraPoser->getSceneInfo()->snapShotCameraSceneInfo;
    al::SnapShotCameraCtrl* snapShotCtrl = new al::SnapShotCameraCtrl(sceneInfo);
    cameraPoser->setSnapShotCtrl(snapShotCtrl);
}

void initSnapShotCameraCtrlZoomAutoReset(al::CameraPoser* cameraPoser) {
    al::SnapShotCameraSceneInfo* sceneInfo = cameraPoser->getSceneInfo()->snapShotCameraSceneInfo;
    al::SnapShotCameraCtrl* snapShotCtrl = new al::SnapShotCameraCtrl(sceneInfo);
    snapShotCtrl->setIsValidZoomFovy(true);
    snapShotCtrl->set_68(true);
    cameraPoser->setSnapShotCtrl(snapShotCtrl);
}

void initSnapShotCameraCtrlZoomRollMove(al::CameraPoser* cameraPoser) {
    al::SnapShotCameraSceneInfo* sceneInfo = cameraPoser->getSceneInfo()->snapShotCameraSceneInfo;
    al::SnapShotCameraCtrl* snapShotCtrl = new al::SnapShotCameraCtrl(sceneInfo);
    snapShotCtrl->setIsValidZoomFovy(true);
    snapShotCtrl->setIsValidRoll(true);
    snapShotCtrl->setIsValidLookAtOffset(true);
    cameraPoser->setSnapShotCtrl(snapShotCtrl);
}

void validateSnapShotCameraLookAtOffset(al::CameraPoser* cameraPoser) {
    cameraPoser->getSnapShotCtrl()->setIsValidLookAtOffset(true);
}

void validateSnapShotCameraZoomFovy(al::CameraPoser* cameraPoser) {
    cameraPoser->getSnapShotCtrl()->setIsValidZoomFovy(true);
}

void validateSnapShotCameraRoll(al::CameraPoser* cameraPoser) {
    cameraPoser->getSnapShotCtrl()->setIsValidRoll(true);
}

void updateSnapShotCameraCtrl(al::CameraPoser* cameraPoser) {
    sead::LookAtCamera camera;
    cameraPoser->makeLookAtCameraPrev(&camera);

    cameraPoser->getSnapShotCtrl()->update(camera, cameraPoser, getCameraInput(cameraPoser));
}

void startResetSnapShotCameraCtrl(al::CameraPoser* cameraPoser, s32 value) {
    cameraPoser->getSnapShotCtrl()->startReset(value);
}

void setSnapShotMaxZoomOutFovyDegree(al::CameraPoser* cameraPoser, f32 value) {
    cameraPoser->getSnapShotCtrl()->setMaxZoomOutFovyDegree(value);
}

f32 getSnapShotRollDegree(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getSnapShotCtrl()->getRollDegree();
}

const sead::Vector3f& getSnapShotLookAtOffset(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getSnapShotCtrl()->getLookAtOffset();
}

bool isOffVerticalAbsorb(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getPoserFag()->isOffVerticalAbsorb;
}

void onVerticalAbsorb(al::CameraPoser* cameraPoser) {
    cameraPoser->getPoserFag()->isOffVerticalAbsorb = false;
}

void offVerticalAbsorb(al::CameraPoser* cameraPoser) {
    cameraPoser->getPoserFag()->isOffVerticalAbsorb = true;
}

void invalidateCameraBlur(al::CameraPoser* cameraPoser) {
    cameraPoser->getPoserFag()->isInvalidCameraBlur = true;
}

bool isRequestStopVerticalAbsorb(const al::CameraObjectRequestInfo& requestInfo) {
    return requestInfo.isStopVerticalAbsorb;
}

bool isRequestResetPosition(const al::CameraObjectRequestInfo& requestInfo) {
    return requestInfo.isResetPosition;
}

bool isRequestResetAngleV(const al::CameraObjectRequestInfo& requestInfo) {
    return requestInfo.isResetAngleV;
}

bool isRequestDownToDefaultAngleBySpeed(const al::CameraObjectRequestInfo& requestInfo) {
    return requestInfo.isDownToDefaultAngleBySpeed;
}

bool isRequestUpToTargetAngleBySpeed(const al::CameraObjectRequestInfo& requestInfo) {
    return requestInfo.isUpToTargetAngleBySpeed;
}

f32 getRequestTargetAngleV(const al::CameraObjectRequestInfo& requestInfo) {
    return requestInfo.targetAngleV;
}

f32 getRequestAngleSpeed(const al::CameraObjectRequestInfo& requestInfo) {
    return requestInfo.angleSpeed;
}

bool isRequestMoveDownAngleV(const al::CameraObjectRequestInfo& requestInfo) {
    return requestInfo.isMoveDownAngle;
}

bool isRequestSetAngleV(const al::CameraObjectRequestInfo& requestInfo) {
    return requestInfo.isSetAngleV;
}

f32 getRequestAngleV(const al::CameraObjectRequestInfo& requestInfo) {
    return requestInfo.angleV;
}

bool isInvalidCollider(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getPoserFag()->isInvalidCollider;
}

void validateCollider(al::CameraPoser* cameraPoser) {
    cameraPoser->getPoserFag()->isInvalidCollider = false;
}

void invalidateCollider(al::CameraPoser* cameraPoser) {
    cameraPoser->getPoserFag()->isInvalidCollider = true;
}

void validateCtrlSubjective(al::CameraPoser* cameraPoser) {
    cameraPoser->getPoserFag()->isValidCtrlSubjective = true;
}

void invalidateChangeSubjective(al::CameraPoser* cameraPoser) {
    cameraPoser->getPoserFag()->isInvalidChangeSubjective = true;
}

void invalidateKeepDistanceNextCamera(al::CameraPoser* cameraPoser) {
    cameraPoser->getPoserFag()->isInvalidKeepDistanceNextCamera = true;
}

void invalidateKeepDistanceNextCameraIfNoCollide(al::CameraPoser* cameraPoser) {
    cameraPoser->getPoserFag()->isInvalidKeepDistanceNextCameraIfNoCollide = true;
}

void invalidatePreCameraEndAfterInterpole(al::CameraPoser* cameraPoser) {
    cameraPoser->getPoserFag()->isInvalidPreCameraEndAfterInterpole = true;
}

bool isInvalidPreCameraEndAfterInterpole(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getPoserFag()->isInvalidPreCameraEndAfterInterpole;
}

bool isSceneCameraFirstCalc(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getViewInfo()->isFirstCalc();
}

bool isActiveInterpole(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getViewInfo()->isActiveInterpole();
}

bool isInvalidEndEntranceCamera(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getFlagCtrl()->isInvalidEndEntranceCamera;
}

bool isPause(const al::CameraPoser* cameraPoser) {
    return false;
}

bool checkFirstCameraCollisionArrow(sead::Vector3f* outHitPos, sead::Vector3f* outNormal,
                                    const al::IUseCollision* collision, const sead::Vector3f& pos,
                                    const sead::Vector3f& dir) {
    CameraCollisionHitResult result;
    if (!checkFirstCameraCollisionArrow(&result, collision, pos, dir))
        return false;

    if (outHitPos)
        outHitPos->set(result.hitPos);
    if (outNormal)
        outNormal->set(result.normal);

    return true;
}

bool checkFirstCameraCollisionArrow(CameraCollisionHitResult* outResult,
                                    const al::IUseCollision* collision, const sead::Vector3f& pos,
                                    const sead::Vector3f& dir) {
    const al::ArrowHitInfo* hitInfo = nullptr;

    if (!alCollisionUtil::getFirstPolyOnArrow(collision, &hitInfo, pos, dir, &sPartsFiler,
                                              &sTriangleFilter)) {
        return false;
    }

    outResult->hitPos.set(hitInfo->hitInfo->collisionHitPos);
    outResult->normal.set(hitInfo->hitInfo->triangle.getNormal(0));

    s32 type = 2;
    if (hitInfo->hitInfo->isCollisionAtFace())
        type = 0;
    else if (hitInfo->hitInfo->isCollisionAtEdge())
        type = 1;

    outResult->type = type;

    return true;
}

bool checkFirstCameraCollisionArrowOnlyCeiling(sead::Vector3f* outHitPos, sead::Vector3f* outNormal,
                                               const al::IUseCollision* collision,
                                               const sead::Vector3f& pos,
                                               const sead::Vector3f& dir) {
    al::Triangle triangle;
    sead::Vector3f hitPos = {0.0f, 0.0f, 0.0f};
    if (!alCollisionUtil::getFirstPolyOnArrow(collision, &hitPos, &triangle, pos, dir, nullptr,
                                              &sCeilFilter)) {
        return false;
    }

    if (outHitPos)
        outHitPos->set(hitPos);

    if (outNormal)
        outNormal->set(triangle.getNormal(0));
    return true;
}

f32 calcZoneRotateAngleH(f32 angle, const al::CameraPoser* cameraPoser) {
    return calcZoneRotateAngleH(angle, cameraPoser->getViewMtx());
}

f32 calcZoneRotateAngleH(f32 angle, const sead::Matrix34f& mtx) {
    f32 sin = sead::Mathf::sin(sead::Mathf::deg2rad(angle));
    f32 cos = sead::Mathf::cos(sead::Mathf::deg2rad(angle));

    f32 rotateAngle =
        sead::Mathf::atan2(sin * mtx.m[0][0] + 0.0f * mtx.m[0][1] + cos * mtx.m[0][2],
                           sin * mtx.m[2][0] + 0.0f * mtx.m[2][1] + cos * mtx.m[2][2]);

    return sead::Mathf::rad2deg(rotateAngle);
}

f32 calcZoneInvRotateAngleH(f32 angle, const sead::Matrix34f& mtx) {
    f32 sin = sead::Mathf::sin(sead::Mathf::deg2rad(angle));
    f32 cos = sead::Mathf::cos(sead::Mathf::deg2rad(angle));

    f32 rotateAngle =
        sead::Mathf::atan2(sin * mtx.m[0][0] + 0.0f * mtx.m[1][0] + cos * mtx.m[2][0],
                           sin * mtx.m[0][2] + 0.0f * mtx.m[1][2] + cos * mtx.m[2][2]);

    return sead::Mathf::rad2deg(rotateAngle);
}

void multVecZone(sead::Vector3f* outVec, const sead::Vector3f& vec,
                 const al::CameraPoser* cameraPoser) {
    outVec->setMul(cameraPoser->getViewMtx(), vec);
}

void multVecInvZone(sead::Vector3f* outVec, const sead::Vector3f& vec,
                    const al::CameraPoser* cameraPoser) {
    sead::Matrix34f mtx = sead::Matrix34f::ident;
    mtx.setInverse(cameraPoser->getViewMtx());
    outVec->setMul(mtx, vec);
}

void rotateVecZone(sead::Vector3f* outVec, const sead::Vector3f& vec,
                   const al::CameraPoser* cameraPoser) {
    outVec->setRotated(cameraPoser->getViewMtx(), vec);
}

bool makeCameraKeepInFrameV(sead::LookAtCamera* camera, const sead::Vector3f& vec,
                            const al::CameraPoser* cameraPoser, f32 a, f32 b) {
    sead::Vector3f offset = {0.0f, 0.0f, 0.0f};
    if (calcOffsetCameraKeepInFrameV(&offset, camera, vec, cameraPoser, a, b)) {
        camera->setAt(camera->getAt() + offset);
        camera->setPos(camera->getPos() + offset);
        return true;
    }
    return false;
}

void initCameraRail(al::CameraPoser* cameraPoser, const al::PlacementInfo& info, const char* name) {
    al::PlacementInfo placementInfo;
    al::tryGetLinksInfo(&placementInfo, info, name);
    cameraPoser->initRail(placementInfo);
}

bool tryGetCameraRailArg(f32* outArg, const al::PlacementInfo& info, const char* argName,
                         const char* name) {
    al::PlacementInfo placementInfo;
    if (!al::tryGetLinksInfo(&placementInfo, info, name))
        return false;

    return al::tryGetArg(outArg, placementInfo, argName);
}

const char* getCameraRailPointObjId(const al::CameraPoser* cameraPoser, s32 index) {
    al::PlacementInfo* railPointInfo = al::getRailPointInfo(cameraPoser, index);

    al::PlacementId placementId;
    placementId.init(*railPointInfo);
    return placementId.getId();
}

al::CameraLimitRailKeeper* tryFindNearestLimitRailKeeper(const al::CameraPoser* cameraPoser,
                                                         const sead::Vector3f& pos) {
    f32 minDistance = -1.0f;
    al::CameraLimitRailKeeper* nearestRailKeeper = nullptr;

    for (s32 i = 0; i < cameraPoser->getSceneInfo()->railHolderNum; i++) {
        for (s32 j = 0; j < cameraPoser->getSceneInfo()->railHolders[i]->getRailCount(); j++) {
            al::CameraLimitRailKeeper* railKeeper =
                cameraPoser->getSceneInfo()->railHolders[i]->getRail(j);

            sead::Vector3f nearestPos = {0.0f, 0.0f, 0.0f};
            railKeeper->calcNearestRailPos(&nearestPos, pos);
            f32 distance = (nearestPos - pos).length();

            if (railKeeper->getActivateDistance() < distance)
                continue;

            if (minDistance < 0.0f || distance < minDistance) {
                minDistance = distance;
                nearestRailKeeper = railKeeper;
            }
        }
    }

    return nearestRailKeeper;
}

void calcCameraRotateStick(sead::Vector2f* outStick, const al::CameraPoser* cameraPoser) {
    getCameraInput(cameraPoser)->calcInputStick(outStick);

    if (cameraPoser->getFlagCtrl()->isCameraReverseInputH)
        outStick->x = -outStick->x;
    if (cameraPoser->getFlagCtrl()->isCameraReverseInputV)
        outStick->y = -outStick->y;
}

f32 calcCameraRotateStickH(const al::CameraPoser* cameraPoser) {
    sead::Vector2f stick = {0.0f, 0.0f};
    calcCameraRotateStick(&stick, cameraPoser);
    return stick.x;
}

f32 calcCameraRotateStickV(const al::CameraPoser* cameraPoser) {
    sead::Vector2f stick = {0.0f, 0.0f};
    calcCameraRotateStick(&stick, cameraPoser);
    return stick.y;
}

f32 calcCameraRotateStickPower(const al::CameraPoser* cameraPoser) {
    sead::Vector2f stick = {0.0f, 0.0f};
    calcCameraRotateStick(&stick, cameraPoser);
    return stick.length();
}

s32 getStickSensitivityLevel(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getSceneInfo()->requestParamHolder->getStickSensitivityLevel();
}

f32 getStickSensitivityScale(const al::CameraPoser* cameraPoser) {
    switch (getStickSensitivityLevel(cameraPoser)) {
    case -2:
        return 0.44f;
    case -1:
        return 0.72f;
    case 0:
        return 1.6f;
    case 1:
        return 1.27f;
    case 2:
        return 1.55f;
    default:
        return 1.0f;
    }
}

bool isValidGyro(const al::CameraPoser* cameraPoser) {
    return !cameraPoser->getFlagCtrl()->isInvalidCameraGyro;
}

s32 getGyroSensitivityLevel(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getSceneInfo()->requestParamHolder->getGyroSensitivityLevel();
}

f32 getGyroSensitivityScale(const al::CameraPoser* cameraPoser) {
    switch (getGyroSensitivityLevel(cameraPoser)) {
    case -1:
        return 0.625f;
    case 0:
        return 1.0f;
    case 1:
        return 1.6f;
    default:
        return 1.0f;
    }
}

bool isTriggerCameraResetRotate(const al::CameraPoser* cameraPoser) {
    return !isSnapShotMode(cameraPoser) && getCameraInput(cameraPoser)->isTriggerReset();
}

bool isHoldCameraZoom(const al::CameraPoser* cameraPoser) {
    return getCameraInput(cameraPoser)->isHoldZoom();
}

bool isHoldCameraSnapShotZoomIn(const al::CameraPoser* cameraPoser) {
    return getCameraInput(cameraPoser)->isHoldSnapShotZoomIn();
}

bool isHoldCameraSnapShotZoomOut(const al::CameraPoser* cameraPoser) {
    return getCameraInput(cameraPoser)->isHoldSnapShotZoomOut();
}

bool isHoldCameraSnapShotRollLeft(const al::CameraPoser* cameraPoser) {
    return getCameraInput(cameraPoser)->isHoldSnapShotRollLeft();
}

bool isHoldCameraSnapShotRollRight(const al::CameraPoser* cameraPoser) {
    return getCameraInput(cameraPoser)->isHoldSnapShotRollRight();
}

bool tryCalcCameraSnapShotMoveStick(sead::Vector2f* outStick, const al::CameraPoser* cameraPoser) {
    return getCameraInput(cameraPoser)->tryCalcSnapShotMoveStick(outStick);
}

bool isPlayerTypeFlyer(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getSceneInfo()->requestParamHolder->isPlayerTypeFlyer();
}

bool isPlayerTypeHighSpeedMove(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getSceneInfo()->requestParamHolder->isPlayerTypeHighSpeedMove();
}

bool isPlayerTypeHighJump(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getSceneInfo()->requestParamHolder->isPlayerTypeHighJump();
}

bool isPlayerTypeNotTouchGround(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getSceneInfo()->requestParamHolder->isPlayerTypeNotTouchGround();
}

bool isOnRideObj(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getSceneInfo()->requestParamHolder->isOnRideObj();
}

}  // namespace alCameraPoserFunction
