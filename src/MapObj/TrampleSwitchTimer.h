#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class CollisionObj;
class MtxConnector;
}  // namespace al
class AppearSwitchTimer;

class TrampleSwitchTimer : public al::LiveActor {
public:
    TrampleSwitchTimer(const char* actorName);

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void control() override;

    void exeOn();
    void exeOnWait();
    void exeOff();
    void exeOffWait();
    void exeFreeze();

    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

private:
    bool mIsFacingUp = false;
    al::CollisionObj* mCollisionObj = nullptr;
    al::MtxConnector* mMtxConnector = nullptr;
    AppearSwitchTimer* mAppearSwitchTimer = nullptr;
};
