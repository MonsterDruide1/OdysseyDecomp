#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class LiveActor;
class SensorMsg;
}  // namespace al

class EnemyCap;

class EnemyStateDamageCap : public al::ActorStateBase {
public:
    EnemyStateDamageCap(al::LiveActor* actor);
    void kill() override;

    void createEnemyCap(const al::ActorInitInfo& info, const char* name);
    bool tryReceiveMsgCapBlow(const al::SensorMsg* msg, al::HitSensor* other, al::HitSensor* self);
    bool isCapOn() const;
    void blowCap(al::HitSensor* source);
    void resetCap();
    void makeActorDeadCap();

    void exeWait();
    void exeDamageCap();

    const EnemyCap* getEnemyCap() { return mEnemyCap; };

private:
    EnemyCap* mEnemyCap = nullptr;
};
