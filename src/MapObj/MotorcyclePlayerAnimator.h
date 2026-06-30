#pragma once

#include <basis/seadTypes.h>

#include "Library/Nerve/NerveExecutor.h"

class IUsePlayerPuppet;

class MotorcyclePlayerAnimator : public al::NerveExecutor {
public:
    MotorcyclePlayerAnimator();

    void update(f32 weightL, f32 weightC, f32 weightR);
    void startBind(IUsePlayerPuppet* playerPuppet);
    void startBindKeepDemo();
    void endBind();
    void startBindRideStartL();
    void startBindRideStartR();
    void startBindRideOn();
    void startBindWait();
    void startBindRideRunStart();
    void startBindRideClash();
    void startBindRideJump();
    bool tryStartBindRideRunIfNotPlaying();
    bool tryStartBindRideLandIfJump();
    void exeNone();
    void exeRideStartL();
    void exeRideStartR();
    void exeRideOn();
    void exeWait();
    void exeRideRunStart();
    void exeRideRun();
    void exeRideClash();
    void exeRideJump();
    void exeRideLand();
    void exeBindKeepDemo();

private:
    IUsePlayerPuppet* mPlayerPuppet = nullptr;
    f32 mWeightL = 0.0f;
    f32 mWeightC = 0.0f;
    f32 mWeightR = 0.0f;
};

static_assert(sizeof(MotorcyclePlayerAnimator) == 0x28);
