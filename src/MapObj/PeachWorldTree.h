#pragma once

#include "Library/LiveActor/LiveActor.h"

class PeachWorldTree : public al::LiveActor {
public:
    PeachWorldTree(const char* name);

    virtual void init(const al::ActorInitInfo& info) override;
    virtual void control() override;
};
