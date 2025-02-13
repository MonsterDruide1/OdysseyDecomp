#include "Library/Camera/CameraPoserActorRailParallel.h"

#include "Library/Camera/CameraPoserFunction.h"
#include "Library/Yaml/ByamlUtil.h"

namespace al {

void CameraPoserActorRailParallel::init() {
    alCameraPoserFunction::initCameraMoveLimit(this);
}

void CameraPoserActorRailParallel::loadParam(const ByamlIter& iter) {
    tryGetByamlV3f(&mOffset, iter, "Offset");
    tryGetByamlF32(&mDistance, iter, "Distance");
    tryGetByamlF32(&mAngleDegreeH, iter, "AngleDegreeH");
    tryGetByamlF32(&mAngleDegreeV, iter, "AngleDegreeV");
    tryGetByamlF32(&mFollowRate, iter, "FollowRate");
}
}  // namespace al
