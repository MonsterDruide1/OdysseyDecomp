#pragma once

#include "Library/Nerve/NerveStateBase.h"

class EnemyStateWander : public al::ActorStateBase {
private:
    const char* mStateName;
    s32 mRandNum = 0;
    f32 mWalkSpeed = -1.0f;
    bool mIsHalfProbability = false;

public:
    EnemyStateWander(al::LiveActor* actor, const char* stateName);
    void appear();

    void exeWait();
    void exeWalk();
    void exeFall();

    bool isWait() const;
    bool isWalk() const;
    bool isFall() const;
    void changeWalkAnim(const char*);
};
