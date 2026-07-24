#pragma once

#include "Library/Obj/SePlayObj.h"

class SePlayObjWithSave : public al::SePlayObj {
public:
    SePlayObjWithSave(const char* name);
    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
};

static_assert(sizeof(SePlayObjWithSave) == 0x1a0);
