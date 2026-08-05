#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

class ShineChip : public al::LiveActor {
public:
    ShineChip(const char* name, bool isUseFall, bool isEmpty);

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void makeActorAlive() override;
    void makeActorDead() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void control() override;
    void endClipped() override;

    bool isGot() const;
    void exeAppear();
    void exeWait();
    void exeGot();
    void exeStored();
    void exeGoAppear();

private:
    u8 _108[0x68];
};

static_assert(sizeof(ShineChip) == 0x170);
