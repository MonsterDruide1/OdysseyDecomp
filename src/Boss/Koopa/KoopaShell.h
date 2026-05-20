#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

#include "Player/IUsePlayerCollision.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class PlayerCollider;

class KoopaShell : public al::LiveActor, public IUsePlayerCollision {
public:
    KoopaShell(const char* name);
    void init(const al::ActorInitInfo& initInfo) override;
    void appear() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void updateCollider() override;
    PlayerCollider* getPlayerCollider() const override;

    void appearRecover();
    bool isDeactive() const;
    void exeAppear();
    void exeSpinStart();
    void exeSpin();
    void exeBlow();
    void exeEnd();
    void exeDeactive();
    void exeRecoverJump();
    void exeRecoverFallSign();
    void exeRecoverFall();
    void exeRecoverLand();

private:
    u8 _110[0x50];
};

static_assert(sizeof(KoopaShell) == 0x160);
