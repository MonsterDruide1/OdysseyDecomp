#include "Library/Camera/CameraPoserFunction.h"

#include "CameraOffsetCtrlPreset.h"
#include "CameraPoserFix.h"
#include "CameraPoserSceneInfo.h"
#include "CameraRequestParamHolder.h"
#include "CameraStartInfo.h"
#include "CameraSubTargetBase.h"
#include "CameraTargetBase.h"
#include "CameraTargetCollideInfoHolder.h"
#include "CameraTargetHolder.h"
#include "CameraViewInfo.h"
#include "Library/Base/StringUtil.h"
#include "Library/Camera/CameraPoser.h"
#include "Library/Math/MathUtil.h"
#include "Library/Projection/Projection.h"
#include "Library/Play/Camera/CameraVerticalAbsorber.h"
#include "Library/Camera/CameraArrowCollider.h"
#include "Project/Camera/CameraSubTargetTurnParam.h"

namespace alCameraPoserFunction {

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
    return cameraPoser->getCameraViewInfo()->getLookAtCam().getPos();
}

const sead::Vector3f& getPreLookAtPos(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getCameraViewInfo()->getLookAtCam().getAt();
}

const sead::Vector3f& getPreUpDir(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getCameraViewInfo()->getLookAtCam().getUp();
}

f32 getPreFovyDegree(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getCameraViewInfo()->getProjection().getFovy() * 57.29578f;
}

f32 getPreFovyRadian(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getCameraViewInfo()->getProjection().getFovy();
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
    return cameraStartInfo.prePriorityType == al::CameraTicket::Priority_Demo ||
           cameraStartInfo.prePriorityType == al::CameraTicket::Priority_Demo2;
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
    const char* cameraName = al::CameraPoserFix::getFixAbsoluteCameraName();
    if (cameraStartInfo.preCameraName)
        return al::isEqualString(cameraName, cameraStartInfo.preCameraName);
    return false;
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
    sead::Vector3f vec;
    vec.x = cameraPoser->getPosition().x - cameraPoser->getTargetTrans().x;
    vec.y = cameraPoser->getPosition().y - cameraPoser->getTargetTrans().y;
    vec.z = cameraPoser->getPosition().z - cameraPoser->getTargetTrans().z;

    al::normalize(&vec);
    vec.negate();
    al::makeQuatFrontUp(out, vec, cameraPoser->getCameraUp());
}

void calcLookDir(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    out->x = cameraPoser->getPosition().x - cameraPoser->getTargetTrans().x;
    out->y = cameraPoser->getPosition().y - cameraPoser->getTargetTrans().y;
    out->z = cameraPoser->getPosition().z - cameraPoser->getTargetTrans().z;
    al::normalize(out);
    out->negate();
}

void calcCameraDir(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    out->x = cameraPoser->getPosition().x - cameraPoser->getTargetTrans().x;
    out->y = cameraPoser->getPosition().y - cameraPoser->getTargetTrans().y;
    out->z = cameraPoser->getPosition().z - cameraPoser->getTargetTrans().z;
    al::normalize(out);
}

void calcCameraDirH(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    out->x = cameraPoser->getPosition().x - cameraPoser->getTargetTrans().x;
    out->y = cameraPoser->getPosition().y - cameraPoser->getTargetTrans().y;
    out->z = cameraPoser->getPosition().z - cameraPoser->getTargetTrans().z;
    al::verticalizeVec(out, cameraPoser->getCameraUp(), *out);
    al::tryNormalizeOrZero(out);
}

bool calcLookDirH(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    out->x = cameraPoser->getPosition().x - cameraPoser->getTargetTrans().x;
    out->y = cameraPoser->getPosition().y - cameraPoser->getTargetTrans().y;
    out->z = cameraPoser->getPosition().z - cameraPoser->getTargetTrans().z;
    al::verticalizeVec(out, cameraPoser->getCameraUp(), *out);
    bool ret = al::tryNormalizeOrZero(out);
    out->negate();
    return ret;
}

void calcSideDir(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    sead::Vector3f vec = cameraPoser->getPosition() - cameraPoser->getTargetTrans();
    al::normalize(&vec);
    *out = vec.cross(cameraPoser->getCameraUp());
    al::normalize(out);
}

void calcPreCameraDir(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    const sead::LookAtCamera& lookAtCam = cameraPoser->getCameraViewInfo()->getLookAtCam();
    out->x = lookAtCam.getPos().x - lookAtCam.getAt().x;
    out->y = lookAtCam.getPos().y - lookAtCam.getAt().y;
    out->z = lookAtCam.getPos().z - lookAtCam.getAt().z;
    al::normalize(out);
}

void calcPreCameraDirH(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    const sead::LookAtCamera& lookAtCam = cameraPoser->getCameraViewInfo()->getLookAtCam();
    out->x = lookAtCam.getPos().x - lookAtCam.getAt().x;
    out->y = lookAtCam.getPos().y - lookAtCam.getAt().y;
    out->z = lookAtCam.getPos().z - lookAtCam.getAt().z;
    al::verticalizeVec(out, sead::Vector3f::ey, *out);
    al::tryNormalizeOrZero(out);
}

void calcPreLookDir(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    const sead::LookAtCamera& lookAtCam = cameraPoser->getCameraViewInfo()->getLookAtCam();
    out->x = lookAtCam.getPos().x - lookAtCam.getAt().x;
    out->y = lookAtCam.getPos().y - lookAtCam.getAt().y;
    out->z = lookAtCam.getPos().z - lookAtCam.getAt().z;
    al::normalize(out);
    out->negate();
}

void calcPreLookDirH(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    const sead::LookAtCamera& lookAtCam = cameraPoser->getCameraViewInfo()->getLookAtCam();
    out->set(lookAtCam.getAt() - lookAtCam.getPos());
    al::verticalizeVec(out, cameraPoser->getCameraViewInfo()->getLookAtCam().getUp(), *out);
    al::tryNormalizeOrZero(out);
}

f32 calcPreCameraAngleH(const al::CameraPoser* cameraPoser) {
    sead::Vector3f vec = {0, 0, 0};
    calcPreCameraDirH(&vec, cameraPoser);
    return sead::Mathf::atan2(vec.x, vec.z) * 57.29578f;
}

f32 calcPreCameraAngleV(const al::CameraPoser* cameraPoser) {
    sead::Vector3f vec = {0, 0, 0};
    calcPreCameraDir(&vec, cameraPoser);
    al::parallelizeVec(&vec, sead::Vector3f::ey, vec);
    return sead::Mathf::asin(sead::Mathf::sqrt(vec.dot(vec))) * 57.29578f;
}

void setLookAtPosToTarget(al::CameraPoser* cameraPoser) {
    sead::Vector3f vec = {0, 0, 0};
    calcTargetTrans(&vec, cameraPoser);
    cameraPoser->setTargetTrans(vec);
}

void calcTargetTrans(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    al::CameraTargetHolder* cameraTargetHolder = cameraPoser->getTargetHolder();
    al::CameraTargetBase* cameraTargetBase =
        cameraTargetHolder->getViewTarget(cameraPoser->getViewInfo()->getIndex());
    cameraTargetBase->calcTrans(out);
}

void setLookAtPosToTargetAddOffset(al::CameraPoser* cameraPoser, const sead::Vector3f& offset) {
    sead::Vector3f vec = {0, 0, 0};
    calcTargetTrans(&vec, cameraPoser);
    cameraPoser->setTargetTrans(vec + offset);
}

void setCameraPosToTarget(al::CameraPoser* cameraPoser) {
    sead::Vector3f vec = {0, 0, 0};
    calcTargetTrans(&vec, cameraPoser);
    cameraPoser->setPosition(vec);
}

void setCameraPosToTargetAddOffset(al::CameraPoser* cameraPoser, const sead::Vector3f& offset) {
    sead::Vector3f vec = {0, 0, 0};
    calcTargetTrans(&vec, cameraPoser);
    cameraPoser->setPosition(vec + offset);
}

void calcTargetTransWithOffset(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    calcTargetTrans(out, cameraPoser);
    al::CameraOffsetCtrlPreset* cameraOffsetCtrlPreset = cameraPoser->getCameraOffsetCtrlPreset();
    if (cameraOffsetCtrlPreset)
        out->add(cameraOffsetCtrlPreset->getOffset());
}

void calcTargetVelocity(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    al::CameraTargetHolder* cameraTargetHolder = cameraPoser->getTargetHolder();
    al::CameraTargetBase* cameraTargetBase =
        cameraTargetHolder->getViewTarget(cameraPoser->getViewInfo()->getIndex());
    cameraTargetBase->calcVelocity(out);
}

void calcTargetVelocityH(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    calcTargetVelocity(out, cameraPoser);
    sead::Vector3f vec = sead::Vector3f::ey;
    calcTargetUp(&vec, cameraPoser);
    al::verticalizeVec(out, vec, *out);
}

void calcTargetUp(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    al::CameraTargetHolder* cameraTargetHolder = cameraPoser->getTargetHolder();
    al::CameraTargetBase* cameraTargetBase =
        cameraTargetHolder->getViewTarget(cameraPoser->getViewInfo()->getIndex());
    cameraTargetBase->calcUp(out);
}

f32 calcTargetSpeedV(const al::CameraPoser* cameraPoser) {
    sead::Vector3f vec = {0, 0, 0};
    sead::Vector3f vec2 = {0, 0, 0};
    calcTargetVelocity(&vec, cameraPoser);
    calcTargetUp(&vec2, cameraPoser);
    al::parallelizeVec(&vec, vec2, vec);

    f32 dot = vec.dot(vec2);
    f32 sqrtDot = sead::Mathf::sqrt(vec.dot(vec));
    if (!(dot > 0.0f))
        sqrtDot = -sqrtDot;
    return sqrtDot;
}

void calcTargetPose(sead::Quatf* out, const al::CameraPoser* cameraPoser) {
    sead::Vector3f vec = {0, 0, 0};
    sead::Vector3f vec2 = {0, 0, 0};
    calcTargetUp(&vec, cameraPoser);
    calcTargetFront(&vec2, cameraPoser);
    al::makeQuatFrontUp(out, vec2, vec);
}

void calcTargetFront(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    al::CameraTargetHolder* cameraTargetHolder = cameraPoser->getTargetHolder();
    al::CameraTargetBase* cameraTargetBase =
        cameraTargetHolder->getViewTarget(cameraPoser->getViewInfo()->getIndex());
    cameraTargetBase->calcFront(out);
}

void calcTargetSide(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    al::CameraTargetHolder* cameraTargetHolder = cameraPoser->getTargetHolder();
    al::CameraTargetBase* cameraTargetBase =
        cameraTargetHolder->getViewTarget(cameraPoser->getViewInfo()->getIndex());
    cameraTargetBase->calcSide(out);
}

void calcTargetGravity(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    al::CameraTargetHolder* cameraTargetHolder = cameraPoser->getTargetHolder();
    al::CameraTargetBase* cameraTargetBase =
        cameraTargetHolder->getViewTarget(cameraPoser->getViewInfo()->getIndex());
    cameraTargetBase->calcGravity(out);
}

f32 calcTargetSpeedH(const al::CameraPoser* cameraPoser) {
    sead::Vector3f vec = {0, 0, 0};
    sead::Vector3f vec2 = {0, 0, 0};
    calcTargetGravity(&vec, cameraPoser);
    calcTargetVelocity(&vec2, cameraPoser);
    al::verticalizeVec(&vec2, vec, vec2);
    return sead::Mathf::sqrt(vec2.dot(vec2));
}

f32 calcTargetJumpSpeed(const al::CameraPoser* cameraPoser) {
    sead::Vector3f vec = {0, 0, 0};
    sead::Vector3f vec2 = {0, 0, 0};
    calcTargetGravity(&vec, cameraPoser);
    calcTargetVelocity(&vec2, cameraPoser);
    al::parallelizeVec(&vec2, vec, vec2);
    if (!al::isNearZero(vec2, 0.001f) && !(vec.dot(vec2) > 0.0f))
        return sead::Mathf::sqrt(vec2.dot(vec2));
    return 0.0f;
}

f32 calcTargetFallSpeed(const al::CameraPoser* cameraPoser) {
    sead::Vector3f vec = {0, 0, 0};
    sead::Vector3f vec2 = {0, 0, 0};
    calcTargetGravity(&vec, cameraPoser);
    calcTargetVelocity(&vec2, cameraPoser);
    al::parallelizeVec(&vec2, vec, vec2);
    if (!al::isNearZero(vec2, 0.001f) && !(vec.dot(vec2) < 0.0f))
        return sead::Mathf::sqrt(vec2.dot(vec2));
    return 0.0f;
}

bool isChangeTarget(const al::CameraPoser* cameraPoser) {
    al::CameraTargetHolder* cameraTargetHolder = cameraPoser->getTargetHolder();
    return cameraTargetHolder->isChangeViewTarget(cameraPoser->getViewInfo()->getIndex());
}

bool tryGetTargetRequestDistance(f32* out, const al::CameraPoser* cameraPoser) {
    al::CameraTargetHolder* cameraTargetHolder = cameraPoser->getTargetHolder();
    al::CameraTargetBase* cameraTargetBase =
        cameraTargetHolder->getViewTarget(cameraPoser->getViewInfo()->getIndex());
    f32 requestDistance = cameraTargetBase->getRequestDistance();
    if (requestDistance > 0.0) {
        *out = cameraTargetBase->getRequestDistance();
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

bool isExistCollisionUnderTarget(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getCameraPoserSceneInfo()
        ->targetCollideInfoHolder->isExistCollisionUnderTarget();
}

const sead::Vector3f& getUnderTargetCollisionPos(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getCameraPoserSceneInfo()->targetCollideInfoHolder->getTargetCollisionPos();
}

const sead::Vector3f& getUnderTargetCollisionNormal(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getCameraPoserSceneInfo()
        ->targetCollideInfoHolder->getTargetCollisionNormal();
}

bool isExistSlopeCollisionUnderTarget(const al::CameraPoser* cameraPoser) {
    al::CameraTargetCollideInfoHolder* cameraTargetCollideInfoHolder =
        cameraPoser->getCameraPoserSceneInfo()->targetCollideInfoHolder;
    if (cameraTargetCollideInfoHolder->isExistCollisionUnderTarget())
        return cameraTargetCollideInfoHolder->isExistSlopeCollisionUnderTarget();
    return false;
}

bool isExistWallCollisionUnderTarget(const al::CameraPoser* cameraPoser) {
    al::CameraTargetCollideInfoHolder* cameraTargetCollideInfoHolder =
        cameraPoser->getCameraPoserSceneInfo()->targetCollideInfoHolder;
    return cameraTargetCollideInfoHolder->isExistUnderWall();
}

bool tryCalcSlopeCollisionDownFrontDirH(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    al::CameraTargetCollideInfoHolder* cameraTargetCollideInfoHolder =
        cameraPoser->getCameraPoserSceneInfo()->targetCollideInfoHolder;
    return cameraTargetCollideInfoHolder->tryCalcSlopDownFrontDirH(out);
}

f32 getSlopeCollisionUpSpeed(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getCameraPoserSceneInfo()
        ->targetCollideInfoHolder->getSlopeCollisionUpSpeed();
}

f32 getSlopeCollisionDownSpeed(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getCameraPoserSceneInfo()
        ->targetCollideInfoHolder->getSlopeCollisionDownSpeed();
}

bool isExistSubTarget(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getTargetHolder()->getSubTarget();
}

bool checkValidTurnToSubTarget(const al::CameraPoser* cameraPoser) {
    if (!cameraPoser->getTargetHolder()->getSubTarget())
        return false;
    al::CameraTargetHolder* cameraTargetHolder = cameraPoser->getTargetHolder();
    al::CameraSubTargetBase* topSubTarget = cameraTargetHolder->getTopSubTarget();
    al::CameraSubTargetTurnParam* subTargetTurnParam = topSubTarget->getSubTargetTurnParam();
    if (subTargetTurnParam->getValidTurnDegreeRangeH() < 0.0f &&
        subTargetTurnParam->getValidFaceDegreeRangeH() != 0.0f) {
        return true;
    }
    sead::Vector3f vec = {};
    vec.x = cameraPoser->getPosition().x - cameraPoser->getTargetTrans().x;
    vec.y = cameraPoser->getPosition().y - cameraPoser->getTargetTrans().y;
    vec.z = cameraPoser->getPosition().z - cameraPoser->getTargetTrans().z;
    al::verticalizeVec(&vec, cameraPoser->getCameraUp(), vec);
    bool ret = al::tryNormalizeOrZero(&vec);
    vec.negate();
    if (!ret)
        return false;
    if (subTargetTurnParam->getValidFaceDegreeRangeH() >= 0.0f) {
        sead::Vector3f vec2 = {0,0,0};
        calcSubTargetBack(&vec2, cameraPoser);
        f32 validFaceDegreeRangeH = subTargetTurnParam->getValidFaceDegreeRangeH();
        f32 angleDegree = al::calcAngleDegree(vec2, vec);
        if (validFaceDegreeRangeH < angleDegree)
            return false;
    }
    if (subTargetTurnParam->getValidTurnDegreeRangeH() >= 0.0f) {
        sead::Vector3f vec2 = {0, 0, 0};
        sead::Vector3f vec3 = {0, 0, 0};
        calcSubTargetTrans(&vec2, cameraPoser);
        vec3 = vec2 - cameraPoser->getPosition();
        al::verticalizeVec(&vec3, cameraPoser->getCameraUp(), vec3);
        if (!al::tryNormalizeOrZero(&vec3))
            return false;
        if (subTargetTurnParam->getValidTurnDegreeRangeH() / 2 < al::calcAngleDegree(vec, vec3))
            return false;
    }
    return true;
}

void calcSubTargetBack(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    calcSubTargetFront(out, cameraPoser);
    out->negate();
}

void calcSubTargetTrans(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    al::CameraTargetHolder* targetHolder = cameraPoser->getTargetHolder();
    al::CameraSubTargetBase* subTargetBase = targetHolder->getTopSubTarget();
    subTargetBase->calcTrans(out);
}

bool isChangeSubTarget(const al::CameraPoser* cameraPoser) {
    al::CameraTargetHolder* targetHolder = cameraPoser->getTargetHolder();
    return targetHolder->get_30();
}

void calcSubTargetFront(sead::Vector3f* out, const al::CameraPoser* cameraPoser) {
    al::CameraTargetHolder* targetHolder = cameraPoser->getTargetHolder();
    al::CameraSubTargetBase* subTargetBase = targetHolder->getTopSubTarget();
    subTargetBase->calcFront(out);
}

f32 getSubTargetRequestDistance(const al::CameraPoser* cameraPoser) {
    al::CameraTargetHolder* targetHolder = cameraPoser->getTargetHolder();
    al::CameraSubTargetBase* subTargetBase = targetHolder->getTopSubTarget();
    return subTargetBase->getRequestDistance();
}

f32 getSubTargetTurnSpeedRate1(const al::CameraPoser* cameraPoser) {
    al::CameraTargetHolder* targetHolder = cameraPoser->getTargetHolder();
    al::CameraSubTargetBase* subTargetBase = targetHolder->getTopSubTarget();
    return subTargetBase->getSubTargetTurnParam()->getTurnSpeedRate1();
}

f32 getSubTargetTurnSpeedRate2(const al::CameraPoser* cameraPoser) {
    al::CameraTargetHolder* targetHolder = cameraPoser->getTargetHolder();
    al::CameraSubTargetBase* subTargetBase = targetHolder->getTopSubTarget();
    return subTargetBase->getSubTargetTurnParam()->getTurnSpeedRate2();
}

s32 getSubTargetTurnRestartStep(const al::CameraPoser* cameraPoser) {
    al::CameraTargetHolder* targetHolder = cameraPoser->getTargetHolder();
    al::CameraSubTargetBase* subTargetBase = targetHolder->getTopSubTarget();
    return subTargetBase->getSubTargetTurnParam()->getTargetTurnRestartStep();
}

bool tryCalcSubTargetTurnBrakeDistanceRate(f32* out, const al::CameraPoser* cameraPoser) {
    al::CameraSubTargetBase* subTargetBase = cameraPoser->getTargetHolder()->getTopSubTarget();
    al::CameraSubTargetTurnParam* turnParam = subTargetBase->getSubTargetTurnParam();

    sead::Vector3f vec = {0, 0, 0};
    sead::Vector3f vec2 = {0, 0, 0};
    calcTargetTrans(&vec, cameraPoser);
    calcSubTargetTrans(&vec2, cameraPoser);

    f32 distance = sead::Mathf::sqrt(sead::Mathf::pow(vec.x - vec2.x, 2) + sead::Mathf::pow(vec.z - vec2.z, 2));

    f32 turnBrakeDistance = turnParam->getTurnBrakeEndDistance();
    if (turnBrakeDistance > 0.0f) {
        f32 turnBrakeStartDistance = turnParam->getTurnBrakeStartDistance();
        if(turnBrakeDistance < turnBrakeStartDistance && distance < turnBrakeStartDistance) {
            *out = 1.0f - al::normalize(distance, turnBrakeDistance, turnBrakeStartDistance);
            return true;
        }
    }

    f32 turnStopStartDistance=turnParam->getTurnStopStartDistance();
    if (turnStopStartDistance > 0.0f) {
        f32 turnStopEndDistance = turnParam->getTurnStopEndDistance();
        if(turnStopStartDistance < turnStopEndDistance && turnStopStartDistance < distance) {
            *out = al::normalize(distance, turnStopStartDistance, turnStopEndDistance);
            return true;
        }
    }

    return false;
}

bool isValidSubTargetTurnV(const al::CameraPoser* cameraPoser) {
    al::CameraTargetHolder* targetHolder = cameraPoser->getTargetHolder();
    al::CameraSubTargetBase* subTargetBase = targetHolder->getTopSubTarget();
    return subTargetBase->getSubTargetTurnParam()->isTurnV();
}

bool isValidSubTargetResetAfterTurnV(const al::CameraPoser* cameraPoser) {
    al::CameraTargetHolder* targetHolder = cameraPoser->getTargetHolder();
    al::CameraSubTargetBase* subTargetBase = targetHolder->getTopSubTarget();
    return subTargetBase->getSubTargetTurnParam()->isResetAfterTurnV();
}

void clampAngleSubTargetTurnRangeV(f32* out, const al::CameraPoser* cameraPoser) {
    al::CameraTargetHolder* targetHolder = cameraPoser->getTargetHolder();
    al::CameraSubTargetBase* topSubTarget = targetHolder->getTopSubTarget();
    f32 minTurnDegree = topSubTarget->getSubTargetTurnParam()->getMinTurnDegreeV();
    f32 maxTurnDegree = 0.0f;

    f32 outParam = *out;
    if (!(outParam < minTurnDegree)) {
	maxTurnDegree = topSubTarget->getSubTargetTurnParam()->getMaxTurnDegreeV();
	minTurnDegree = outParam;
	if (outParam > maxTurnDegree)
	    minTurnDegree = maxTurnDegree;
    }
    *out = minTurnDegree;
}

void initCameraVerticalAbsorber(al::CameraPoser* cameraPoser) {
    al::CameraVerticalAbsorber* cameraVerticalAbsorber = new al::CameraVerticalAbsorber(cameraPoser, false);
    cameraPoser->setVerticalAbsorber(cameraVerticalAbsorber); 
}

void initCameraVerticalAbsorberNoCameraPosAbsorb(al::CameraPoser* cameraPoser) {
    al::CameraVerticalAbsorber* cameraVerticalAbsorber = new al::CameraVerticalAbsorber(cameraPoser, true);
    cameraPoser->setVerticalAbsorber(cameraVerticalAbsorber);
}

f32 getCameraVerticalAbsorbPosUp(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getCameraVerticalAbsorber()->getAbsorbScreenPosUp();
}

f32 getCameraVerticalAbsorbPosDown(const al::CameraPoser* cameraPoser) {
    return cameraPoser->getCameraVerticalAbsorber()->getAbsorbScreenPosDown();
}

void liberateVerticalAbsorb(al::CameraPoser* cameraPoser) {
    cameraPoser->getCameraVerticalAbsorber()->liberateAbsorb();
}

void stopUpdateVerticalAbsorb(al::CameraPoser* cameraPoser) {
    cameraPoser->getCameraVerticalAbsorber()->setIsStopUpdate(true);
}

void stopUpdateVerticalAbsorbForSnapShotMode(al::CameraPoser* cameraPoser, const sead::Vector3f& absorbVec) {
    stopUpdateVerticalAbsorb(cameraPoser);
    cameraPoser->getCameraVerticalAbsorber()->tryResetAbsorbVecIfInCollision(absorbVec);
}

void restartUpdateVerticalAbsorb(al::CameraPoser* cameraPoser) {
    cameraPoser->getCameraVerticalAbsorber()->setIsStopUpdate(false);
}

void validateVerticalAbsorbKeepInFrame(al::CameraPoser* cameraPoser) {
    cameraPoser->getCameraVerticalAbsorber()->setIsKeepInFrame(true);
}

void invalidateVerticalAbsorbKeepInFrame(al::CameraPoser* cameraPoser) {
    cameraPoser->getCameraVerticalAbsorber()->setIsKeepInFrame(false);
}

void setVerticalAbsorbKeepInFrameScreenOffsetUp(al::CameraPoser* cameraPoser, f32 keepInFrameOffsetUp) {
    cameraPoser->getCameraVerticalAbsorber()->setKeepInFrameOffsetUp(keepInFrameOffsetUp);
}

void setVerticalAbsorbKeepInFrameScreenOffsetDown(al::CameraPoser* cameraPoser, f32 keepInFrameOffsetDown) {
    cameraPoser->getCameraVerticalAbsorber()->setKeepInFrameOffsetDown(keepInFrameOffsetDown);
}

void initCameraArrowCollider(al::CameraPoser* cameraPoser) {
    al::CameraArrowCollider* cameraArrowCollider = new al::CameraArrowCollider(cameraPoser->getCollisionDirector());
    cameraPoser->initArrowCollider(cameraArrowCollider);
}

void initCameraArrowColliderWithoutThroughPassCollision(al::CameraPoser* cameraPoser) {
    al::CameraArrowCollider* cameraArrowCollider = new al::CameraArrowCollider(cameraPoser->getCollisionDirector());
    cameraArrowCollider->set_48c(true);
    cameraPoser->initArrowCollider(cameraArrowCollider);
}

}  // namespace alCameraPoserFunction
