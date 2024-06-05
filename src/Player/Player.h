#pragma once

#include "Library/LiveActor/LiveActor.h"

class Player : public al::LiveActor {
public:
    Player(const char* actorName, const char* archiveName, s32 port);

    void init(const al::ActorInitInfo& info) override;

    void exeWait();
    void exeRun();
    void exeJump();
    void exeFall();
    void exeDamage();

    void control() override;
    void attackSensor(al::HitSensor* target, al::HitSensor* source) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* source,
                    al::HitSensor* target) override;

private:
    const char* mArchiveName = nullptr;
    s32 mPort = 0;
    s32 mAirTime = 0;
};

namespace rs {

bool isGuardNosePainCap(const al::LiveActor*);

}  // namespace rs
