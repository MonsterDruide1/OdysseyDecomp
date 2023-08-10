#pragma once

#include "al/Library/Nerve/NerveExecutor.h"

namespace al {
class NerveStateBase : public NerveExecutor {
public:
    NerveStateBase(const char* stateName);
    virtual ~NerveStateBase();

    virtual void init();
    virtual void appear();
    virtual void kill();
    virtual bool update();
    virtual void control();

    bool isDead() const { return mIsDead; }

private:
    bool mIsDead = true;
};

class LiveActor;

class ActorStateBase : public NerveStateBase {
public:
    ActorStateBase(const char* stateName, LiveActor* actor);

private:
    LiveActor* mActor;
};
};  // namespace al
