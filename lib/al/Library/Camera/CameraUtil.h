#pragma once

#include <math/seadVector.h>

namespace al {
// TODO: Add functions.
class IUseCamera;
class Projection;

Projection getProjection(const IUseCamera*, s32);

sead::Vector3f* getCameraPos(const IUseCamera*, s32);
void calcCameraUpDir(sead::Vector3f*, const IUseCamera*, s32);
s32 getViewNumMax(const IUseCamera* cameraHolder);
}  // namespace al
