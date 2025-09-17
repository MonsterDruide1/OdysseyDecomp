#pragma once

#include <math/seadMatrix.h>

namespace al {
class GraphicsSystemInfo;
class ModelKeeper;

bool isGreaterEqualMaxLodLevelNoClamp(const ModelKeeper* modelKeeper);
bool isLessMaxLodLevelNoClamp(const ModelKeeper* modelKeeper);
void setModelProjMtx0(const ModelKeeper*, const sead::Matrix44f&);
void forceApplyCubeMap(ModelKeeper*, const GraphicsSystemInfo*, const char*);

}  // namespace al
