#pragma once

#include "Library/Anim/DitherAnimator.h"

namespace al {
class LiveActor;
class Resource;

class FarDistanceDitherAnimator : public DitherAnimator {
public:
    static FarDistanceDitherAnimator* tryCreate(LiveActor*, const Resource*, const char*);
};

}  // namespace al
