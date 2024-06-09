#pragma once

#include "Library/LiveActor/LiveActor.h"
#include "Project/HitSensor/HitSensor.h"

class PlayerRecoverySafetyPoint;
class HackCap;
class PlayerInput;
class PlayerDamageKeeper;
class IPlayerModelChanger;
class IUsePlayerHeightCheck;

class PlayerHackKeeper {
public:
    PlayerHackKeeper(al::LiveActor* player, HackCap* cap, PlayerRecoverySafetyPoint* safetyPoint,
                     const PlayerInput* input, const sead::Matrix34f* mtx,
                     PlayerDamageKeeper* damageKeeper, const IPlayerModelChanger* modelChanger,
                     const IUsePlayerHeightCheck* heightCheck);

    inline al::LiveActor* getCurrentHackActor() const { return mCurrentHackActor; }
    inline al::HitSensor* getUnkHitSensor() const { return mUnkHitSensor; }

private:
    char padding[0x68];
    al::LiveActor* mCurrentHackActor;
    al::HitSensor* mUnkHitSensor;
};
