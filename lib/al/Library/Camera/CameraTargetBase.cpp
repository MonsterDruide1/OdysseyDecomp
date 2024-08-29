#include "Library/Camera/CameraTargetBase.h"

namespace al {

CameraTargetBase::CameraTargetBase() {}

void CameraTargetBase::calcSide(sead::Vector3f* side) const {}

void CameraTargetBase::calcUp(sead::Vector3f* up) const {}

void CameraTargetBase::calcFront(sead::Vector3f* front) const {}

void CameraTargetBase::calcVelocity(sead::Vector3f* velocity) const {}

}  // namespace al
