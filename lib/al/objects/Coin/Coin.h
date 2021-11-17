#pragma once

#include "al/LiveActor/LiveActor.h"
#include "al/actor/ActorInitInfo.h"

class Coin : public al::LiveActor {
public:
    void init(al::ActorInitInfo const&);

    void control(void);
};