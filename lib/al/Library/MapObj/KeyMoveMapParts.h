#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class KeyPoseKeeper;
class SwitchKeepOnAreaGroup;
class SwitchOnAreaGroup;
class RippleCtrl;
class FunctorBase;

// TODO: Remove this
bool listenStageSwitchOnStart(IUseStageSwitch* stageSwitchHolder, const FunctorBase& actionOnOn);

class KeyMoveMapParts : public LiveActor {
public:
    KeyMoveMapParts(const char* name);

    void init(const ActorInitInfo& info) override;
    void start();
    void stop();
    void appearAndSetStart();
    bool receiveMsg(const SensorMsg* message, HitSensor* source, HitSensor* target) override;
    void control() override;
    void appearAndSetEnd();

    virtual void exeStandBy();
    void exeDelay();
    virtual void exeWait();
    void exeWaitEndNerve();
    void exeMoveSign();
    virtual void exeMove();
    void exeStopSign();
    void exeStop();

private:
    KeyPoseKeeper* mKeyPoseKeeper = nullptr;
    SwitchKeepOnAreaGroup* mSwitchKeepOnAreaGroup = nullptr;
    SwitchOnAreaGroup* mSwitchOnAreaGroup = nullptr;
    RippleCtrl* mRippleCtrl = nullptr;
    sead::Vector3f _128 = sead::Vector3f::zero;
    s32 mKeyMoveWaitTime = 30;
    s32 mKeyMoveMoveTime = 0;
    s32 mDelayTime = 0;
    bool mIsFloorTouchStart = false;
    bool mIsHipDropStart = false;
    bool mIsStopKill = false;
    char* mSeMoveName = nullptr;
};

static_assert(sizeof(KeyMoveMapParts) == 0x150);
}  // namespace al
