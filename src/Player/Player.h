#pragma once

#include <basis/seadTypes.h>

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
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

private:
    const char* mArchiveName = nullptr;
    s32 mPort = 0;
    s32 mAirTime = 0;
};

namespace rs {

bool isGuardNosePainCap(const al::LiveActor*);

}  // namespace rs
