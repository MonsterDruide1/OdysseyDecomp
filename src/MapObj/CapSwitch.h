#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class MtxConnector;
}
class CapTargetInfo;
class AppearSwitchSave;

class CapSwitch : public al::LiveActor {
public:
    CapSwitch(const char* name);

    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self) override;
    void initAfterPlacement() override;
    void control() override;

    void listenStart();
    void listenReset();

    void exeOffWait();
    void exeOffWaitCapHold();
    void exeHitReaction();
    void exeOnWait();
    void exeReturnOff();
    void exeOffWaitInvalid();

    bool isOn() const;

private:
    al::MtxConnector* mMtxConnector = nullptr;
    CapTargetInfo* mCapTargetInfo = nullptr;
    AppearSwitchSave* mAppearSwitchSave = nullptr;
    bool mIsFacingFront = true;
    sead::Vector3f mFlyingCapPos = sead::Vector3f::zero;
    sead::Vector3f mPlayerPos = sead::Vector3f::zero;
};
