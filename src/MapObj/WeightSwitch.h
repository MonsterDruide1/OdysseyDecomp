#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class CameraTicket;
class CollisionObj;
class MtxConnector;
}  // namespace al

class Shine;

class WeightSwitch : public al::LiveActor {
public:
    WeightSwitch(const char* actorName);

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void control() override;

    void exeOffWait();
    void exeOn();
    void exeOnDemoWaitStart();
    void exeOnDemoWaitCameraInterpoling();
    void exeOnDemo();
    void exeOnWait();
    void exeOff();

    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

private:
    Shine* mShine = nullptr;
    al::CollisionObj* mCollisionBody = nullptr;
    al::CameraTicket* mDemoCamera = nullptr;
    al::MtxConnector* mMtxConnector = nullptr;
    s32 mWeight = 0;
};
