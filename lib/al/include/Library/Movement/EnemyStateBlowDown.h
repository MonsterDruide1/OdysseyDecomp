#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
struct EnemyStateBlowDownParam;
class HitSensor;
class LiveActor;

class EnemyStateBlowDown : public ActorStateBase {
public:
    EnemyStateBlowDown(LiveActor*, const EnemyStateBlowDownParam*, const char*);
    void start(const HitSensor*);
    void start(const sead::Vector3f&);
    void start(const HitSensor*, const HitSensor*);
    void start(const LiveActor*);

    void appear() override;
    void kill() override;
    void control() override;

    void setParam(const EnemyStateBlowDownParam* param) { mParam = param; };

private:
    const EnemyStateBlowDownParam* mParam = nullptr;
    s32 mBlowDownTimer = 0;
    bool mIsInvalidClipping = false;
};
}  // namespace al
