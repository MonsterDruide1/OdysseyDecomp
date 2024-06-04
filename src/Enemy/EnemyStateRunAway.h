#pragma once

#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveStateBase.h"

struct ParamEnemyStateRunAway {
    f32 field_0;
    f32 field_4;
    f32 field_8;
    f32 field_C;
    f32 field_10;
    bool mShouldFaceDir;
};

class EnemyStateRunAway : public al::ActorStateBase {
private:
    al::LiveActor* mScaredOfActor;
    ParamEnemyStateRunAway* mParam;
    sead::Vector3f mFrontDir;
    const char* mAnimName;
    bool mIsCollidedWallVelocity;
    bool mNeedToFaceToDirection;

public:
    EnemyStateRunAway(al::LiveActor*, const ParamEnemyStateRunAway*, const char*);
    void appear();
    void calcRunDirBase(sead::Vector3f*);
    void exeRun();
    void exePanicRun();
    void exePanicRunCollided();
};