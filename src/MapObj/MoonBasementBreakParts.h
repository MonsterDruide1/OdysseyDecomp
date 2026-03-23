#pragma once

#include "Library/LiveActor/LiveActor.h"

class MoonBasementBreakParts : public al::LiveActor {
public:
    MoonBasementBreakParts(const char* actorName);

    void init(const al::ActorInitInfo& info) override;
    void appear() override;
    void kill() override;

    void exeWait();
    void exeBreak();
};
