#pragma once

#include <math/seadMatrix.h>

#include "Library/LiveActor/LiveActor.h"

class GiantWanderBoss : public al::LiveActor {
public:
    const sead::Matrix34f* getBulletMtx() const;
};
