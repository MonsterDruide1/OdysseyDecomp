#pragma once

#include <basis/seadTypes.h>
#include "math/seadMatrix.h"

namespace al {
class LiveActor;
}

class PlayerFunction {
public:
    static u32 getPlayerInputPort(const al::LiveActor*);
    static bool isPlayerDeadStatus(const al::LiveActor*);
    static sead::Matrix34f* getPlayerViewMtx(al::LiveActor const*);

};
