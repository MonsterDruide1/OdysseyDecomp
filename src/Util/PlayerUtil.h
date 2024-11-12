#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
}

namespace rs {

const sead::Vector3f& getPlayerPos(const al::LiveActor*);
bool isPlayerHack(const al::LiveActor*);

}  // namespace rs
