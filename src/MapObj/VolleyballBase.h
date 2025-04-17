#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
}

class VolleyballBase : public al::LiveActor {
public:
    VolleyballBase(const char* name);
    void init(const al::ActorInitInfo& initInfo) override;
    void startReaction();

    void exeWait();
    void exeReaction();
};

static_assert(sizeof(VolleyballBase) == 0x108);
