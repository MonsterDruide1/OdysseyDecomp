#pragma once

#include "Library/LiveActor/LiveActor.h"
#include "MapObj/CapAcceleratorKeyMoveMapParts.h"

namespace al {
struct ActorInitInfo;
class CapAcceleratorKeyMoveMapParts;
class HitSensor;
class IUseName;
class KeyPoseKeeper;
class MtxConnector;
class NerveAction;
class SensorMsg;
}  // namespace al

class CapAccelerator : public al::LiveActor {
public:
    CapAccelerator(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void control() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;

    void exeWait();
    void exeReaction();
    void exeTrample();
    void exeHit();

private:
    bool mCanReceiveRethrow = false;
    al::MtxConnector* mMtxConnector = nullptr;
    al::CapAcceleratorKeyMoveMapParts* mKeyMoveMapParts = nullptr;
};

static_assert(sizeof(CapAccelerator) == 0x120);
