#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class CollisionObj;
class CameraTicket;
class MtxConnector;
}  // namespace al

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
    al::LiveActor* mDemoActor = nullptr;
    al::CollisionObj* mCollisionBody = nullptr;
    al::CameraTicket* mDemoCamera = nullptr;
    al::MtxConnector* mMtxConnector = nullptr;
    s32 mWeight = 0;
};
