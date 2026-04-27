#pragma once

#include <basis/seadTypes.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
}

struct FastenerVertex {
    s32 calcValidEdgeNum() const;
    s32 calcValidEdgeNumFastener() const;

    u8 _0[0x14];
    sead::Vector3f trans;
    u8 _20[0x24b];
    bool _26b;
    u8 _26c[0x14];
    u32 _280;
    u8 _284[0x4];
};

static_assert(sizeof(FastenerVertex) == 0x288);

namespace FastenerFunction {
bool tryMakeQuatResetKnob(sead::Quatf* quat, const FastenerVertex* vertex);
void resetKnobPose(al::LiveActor* actor, const FastenerVertex* vertex);
}  // namespace FastenerFunction
