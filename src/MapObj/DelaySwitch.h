#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
}  // namespace al

class DelaySwitch : public al::LiveActor {
public:
    DelaySwitch(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void appear() override;

    void exeRun();

private:
    s32 mDelayFrame = 1;
};

static_assert(sizeof(DelaySwitch) == 0x110);
