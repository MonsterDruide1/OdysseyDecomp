#pragma once

#include "Library/LiveActor/LiveActor.h"

class MoonWorldCaptureParadeLift : public al::LiveActor {
public:
    MoonWorldCaptureParadeLift(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void start();

    void exeWait();
    void exeMoveSign();
    void exeMove();
    void exeDelay();

private:
    f32 mSpeed = 0.0f;
    s32 mDelay = 0;
    sead::Vector3f mRailPos = sead::Vector3f::zero;
};
