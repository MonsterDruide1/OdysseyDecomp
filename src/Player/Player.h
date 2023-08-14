#pragma once

#include "Library/LiveActor/LiveActor.h"

class Player : public al::LiveActor {
public:
    Player(const char* actorName, const char* archiveName, s32 port);

    void init(const al::ActorInitInfo& initInfo) override;
    void control() override;

    void exeWait();
    void exeRun();
    void exeJump();
    void exeFall();
    void exeDamage();

private:
    const char* mArchiveName;
    s32 mPort;
    s32 mAirTime;
};

namespace rs {

bool isGuardNosePainCap(const al::LiveActor*);

}  // namespace rs
