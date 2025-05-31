#pragma once

#include "Npc/BirdStateFlyAwayBase.h"

class SmallBirdStateFlyAway : public BirdStateFlyAwayBase {
public:
    SmallBirdStateFlyAway(al::LiveActor* actor);

    void appear() override;
    void startFlyAwayByKoopa() override;

    void exeFlyAway();
    void exeFlyAwayHomeLanding();
    void exeFlyAwayKoopa();

private:
    sead::Vector3f mTargetDir = {0, 0, 0};
    f32 mHorizontalAccel = 0;
    f32 mVerticalAccel = 0;
    sead::Vector3f mTargetAccelDir = {0, 0, 0};
    s32 mDisappearStep = 240;
    s32 mCollisionCheckOffsetStep = 0;
    bool mIsColliding = false;
    f32 mAngle = 0;
};

static_assert(sizeof(SmallBirdStateFlyAway) == 0x68);
