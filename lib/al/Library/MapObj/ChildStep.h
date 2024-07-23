#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class ChildStep : public LiveActor {
public:
    ChildStep(const char* name, LiveActor* parent);

    void init(const ActorInitInfo& info) override;
    bool receiveMsg(const SensorMsg* message, HitSensor* source, HitSensor* target) override;
    void exeWait();

private:
    LiveActor* mParent = nullptr;
    sead::Vector3f mPos = sead::Vector3f::zero;
};

s32 calcChildStepCount(const ActorInitInfo& info);
void tryInitSubActorKeeperChildStep(LiveActor* actor, const ActorInitInfo& info);
void createChildStep(const ActorInitInfo& info, LiveActor* parent, bool isSyncClipping);
}  // namespace al
