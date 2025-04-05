#pragma once

#include <basis/seadTypes.h>

// TODO: Someone should add this to nnheaders!
namespace nn::g3d {
class MaterialObj;
}

// TODO: Someone should add this to sead!
namespace sead {
class GraphicsContext;
}

namespace al {
class Scene;

f32 getDepthClearValue();

void setDepthFuncNearDraw(sead::GraphicsContext* context);
void setDepthFuncFarDraw(sead::GraphicsContext* context);
void setDepthFuncNearDraw(sead::GraphicsContext* context);
void setDepthFuncFarDraw(sead::GraphicsContext* context);
bool getAlphaTestEnable(nn::g3d::MaterialObj* material);
}  // namespace al

namespace alGraphicsFunction {
void requestUpdateMaterialInfo(al::Scene*);
void validateGpuStressAnalyzer(al::Scene*);
void invalidateGpuStressAnalyzer(al::Scene*);
}  // namespace alGraphicsFunction
