#pragma once

#include "Library/LiveActor/LiveActor.h"
#include "Library/LiveActor/ActorInitInfo.h"

class Coin : public al::LiveActor {
public:
    void init(al::ActorInitInfo const&);

    void control(void);
};
