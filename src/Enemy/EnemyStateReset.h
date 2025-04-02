#pragma once

#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
struct ActorInitInfo;
}

class EnemyCap;

class EnemyStateReset : public al::ActorStateBase {
public:
    EnemyStateReset(al::LiveActor*, const al::ActorInitInfo&, EnemyCap*);

    void appear() override;
    void kill() override;

    void exeWait();

private:
    sead::Vector3f mPos = sead::Vector3f::zero;
    sead::Vector3f mRot = sead::Vector3f::zero;
    f32 mValidDistance = 4000.0f;
    bool mIsRevive = true;
    bool mIsInvalidateSensors = false;
    EnemyCap* mEnemyCap = nullptr;
};
