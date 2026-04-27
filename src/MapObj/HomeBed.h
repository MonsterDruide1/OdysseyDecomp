#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class ChairStateBindSitDown;
class ChairStateBindStandUp;
class FurnitureStateWait;
class IUsePlayerPuppet;

class HomeBed : public al::LiveActor {
public:
    HomeBed(const char* name);

    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void exeWait();
    void exeSnoozeStart();
    void exeSnooze();
    void exeSleepStart();
    void exeSleep();
    void exeStandUp();
    void exeBindJump();
    void exeWaitNoSleep();
    bool tryEndBind();

private:
    IUsePlayerPuppet* mPlayerPuppet = nullptr;
    FurnitureStateWait* mStateWait = nullptr;
    ChairStateBindSitDown* mStateBindSitDown = nullptr;
    ChairStateBindStandUp* mStateBindStandUp = nullptr;
};

static_assert(sizeof(HomeBed) == 0x128);
