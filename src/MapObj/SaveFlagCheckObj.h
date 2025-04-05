#pragma once

#include "Library/LiveActor/LiveActor.h"

class SaveFlagCheckObj : public al::LiveActor {
public:
    SaveFlagCheckObj(const char* name);
    void init(const al::ActorInitInfo& info) override;
};
