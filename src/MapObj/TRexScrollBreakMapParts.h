#pragma once

#include <container/seadPtrArray.h>

#include "Library/LiveActor/LiveActor.h"

class TRexScrollBreakMapParts : public al::LiveActor {
public:
    TRexScrollBreakMapParts(const char* actorName);

    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void exeWait();
    void exeBreak();

private:
    sead::PtrArray<al::HitSensor> mBreakSensors;
};

static_assert(sizeof(TRexScrollBreakMapParts) == 0x118);
