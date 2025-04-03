#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class FlowMapParts;

class FlowMapCtrl {
public:
    friend FlowMapParts;

    FlowMapCtrl(al::LiveActor* parentActor);
    void update();

private:
    al::LiveActor* mParentActor;
    s32 _8 = 0;
    s32 mInterval = 60;
    f32 mSpeed = 1.0f;
};
}  // namespace al
