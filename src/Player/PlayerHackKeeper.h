#pragma once

#include <math/seadMatrix.h>
#include "Library/LiveActor/LiveActor.h"
#include "Player/IUsePlayerHack.h"

namespace al {
class LiveActor;
class HitSensor;
}  // namespace al

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
                     const PlayerDamageKeeper* damageKeeper, const IPlayerModelChanger* modelChanger,
                     const IUsePlayerHeightCheck* heightCheck);
                     
    void createHackModel(al::ActorInitInfo const&);
    bool executeForceHackStageStart(al::HitSensor *,IUsePlayerHack *);
    const char* getCurrentHackName() const;

    al::LiveActor* getCurrentHackActor() const { return mCurrentHackActor; }
    al::HitSensor* getUnkHitSensor() const { return _70; }
public:
    al::LiveActor *mPlayer;
    HackCap *mCap;
    PlayerRecoverySafetyPoint *mSafetyPoint;
    void *size3[2];
    const PlayerInput *mInput;
    const sead::Matrix34f *_20;
    const PlayerDamageKeeper *mDamageKeeper;
    const IPlayerModelChanger *mModelChanger;
    const IUsePlayerHeightCheck *mHeightCheck;
    al::HitSensor *_50;
    int _58;
    bool _5C;
    bool _5D;
    bool _5E;
    bool pad[1];
    void *size4[1];
    al::LiveActor *mCurrentHackActor = nullptr;
    al::HitSensor* _70 = nullptr;
    void *size2[11];
};
