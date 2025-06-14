#include "Library/Camera/CameraPoserFunction.h"

#include "Library/Base/StringUtil.h"
#include "Library/Camera/CameraPoser.h"
#include "Library/Camera/CameraPoserFix.h"
#include "Library/Camera/CameraStartInfo.h"
#include "Library/Camera/CameraTargetBase.h"
#include "Library/Camera/CameraTargetHolder.h"
#include "Library/Camera/CameraViewInfo.h"
#include "Library/Math/MathUtil.h"
#include "Library/Projection/Projection.h"

namespace alCameraPoserFunction {

inline al::CameraViewInfo* getViewInfo(const al::CameraPoser* poser) {
    return poser->getViewInfo();
}

s32 getViewIndex(const al::CameraPoser* poser) {
    return getViewInfo(poser)->getIndex();
}

const sead::LookAtCamera& getLookAtCamera(const al::CameraPoser* poser) {
    return getViewInfo(poser)->getLookAtCam();
}

const sead::Projection& getProjectionSead(const al::CameraPoser* poser) {
    return getViewInfo(poser)->getProjectionSead();
}

const al::Projection* getProjection(const al::CameraPoser* poser) {
    return &getViewInfo(poser)->getProjection();
}

const sead::Matrix44f& getProjectionMtx(const al::CameraPoser* poser) {
    return *getViewInfo(poser)->getProjMtx();
}

f32 getNear(const al::CameraPoser* poser) {
    return getViewInfo(poser)->getNear();
}

f32 getFar(const al::CameraPoser* poser) {
    return getViewInfo(poser)->getFar();
}

f32 getAspect(const al::CameraPoser* poser) {
    return getViewInfo(poser)->getAspect();
}

const sead::Vector3f& getPreCameraPos(const al::CameraPoser* poser) {
    return getLookAtCamera(poser).getPos();
}

const sead::Vector3f& getPreLookAtPos(const al::CameraPoser* poser) {
    return getLookAtCamera(poser).getAt();
}

const sead::Vector3f& getPreUpDir(const al::CameraPoser* poser) {
    return getLookAtCamera(poser).getUp();
}

f32 getPreFovyDegree(const al::CameraPoser* poser) {
    return getPreFovyRadian(poser) * 57.296f;
}

f32 getPreFovyRadian(const al::CameraPoser* poser) {
    return getProjection(poser)->getFovy();
}

bool isPrePriorityDemo(const al::CameraStartInfo& startInfo) {
    return startInfo.prePriorityType == al::CameraTicket::Priority_Demo;
}

bool isPrePriorityDemo2(const al::CameraStartInfo& startInfo) {
    return startInfo.prePriorityType == al::CameraTicket::Priority_Demo2;
}

bool isPrePriorityDemoTalk(const al::CameraStartInfo& startInfo) {
    return startInfo.prePriorityType == al::CameraTicket::Priority_DemoTalk;
}

bool isPrePriorityDemoAll(const al::CameraStartInfo& startInfo) {
    return (startInfo.prePriorityType - al::CameraTicket::Priority_Demo) < 2;
}

bool isPrePriorityEntranceAll(const al::CameraStartInfo& startInfo) {
    return (startInfo.prePriorityType == al::CameraTicket::Priority_Entrance) |
           (startInfo.prePriorityType == al::CameraTicket::Priority_Unknown);
}

bool isPrePriorityPlayer(const al::CameraStartInfo& startInfo) {
    return startInfo.prePriorityType == al::CameraTicket::Priority_Player;
}

bool isEqualPreCameraName(const al::CameraStartInfo& startInfo, const char* compareName) {
    const char* name = startInfo.preCameraName;
    return name && al::isEqualString(compareName, name);
}

bool isPreCameraFixAbsolute(const al::CameraStartInfo& startInfo) {
    return isEqualPreCameraName(startInfo, al::CameraPoserFix::getFixAbsoluteCameraName());
}

bool isInvalidCollidePreCamera(const al::CameraStartInfo& startInfo) {
    return startInfo.isInvalidCollidePreCamera;
}

bool isInvalidKeepPreCameraDistance(const al::CameraStartInfo& startInfo) {
    return startInfo.isInvalidKeepPreCameraDistance;
}

bool isInvalidKeepPreCameraDistanceIfNoCollide(const al::CameraStartInfo& startInfo) {
    return startInfo.isInvalidKeepPreCameraDistanceIfNoCollide;
}

bool isValidResetPreCameraPose(const al::CameraStartInfo& startInfo) {
    return startInfo.isValidResetPreCameraPose;
}

bool isValidKeepPreSelfCameraPose(const al::CameraStartInfo& startInfo) {
    return startInfo.isValidKeepPreSelfCameraPose;
}

f32 getPreCameraSwingAngleH(const al::CameraStartInfo& startInfo) {
    return startInfo.preCameraSwingAngleH;
}

f32 getPreCameraSwingAngleV(const al::CameraStartInfo& startInfo) {
    return startInfo.preCameraSwingAngleV;
}

f32 getPreCameraMaxSwingAngleH(const al::CameraStartInfo& startInfo) {
    return startInfo.preCameraMaxSwingAngleH;
}

f32 getPreCameraMaxSwingAngleV(const al::CameraStartInfo& startInfo) {
    return startInfo.preCameraMaxSwingAngleV;
}

bool isExistAreaAngleH(const al::CameraStartInfo& startInfo) {
    return startInfo.isExistAreaAngleH;
}

bool isExistAreaAngleV(const al::CameraStartInfo& startInfo) {
    return startInfo.isExistAreaAngleV;
}

f32 getAreaAngleH(const al::CameraStartInfo& startInfo) {
    return startInfo.areaAngleH;
}

f32 getAreaAngleV(const al::CameraStartInfo& startInfo) {
    return startInfo.areaAngleV;
}

bool isExistNextPoseByPreCamera(const al::CameraStartInfo& startInfo) {
    return startInfo.isExistNextPoseByPreCamera;
}

f32 getNextAngleHByPreCamera(const al::CameraStartInfo& startInfo) {
    return startInfo.nextAngleHByPreCamera;
}

f32 getNextAngleVByPreCamera(const al::CameraStartInfo& startInfo) {
    return startInfo.nextAngleVByPreCamera;
}

void calcLookDir(sead::Vector3f* vec, const al::CameraPoser* poser) {
    calcCameraDir(vec, poser);
    vec->negate();
}

void calcCameraPose(sead::Quatf* quat, const al::CameraPoser* poser) {
    sead::Vector3f vec;
    calcLookDir(&vec, poser);
    al::makeQuatFrontUp(quat, vec, poser->getCameraUp());
}

void calcCameraDir(sead::Vector3f* vec, const al::CameraPoser* poser) {
    vec->x = poser->getPosition().x - poser->getTargetTrans().x;
    vec->y = poser->getPosition().y - poser->getTargetTrans().y;
    vec->z = poser->getPosition().z - poser->getTargetTrans().z;
    al::normalize(vec);
}

void calcCameraDirH(sead::Vector3f* vec, const al::CameraPoser* poser) {
    vec->x = poser->getPosition().x - poser->getTargetTrans().x;
    vec->y = poser->getPosition().y - poser->getTargetTrans().y;
    vec->z = poser->getPosition().z - poser->getTargetTrans().z;
    al::verticalizeVec(vec, poser->getCameraUp(), *vec);
    al::tryNormalizeOrZero(vec);
}

void calcPreCameraDir(sead::Vector3f* vec, const al::CameraPoser* poser) {
    const sead::LookAtCamera* cam = &getLookAtCamera(poser);
    vec->x = cam->getPos().x - cam->getAt().x;
    vec->y = cam->getPos().y - cam->getAt().y;
    vec->z = cam->getPos().z - cam->getAt().z;
    al::normalize(vec);
}

void calcPreCameraDirH(sead::Vector3f* vec, const al::CameraPoser* poser) {
    const sead::LookAtCamera* cam = &getLookAtCamera(poser);
    vec->x = cam->getPos().x - cam->getAt().x;
    vec->y = cam->getPos().y - cam->getAt().y;
    vec->z = cam->getPos().z - cam->getAt().z;
    al::verticalizeVec(vec, sead::Vector3f::ey, *vec);
    al::tryNormalizeOrZero(vec);
}

void calcPreLookDir(sead::Vector3f* vec, const al::CameraPoser* poser) {
    calcPreCameraDir(vec, poser);
    vec->negate();
}

void calcPreLookDirH(sead::Vector3f* vec, const al::CameraPoser* poser) {
    const sead::LookAtCamera* cam = &getLookAtCamera(poser);
    vec->set(cam->getAt() - cam->getPos());
    al::verticalizeVec(vec, getLookAtCamera(poser).getUp(), *vec);
    al::tryNormalizeOrZero(vec);
}

f32 calcPreCameraAngleH(const al::CameraPoser* poser) {
    sead::Vector3f vec = {0, 0, 0};
    calcPreCameraDirH(&vec, poser);
    return atan2f(vec.x, vec.z) * 57.296f;
}

f32 calcPreCameraAngleV(const al::CameraPoser* poser) {
    sead::Vector3f vec = {0, 0, 0};
    calcPreCameraDir(&vec, poser);
    al::parallelizeVec(&vec, sead::Vector3f::ey, vec);
    return asinf(vec.length()) * 57.296f;
}

void calcTargetTrans(sead::Vector3f* vec, const al::CameraPoser* poser) {
    poser->getTargetHolder()->getViewTarget(getViewIndex(poser))->calcTrans(vec);
}

void setLookAtPosToTarget(al::CameraPoser* poser) {
    sead::Vector3f vec = {0, 0, 0};
    poser->getTargetHolder()->getViewTarget(getViewIndex(poser))->calcTrans(&vec);
    poser->setTargetTrans(vec);
}

void setLookAtPosToTargetAddOffset(al::CameraPoser* poser, const sead::Vector3f& offset) {
    sead::Vector3f vec = {0, 0, 0};
    poser->getTargetHolder()->getViewTarget(getViewIndex(poser))->calcTrans(&vec);
    poser->setTargetTrans(vec + offset);
}

void setCameraPosToTarget(al::CameraPoser* poser) {
    sead::Vector3f vec = {0, 0, 0};
    poser->getTargetHolder()->getViewTarget(getViewIndex(poser))->calcTrans(&vec);
    poser->setPosition(vec);
}

void setCameraPosToTargetWithOffset(al::CameraPoser* poser, const sead::Vector3f& offset) {
    sead::Vector3f vec = {0, 0, 0};
    poser->getTargetHolder()->getViewTarget(getViewIndex(poser))->calcTrans(&vec);
    poser->setPosition(vec + offset);
}

void calcTargetVelocity(sead::Vector3f* vec, const al::CameraPoser* poser) {
    poser->getTargetHolder()->getViewTarget(getViewIndex(poser))->calcVelocity(vec);
}

void calcTargetVelocityH(sead::Vector3f* vec, const al::CameraPoser* poser) {
    calcTargetVelocity(vec, poser);
    sead::Vector3f vec2 = {1.0f, 0.0f, 0.0f};
    calcTargetUp(&vec2, poser);
    al::verticalizeVec(vec, vec2, *vec);
}

void calcTargetUp(sead::Vector3f* vec, const al::CameraPoser* poser) {
    poser->getTargetHolder()->getViewTarget(getViewIndex(poser))->calcUp(vec);
}
}  // namespace alCameraPoserFunction
