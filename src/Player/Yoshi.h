#pragma once

#include <math/seadMatrix.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class HitSensor;
class SensorMsg;
} // namespace al

class Yoshi : public al::LiveActor {
public:
    Yoshi(const char* actorName);

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void movement() override;
    void attackSensor(al::HitSensor*, al::HitSensor*) override;
    bool receiveMsg(const al::SensorMsg*, al::HitSensor*, al::HitSensor*) override;

    void control() override;
    void updateCollider() override;

    void updateCollisionShape();
    void sendCollisionMsg();
    void appearEgg();
    void startFruitShineGetDemo();
    void exeEgg();
    void exeNpc();
    void exeHack();

private:
    char mData[0x80] = {};
    sead::Matrix34f mLocalMtx = sead::Matrix34f::ident;
};
