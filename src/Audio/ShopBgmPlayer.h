#pragma once

#include "Library/LiveActor/LiveActor.h"

class ShopBgmPlayer : public al::LiveActor {
public:
    using al::LiveActor::LiveActor;

    void movement() override;
};
