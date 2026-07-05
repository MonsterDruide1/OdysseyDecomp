#include "Library/Camera/CameraPoserFunction.h"

#include "Library/Base/StringUtil.h"
#include "Library/Camera/CameraArrowCollider.h"
#include "Library/Camera/CameraOffsetCtrlPreset.h"
#include "Library/Camera/CameraPoser.h"
#include "Library/Camera/CameraPoserFix.h"
#include "Library/Camera/CameraPoserSceneInfo.h"
#include "Library/Camera/CameraRequestParamHolder.h"
#include "Library/Camera/CameraStartInfo.h"
#include "Library/Camera/CameraSubTargetBase.h"
#include "Library/Camera/CameraTargetBase.h"
#include "Library/Camera/CameraTargetCollideInfoHolder.h"
#include "Library/Camera/CameraTargetHolder.h"
#include "Library/Camera/CameraViewInfo.h"
#include "Library/Math/MathUtil.h"
#include "Library/Play/Camera/CameraVerticalAbsorber.h"
#include "Library/Projection/Projection.h"
#include "Project/Camera/CameraSubTargetTurnParam.h"

namespace alCameraPoserFunction {

s32 getViewIndex(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getCameraViewInfo()->getIndex();
}

static s32 getViewInfoIndex(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getViewInfo()->getIndex();
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

void calcCameraPose(sead::Quatf* out, const al::CameraPoser* cameraPoser) {
    sead::Vector3f lookDir;
    calcLookDir(&lookDir, cameraPoser);
    al::makeQuatFrontUp(out, lookDir, cameraPoser->getUp());
}

void calcLookDir(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    calcCameraDir(out, cameraPoser);
    out->negate();
}

void calcCameraDir(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    out->setSub(cameraPoser->getEye(), cameraPoser->getAt());
    al::normalize(out);
}

bool calcCameraDirH(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    out->setSub(cameraPoser->getEye(), cameraPoser->getAt());
    al::verticalizeVec(out, cameraPoser->getUp(), *out);
    return al::tryNormalizeOrZero(out);
}

bool calcLookDirH(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    bool isValid = calcCameraDirH(out, cameraPoser);
    out->negate();
    return isValid;
}

void calcSideDir(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    sead::Vector3f vec = cameraPoser->getEye() - cameraPoser->getAt();
    al::normalize(&vec);
    out->set(vec.cross(cameraPoser->getUp()));
    al::normalize(out);
}

void calcPreCameraDir(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    const sead::LookAtCamera& lookAtCam = getLookAtCamera(cameraPoser);
    out->setSub(lookAtCam.getPos(), lookAtCam.getAt());
    al::normalize(out);
}

void calcPreCameraDirH(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    const sead::LookAtCamera& lookAtCam = getLookAtCamera(cameraPoser);
    out->setSub(lookAtCam.getPos(), lookAtCam.getAt());
    al::verticalizeVec(out, sead::Vector3f::ey, *out);
    al::tryNormalizeOrZero(out);
}

void calcPreLookDir(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    calcPreCameraDir(out, cameraPoser);
    out->negate();
}

void calcPreLookDirH(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    const sead::LookAtCamera& lookAtCam = getLookAtCamera(cameraPoser);
    out->set(lookAtCam.getAt() - lookAtCam.getPos());
    al::verticalizeVec(out, getPreUpDir(cameraPoser), *out);
    al::tryNormalizeOrZero(out);
}

f32 calcPreCameraAngleH(const al::CameraPoser* cameraPoser) {
    sead::Vector3f vec = {0.0f, 0.0f, 0.0f};
    calcPreCameraDirH(&vec, cameraPoser);
    return sead::Mathf::rad2deg(sead::Mathf::atan2(vec.x, vec.z));
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

static inline al::CameraTargetBase* getTarget(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getTargetHolder()->getViewTarget(getViewInfoIndex(cameraPoser));
}

void calcTargetTrans(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    getTarget(cameraPoser)->calcTrans(out);
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

void calcTargetTransWithOffset(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    calcTargetTrans(out, cameraPoser);
    al::CameraOffsetCtrlPreset* cameraOffsetCtrlPreset = cameraPoser->getCameraOffsetCtrlPreset();
    if (cameraOffsetCtrlPreset)
        out->add(cameraOffsetCtrlPreset->getOffset());
}

void calcTargetVelocity(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    getTarget(cameraPoser)->calcVelocity(out);
}

void calcTargetVelocityH(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    calcTargetVelocity(out, cameraPoser);
    sead::Vector3f vec = sead::Vector3f::ey;
    calcTargetUp(&vec, cameraPoser);
    al::verticalizeVec(out, vec, *out);
}

void calcTargetUp(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    getTarget(cameraPoser)->calcUp(out);
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

void calcTargetPose(sead::Quatf* out, const al::CameraPoser* cameraPoser) {
    sead::Vector3f targetUp = {0.0f, 0.0f, 0.0f};
    sead::Vector3f targetFront = {0.0f, 0.0f, 0.0f};
    calcTargetUp(&targetUp, cameraPoser);
    calcTargetFront(&targetFront, cameraPoser);
    al::makeQuatFrontUp(out, targetFront, targetUp);
}

void calcTargetFront(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    getTarget(cameraPoser)->calcFront(out);
}

void calcTargetSide(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    getTarget(cameraPoser)->calcSide(out);
}

void calcTargetGravity(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    getTarget(cameraPoser)->calcGravity(out);
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

bool tryGetTargetRequestDistance(f32* out, const al::CameraPoser* cameraPoser) {
    al::CameraTargetBase* target = getTarget(cameraPoser);

    if (target->getRequestDistance() > 0.0f) {
        *out = target->getRequestDistance();
        return true;
    }

    return false;
}

f32* tryGetBossDistanceCurve(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getCameraPoserSceneInfo()->requestParamHolder->get_60();
}

f32* tryGetEquipmentDistanceCurve(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getCameraPoserSceneInfo()->requestParamHolder->get_58();
}

static inline al::CameraTargetCollideInfoHolder*
getTargetCollision(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getCameraPoserSceneInfo()->targetCollideInfoHolder;
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

bool tryCalcSlopeCollisionDownFrontDirH(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    return getTargetCollision(cameraPoser)->tryCalcSlopDownFrontDirH(out);
}

f32 getSlopeCollisionUpSpeed(const al::CameraPoser* cameraPoser) {
    return getTargetCollision(cameraPoser)->getSlopeCollisionUpSpeed();
}

f32 getSlopeCollisionDownSpeed(const al::CameraPoser* cameraPoser) {
    return getTargetCollision(cameraPoser)->getSlopeCollisionDownSpeed();
}

bool isExistSubTarget(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getTargetHolder()->getTopSubTargetInline();
}

static inline al::CameraSubTargetBase* getTopSubTarget(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getTargetHolder()->getTopSubTarget();
}

static inline al::CameraSubTargetTurnParam*
getSubTargetTurnParam(const al::CameraPoser* cameraPoser) {
    return getTopSubTarget(cameraPoser)->getSubTargetTurnParam();
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

void calcSubTargetBack(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    calcSubTargetFront(out, cameraPoser);
    out->negate();
}

void calcSubTargetTrans(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    getTopSubTarget(cameraPoser)->calcTrans(out);
}

bool isChangeSubTarget(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getTargetHolder()->hasTopSubTargetChanged();
}

void calcSubTargetFront(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    getTopSubTarget(cameraPoser)->calcFront(out);
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

bool tryCalcSubTargetTurnBrakeDistanceRate(f32* out, const al::CameraPoser* cameraPoser) {
    al::CameraSubTargetTurnParam* turnParam = getSubTargetTurnParam(cameraPoser);

    sead::Vector3f vec = {0.0f, 0.0f, 0.0f};
    sead::Vector3f vec2 = {0.0f, 0.0f, 0.0f};
    calcTargetTrans(&vec, cameraPoser);
    calcSubTargetTrans(&vec2, cameraPoser);

    f32 distance = sead::Mathf::sqrt(sead::Mathf::pow(vec.x - vec2.x, 2) +
                                     sead::Mathf::pow(vec.z - vec2.z, 2));

    if (turnParam->turnBrakeEndDistance > 0.0f &&
        turnParam->turnBrakeEndDistance < turnParam->turnBrakeStartDistance &&
        distance < turnParam->turnBrakeStartDistance) {
        *out = 1.0f - al::normalize(distance, turnParam->turnBrakeEndDistance,
                                    turnParam->turnBrakeStartDistance);
        return true;
    }

    if (turnParam->turnStopStartDistance > 0.0f &&
        turnParam->turnStopStartDistance < turnParam->turnStopEndDistance &&
        turnParam->turnStopStartDistance < distance) {
        *out = al::normalize(distance, turnParam->turnStopStartDistance,
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

void clampAngleSubTargetTurnRangeV(f32* out, const al::CameraPoser* cameraPoser) {
    al::CameraSubTargetBase* topSubTarget = getTopSubTarget(cameraPoser);
    f32 min = topSubTarget->getSubTargetTurnParam()->minTurnDegreeV;
    f32 max = topSubTarget->getSubTargetTurnParam()->maxTurnDegreeV;

    *out = sead::Mathf::clamp(*out, min, max);
}

void initCameraVerticalAbsorber(al::CameraPoser* cameraPoser) {
    cameraPoser->setVerticalAbsorber(new al::CameraVerticalAbsorber(cameraPoser, false));
}

void initCameraVerticalAbsorberNoCameraPosAbsorb(al::CameraPoser* cameraPoser) {
    cameraPoser->setVerticalAbsorber(new al::CameraVerticalAbsorber(cameraPoser, true));
}

static inline al::CameraVerticalAbsorber* getVerticalAbsorber(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getCameraVerticalAbsorber();
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

}  // namespace alCameraPoserFunction
