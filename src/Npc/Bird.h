#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
}  // namespace al

class Bird : public al::LiveActor {
public:
    Bird(const char* name);
    void initGlideOff(const sead::Matrix34f*, const sead::Vector3f&, bool);
    static Bird* createBirdGlideDownUfo(const char*, const al::ActorInitInfo&);
    void tryStartGlideDown();
    void tryStartFlyAway();

private:
    void* _padding[0x19];
};

static_assert(sizeof(Bird) == 0x1d0);
