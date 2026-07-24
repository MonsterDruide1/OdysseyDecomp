#pragma once

#include <basis/seadTypes.h>

#include "Library/Nerve/NerveExecutor.h"

namespace al {
class LiveActor;
}  // namespace al

class KoopaCapScaleAnimator : public al::NerveExecutor {
public:
    KoopaCapScaleAnimator(al::LiveActor* actor);

    void update(al::LiveActor* actor);
    f32 getScale() const;
    void startWaitHoverScaling();
    void reset();
    void exeStop();
    void exeWaitHoverScaling();
};

static_assert(sizeof(KoopaCapScaleAnimator) == 0x10);
