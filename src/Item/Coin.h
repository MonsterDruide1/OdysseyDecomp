#pragma once

#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/LiveActor.h"

class Coin : public al::LiveActor {
public:
    void init(al::ActorInitInfo const&);

    void control(void);
};
