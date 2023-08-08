#pragma once

#include "al/Library/LiveActor/LiveActor.h"
#include "al/Library/LiveActor/ActorInitInfo.h"

class Coin : public al::LiveActor {
public:
    void init(al::ActorInitInfo const&);

    void control(void);
};
