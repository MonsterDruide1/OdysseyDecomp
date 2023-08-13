#pragma once

#include <math/seadVector.h>

namespace al {
class IUseCamera;
class Projection;

Projection getProjection(const IUseCamera*, int);

sead::Vector3f* getCameraPos(const IUseCamera*, int);
void calcCameraUpDir(sead::Vector3f*, const IUseCamera*, int);

}  // namespace al
