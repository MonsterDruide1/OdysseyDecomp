#pragma once

#include "Library/LiveActor/LiveActor.h"

class WorldMapEarth : public al::LiveActor {
public:
    WorldMapEarth(const char* name);
    void init(const al::ActorInitInfo& initInfo) override;
    void control() override;
};
