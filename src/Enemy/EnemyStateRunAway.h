#pragma once

#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

struct ParamEnemyStateRunAway {
    f32 field_0;
    f32 field_4;
    f32 field_8;
    f32 field_c;
    f32 field_10;
    bool shouldFaceDir;
};

class EnemyStateRunAway : public al::ActorStateBase {
public:
    EnemyStateRunAway(al::LiveActor* actor, const ParamEnemyStateRunAway* param,
                      const char* animName);

    void appear() override;

    void calcRunDirBase(sead::Vector3f* direction);

    void exeRun();
    void exePanicRun();
    void exePanicRunCollided();

private:
    al::LiveActor* mScaredOfActor = nullptr;
    const ParamEnemyStateRunAway* mParam = nullptr;
    sead::Vector3f mFrontDir = {0.0f, 0.0f, 0.0f};
    const char* mAnimName = nullptr;
    bool mIsCollidedWallVelocity = false;
    bool mIsNeedToFaceToDirection = false;
};
