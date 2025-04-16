#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
}  // namespace al

class EnemyStateWander : public al::ActorStateBase {
public:
    EnemyStateWander(al::LiveActor* actor, const char* stateName);

    void appear() override;

    void exeWait();
    void exeWalk();
    void exeFall();

    bool isWait() const;
    bool isWalk() const;
    bool isFall() const;
    void changeWalkAnim(const char*);

private:
    const char* mStateName;
    s32 mMaxWaitTime = 0;
    f32 mWalkSpeed = -1.0f;
    bool mIsSwapRotation = false;
};
