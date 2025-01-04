#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class AreaObjGroup;
class FluidSurfaceHolder;
class LiveActor;

class NatureDirector {
public:
    NatureDirector();
    void init();

    void setWaterAreaGroup(AreaObjGroup* waterAreaGroup) { mWaterAreaGroup = waterAreaGroup; }

private:
    AreaObjGroup* mWaterAreaGroup;
    FluidSurfaceHolder* mFluidSurfaceHolder;
};

static_assert(sizeof(NatureDirector) == 0x10);

bool isInWater(const LiveActor* actor);
void calcFindWaterSurface(sead::Vector3f*, sead::Vector3f*, const LiveActor*, const sead::Vector3f&,
                          const sead::Vector3f&, f32);

}  // namespace al
