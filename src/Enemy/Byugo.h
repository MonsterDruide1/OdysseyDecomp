#pragma once

#include "Library/LiveActor/LiveActor.h"

class Byugo : public al::LiveActor {
public:
    Byugo(const char* name);

    void init(const al::ActorInitInfo&) override;
    void initAfterPlacement() override;
    void attackSensor(al::HitSensor*, al::HitSensor*) override;
    void calcBlowPowerRate() const;
    void calcWindForce(sead::Vector3f*, const sead::Vector3f&, f32) const;
    bool receiveMsg(const al::SensorMsg*, al::HitSensor*, al::HitSensor*) override;

    void exeDelay();
    void exeRevive();
    void exeAppear();
    void exeWait();
    bool isMoveAtWait() const;
    bool updateKeyMove();
    void exeBlowSign();
    void exeBlowStart();
    void exeBlow();
    bool isMoveAtBlow() const;
    void sendBlowMsgToCollision();
    void exeBlowEnd();
    void exeReactionBlow();
    void exeDamageCap();
    void exeHack();
    void exeSwoon();
    f32 getSideStart() const;
    f32 getFrontEnd() const;
    f32 getSideEnd() const;
    f32 getFrontStart() const;

private:
    char _108[0x178 - sizeof(al::LiveActor)];
};
