#pragma once

#include "Library/LiveActor/LiveActor.h"

class GrowPlantLeaf;

class GrowPlantParts : public al::LiveActor {
public:
    GrowPlantParts(const char* name);

    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void control() override;

    void exeWait();
    void exeGrow();
    void exeGrowEnd();
    void trySetNerveGrow();
    void trySetNerveGrowEnd();

private:
    GrowPlantLeaf* mGrowPlantLeaf = nullptr;
    bool mIsPoleClimbKeep = false;
};

static_assert(sizeof(GrowPlantParts) == 0x118);
