#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class TreasureBoxKeyOpener : public al::LiveActor {
public:
    TreasureBoxKeyOpener(const char*);
    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void appear() override;
    void makeActorAlive() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void control() override;
    bool isGot();
    void exeAppear();
    void exeWait();
    void exeGot();
    virtual void setHostForClipping(al::LiveActor*);

private:
    void* filler[4];
};

static_assert(sizeof(TreasureBoxKeyOpener) == 0x128);
