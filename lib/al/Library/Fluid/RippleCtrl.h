#pragma once

#include "Library/Fluid/IUseFluidSurface.h"

namespace al {
class LiveActor;

// TODO: Finish this
class RippleCtrl : public IUseFluidSurface {
public:
    RippleCtrl(LiveActor* parent);

    static RippleCtrl* tryCreate(LiveActor* parent);

    void init(const ActorInitInfo& info);

private:
    unsigned char padding[0x120];
};
}  // namespace al
