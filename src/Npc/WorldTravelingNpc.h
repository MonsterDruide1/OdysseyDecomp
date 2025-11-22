#pragma once

#include "Library/Event/IEventFlowEventReceiver.h"
#include "Library/LiveActor/LiveActor.h"

class WorldTravelingNpc : public al::LiveActor, public al::IEventFlowEventReceiver {
public:
    WorldTravelingNpc(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    bool receiveEvent(const al::EventFlowEventData* event) override;

    void exeEvent();

private:
    void* _padding[0x11];
};

static_assert(sizeof(WorldTravelingNpc) == 0x198);
