#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

class SequentialSwitch : public al::LiveActor {
public:
    SequentialSwitch(const char* name);

    void init(const al::ActorInitInfo& info) override;

    void notifyInputSwitchOn();

private:
    s32 mInputSwitchOnCount = 0;
};

static_assert(sizeof(SequentialSwitch) == 0x110);
