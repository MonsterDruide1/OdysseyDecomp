#pragma once

#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

class FlyerStateWanderParam;

class FlyerStateWander : public al::ActorStateBase {
public:
    FlyerStateWander(al::LiveActor* actor, const FlyerStateWanderParam* param);

    void appear() override;

    void exeWander();
    void exeWait();

public:
    s32 _20 = 0;
    sead::Vector3f _24 = {0.0f, 0.0f, 0.0f};
    const FlyerStateWanderParam* mFlyerStateWanderParam;
};

static_assert(sizeof(FlyerStateWander) == 0x38);
