#pragma once

#include "Library/LiveActor/LiveActor.h"

class ChorobonHolder : public al::LiveActor {
public:
    ChorobonHolder(const char*);

    void init(const al::ActorInitInfo&) override;
    void control() override;
    void appear() override;

private:
    char _108[0x138 - sizeof(al::LiveActor)];
};
