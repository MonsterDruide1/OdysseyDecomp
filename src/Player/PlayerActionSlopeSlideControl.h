#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
}
class PlayerConst;
class PlayerInput;
class IUsePlayerCollision;

class PlayerActionSlopeSlideControl {
public:
    PlayerActionSlopeSlideControl(al::LiveActor*, const PlayerConst*, const PlayerInput*,
                                  const IUsePlayerCollision*);
    void setup();
    void setupCutSlideOppositeDir();
    void update(f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, bool);

private:
    void* filler[0x48 / 8];
};

static_assert(sizeof(PlayerActionSlopeSlideControl) == 0x48);
