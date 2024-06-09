#pragma once

#include <math/seadMatrix.h>

namespace al {
class LiveActor;
class HitSensor;
}  // namespace al

class PlayerRecoverySafetyPoint;
class HackCap;
class PlayerInput;
class PlayerDamageKeeper;
class IPlayerMoelChanger;
class IUsePlayerHeightCheck;

class PlayerHackKeeper {
public:
    PlayerHackKeeper(al::LiveActor* player, HackCap* cap, PlayerRecoverySafetyPoint* safetyPoint,
                     const PlayerInput* input, const sead::Matrix34f* mtx,
                     PlayerDamageKeeper* damageKeeper, const IPlayerModelChanger* modelChanger,
                     const IUsePlayerHeightCheck* heightCheck);

    al::LiveActor* getCurrentHackActor() const { return mCurrentHackActor; }
    al::HitSensor* getUnkHitSensor() const { return mUnkHitSensor; }

private:
    char padding[0x68];
    al::LiveActor* mCurrentHackActor;
    al::HitSensor* mUnkHitSensor;
};
