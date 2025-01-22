#pragma once

#include "Library/LiveActor/LiveActor.h"

class PeachWorldTree : public al::LiveActor {
public:
    PeachWorldTree(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void control() override;
};
