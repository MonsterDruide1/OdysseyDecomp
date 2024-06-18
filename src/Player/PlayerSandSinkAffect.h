#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
}
class PlayerConst;
class PlayerInput;
class IUsePlayerCollision;
class PlayerEffect;

class PlayerSandSinkAffect {
public:
    PlayerSandSinkAffect(const al::LiveActor*, const PlayerConst*, const PlayerInput*,
                         IUsePlayerCollision*, PlayerEffect*);
    void clear();
    bool isSink() const;
    void update(bool);
    bool isSinkDeathHeight() const;
    void reduceVelocity(sead::Vector3f*);
    bool isEnableCapThrow() const;
    f32 calcSandSinkDeathRate() const;

private:
    void* filler[0x38 / 8];
};
static_assert(sizeof(PlayerSandSinkAffect) == 0x38);
