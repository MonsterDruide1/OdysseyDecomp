#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class SwitchDitherMapParts : public LiveActor {
public:
    SwitchDitherMapParts(const char* name);

    void init(const ActorInitInfo& info) override;

    void ditherOn();
    void ditherOff();

    void exeWait();
};

static_assert(sizeof(SwitchDitherMapParts) == 0x108);
}  // namespace al
