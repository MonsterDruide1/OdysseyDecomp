#pragma once

#include "Library/LiveActor/LiveActor.h"

class LavaPan : public al::LiveActor {
public:
    LavaPan(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void exeWait();
    void exeRumble();
};
