#pragma once

#include "Library/LiveActor/LiveActor.h"

class CollisionAnimCtrl;

namespace al {
class BreakModel;
class CameraTicket;
}  // namespace al

class MoonBasementFinalGate : public al::LiveActor {
public:
    MoonBasementFinalGate(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void kill() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void startBreakPillar(s32 breakPillarIndex);

    void exeWait();
    void exeBreakPillar();
    void exeBreak();
    void exeBreakLast();
    void exeBreakLastAfter();

private:
    CollisionAnimCtrl* mCollisionAnimCtrl = nullptr;
    al::BreakModel* mBreakModel = nullptr;
    al::CameraTicket* mBreakLastCameraTicket = nullptr;
    s32 mBreakPillarIndex = 0;
    s32 mBreakAfterIndex = -1;
    s32 mBreakLastIndex = -1;
};

static_assert(sizeof(MoonBasementFinalGate) == 0x130);
