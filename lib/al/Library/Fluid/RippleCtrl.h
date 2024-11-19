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
    void update();

private:
    unsigned char padding[0x128 - sizeof(IUseFluidSurface)];
};
}  // namespace al
