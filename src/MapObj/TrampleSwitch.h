#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class AddDemoInfo;
class CollisionObj;
class CameraTicket;
class MtxConnector;
}  // namespace al
class AppearSwitchSave;

class TrampleSwitch : public al::LiveActor {
public:
    TrampleSwitch(const char* actorName);

    virtual void init(const al::ActorInitInfo& info) override;

    void offSwitch();
    void resetSwitch();

    virtual void initAfterPlacement() override;
    virtual void control() override;

    void exeOffWait();
    void exeOn();
    void exeOnDemoWaitStart();
    void exeOnDemo();
    void exeOnWait();
    void exeOff();

    bool isOn() const;

    virtual bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                            al::HitSensor* self) override;

    bool trySetNerveOn();

private:
    bool mIsFacingUp = false;
    al::CollisionObj* mCollisionBody = nullptr;
    al::AddDemoInfo* mAddDemoInfo = nullptr;
    al::CameraTicket* mDemoCamera = nullptr;
    al::MtxConnector* mMtxConnector = nullptr;
    AppearSwitchSave* mAppearSwitchSave = nullptr;
};
