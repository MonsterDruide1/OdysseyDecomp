#pragma once

#include "Library/LiveActor/LiveActor.h"

class ChorobonHolder : public al::LiveActor {
public:
    ChorobonHolder(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void control() override;
    void appear() override;

private:
    char filler_108[0x30];
};

static_assert(sizeof(ChorobonHolder) == 0x138);
