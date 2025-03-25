#pragma once

#include <basis/seadTypes.h>

namespace al {
struct ActorParamMove {
public:
    f32 forceFront;
    f32 forceGravity;
    f32 decay;
    f32 turnDegrees;
};
}  // namespace al
