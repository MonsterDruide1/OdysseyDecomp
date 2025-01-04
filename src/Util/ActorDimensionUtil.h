#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
}  // namespace al

class IUseDimension;

namespace rs {

void snap2DParallelizeFront(al::LiveActor*, const IUseDimension*, f32);

}  // namespace rs
