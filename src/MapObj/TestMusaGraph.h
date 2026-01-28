#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
class Graph;
}  // namespace al

class TestMusaGraph : public al::LiveActor {
public:
    TestMusaGraph(const char* name);
    void init(const al::ActorInitInfo& info) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void exeWait();

private:
    al::Graph* mGraph = nullptr;
};

static_assert(sizeof(TestMusaGraph) == 0x110);
