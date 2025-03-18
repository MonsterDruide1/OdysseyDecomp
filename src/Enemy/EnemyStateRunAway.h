#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
}  // namespace al

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
    EnemyStateRunAway(al::LiveActor*, const ParamEnemyStateRunAway*, const char*);

    void appear() override;

    void calcRunDirBase(sead::Vector3f*);

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
