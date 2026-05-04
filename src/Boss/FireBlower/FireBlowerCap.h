#pragma once

#include "Library/Obj/PartsModel.h"

namespace al {
struct ActorInitInfo;
class EnemyStateBlowDown;
class HitSensor;
class LiveActor;
}  // namespace al

class FireBlowerCap : public al::PartsModel {
public:
    FireBlowerCap();

    void initCap(al::LiveActor* actor, const al::ActorInitInfo& initInfo);
    void appear() override;
    void disappear(const al::HitSensor* sourceSensor, const al::HitSensor* targetSensor);
    void exeAppear();
    void exeWait();
    void appearResetAttackStart();
    void exeResetAttackStart();
    void exeDisappear();
    bool isDisappear() const;

private:
    al::EnemyStateBlowDown* mStateBlowDown = nullptr;
};

static_assert(sizeof(FireBlowerCap) == 0x150);
