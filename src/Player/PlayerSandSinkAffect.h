#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
}
class IUsePlayerCollision;
class PlayerConst;
class PlayerEffect;
class PlayerInput;

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
    const al::LiveActor* mPlayer;
    const PlayerConst* mConst;
    const PlayerInput* mInput;
    IUsePlayerCollision* mCollider;
    PlayerEffect* mEffect;
    f32 mSinkVelocity;
    f32 mSinkAmount;
    bool mIsSafe;
};

static_assert(sizeof(PlayerSandSinkAffect) == 0x38);
