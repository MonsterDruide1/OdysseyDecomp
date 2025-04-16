#pragma once

#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

#include "Player/IUsePlayerCollision.h"

namespace al {
class HitSensor;
class LiveActor;
class SensorMsg;
struct ActorInitInfo;
}  // namespace al

class IUsePlayerHack;

class KaronWingStateHack : public al::ActorStateBase, public IUsePlayerCollision {
public:
    KaronWingStateHack(al::LiveActor* parent, const al::ActorInitInfo& info,
                       IUsePlayerHack** playerHack);

    PlayerCollider* getPlayerCollider() const override;

    void updateCollider();
    void attackSensor(al::HitSensor* self, al::HitSensor* other);
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self);

    void resetFlyLimit(const sead::Vector3f&);

    bool isEndCancel() const;
    bool isEndReset() const;
    bool isEndDamage() const;

private:
    void* filler[0x48 / 8];
};
