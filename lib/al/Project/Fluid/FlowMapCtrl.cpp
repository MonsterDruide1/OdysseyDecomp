#include "Project/Fluid/FlowMapCtrl.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/Math/MathUtil.h"

namespace al {

FlowMapCtrl::FlowMapCtrl(al::LiveActor* parentActor) : mParentActor(parentActor) {
    al::setMaterialProgrammable(parentActor);
}

// NON_MATCHING
void FlowMapCtrl::update() {
    s32 interval = mInterval;

    sead::Vector4f materialParameters;
    materialParameters.x = mSpeed;
    materialParameters.y =
        al::calcRate01(interval / 2 - sead::Mathi::abs(interval / 2 - _8), 0.0f, interval / 2);
    materialParameters.z = al::calcRate01(_8, 0.0f, mInterval);
    materialParameters.w =
        al::calcRate01(al::modi(_8 + interval / 2 + mInterval, mInterval), 0.0f, mInterval);

    s32 materialCount = al::getMaterialCount(mParentActor);
    for (s32 i = 0; i < materialCount; i++) {
        const char* materialName = al::getMaterialName(mParentActor, i);
        al::setModelMaterialParameterV4F(mParentActor, materialName, "flow0_param",
                                         materialParameters);
    }

    s32 prevInterval = mInterval;
    s32 v8 = _8++ + 1;
    _8 = al::modi(prevInterval + v8, prevInterval);
}

}  // namespace al
