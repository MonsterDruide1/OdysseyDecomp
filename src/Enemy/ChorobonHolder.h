#pragma once

#include "Library/LiveActor/LiveActor.h"

class ChorobonHolder : public al::LiveActor {
public:
    ChorobonHolder(const char*);

    void init(const al::ActorInitInfo&) override;
    void control() override;
    void appear() override;

private:
    char filler_108[0x30];
};

static_assert(sizeof(ChorobonHolder) == 0x138);
