#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {

class ModelDrawParts : public LiveActor {
public:
    ModelDrawParts(const char*, const LiveActor*, const ActorInitInfo&, const char*);
};

static_assert(sizeof(ModelDrawParts) == 0x108, "ModelDrawParts Size");

}  // namespace al
