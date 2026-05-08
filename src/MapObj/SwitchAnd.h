#pragma once

#include "Library/LiveActor/LiveActor.h"

class SwitchAnd : public al::LiveActor {
public:
    SwitchAnd(const char* name);

    void init(const al::ActorInitInfo& info) override;

    void notifyInputSwitchOn();
    void notifyInputSwitchOff();

private:
    s32 mInputSwitchNum = 0;
};

static_assert(sizeof(SwitchAnd) == 0x110);
