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

    void createEnemyCap(const al::ActorInitInfo&, const char*);
    bool tryReceiveMsgCapBlow(const al::SensorMsg*, al::HitSensor*, al::HitSensor*);
    bool isCapOn() const;
    void blowCap(al::HitSensor*);
    void resetCap();
    void makeActorDeadCap();

    void exeWait();
    void exeDamageCap();

    const EnemyCap* getEnemyCap() { return mEnemyCap; };

private:
    EnemyCap* mEnemyCap = nullptr;
};
