#include "Project/Fluid/FlowMapCtrl.h"

#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/Math/MathUtil.h"

namespace al {

FlowMapCtrl::FlowMapCtrl(LiveActor* parentActor) : mParent(parentActor) {
    setMaterialProgrammable(parentActor);
}

void FlowMapCtrl::update() {
    s32 halfInterval = mInterval / 2;

    sead::Vector4f flowParameters;
    flowParameters.x = mSpeed;
    flowParameters.y =
        calcRate01(halfInterval - sead::Mathi::abs(halfInterval - mFlowStep), 0.0f, halfInterval);
    flowParameters.z = calcRate01(mFlowStep, 0.0f, mInterval);
    flowParameters.w =
        calcRate01(modi(mFlowStep + halfInterval + mInterval, mInterval), 0.0f, mInterval);

    s32 materialCount = getMaterialCount(mParent);
    for (s32 i = 0; i < materialCount; i++) {
        const char* materialName = getMaterialName(mParent, i);
        setModelMaterialParameterV4F(mParent, materialName, "flow0_param", flowParameters);
    }

    mFlowStep++;
    mFlowStep = modi(mFlowStep + mInterval, mInterval);
}

}  // namespace al
