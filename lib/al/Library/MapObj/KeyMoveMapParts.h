#pragma once

#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class KeyPoseKeeper;
class RippleCtrl;
class SwitchKeepOnAreaGroup;
class SwitchOnAreaGroup;

class KeyMoveMapParts : public LiveActor {
public:
    KeyMoveMapParts(const char* name);

    void init(const ActorInitInfo& info) override;
    void start();
    void stop();
    void appearAndSetStart();
    bool receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) override;
    void control() override;
    void appearAndSetEnd();

    virtual void exeStandBy();
    void exeDelay();
    virtual void exeWait();
    void setWaitEndNerve();
    void exeMoveSign();
    virtual void exeMove();
    void exeStopSign();
    void exeStop();

    KeyPoseKeeper* getKeyPoseKeeper() const { return mKeyPoseKeeper; }

    void setIsStopKill(bool isStopKill) { mIsStopKill = isStopKill; }

private:
    static const char* getSeNameByIndex(s32 index) {
        switch (index) {
        case 0:
            return "PgMove0";
        case 1:
            return "PgMove1";
        default:
            return nullptr;
        }
    }

    KeyPoseKeeper* mKeyPoseKeeper = nullptr;
    SwitchKeepOnAreaGroup* mSwitchKeepOnAreaGroup = nullptr;
    SwitchOnAreaGroup* mSwitchOnAreaGroup = nullptr;
    RippleCtrl* mRippleCtrl = nullptr;
    sead::Vector3f mClippingOffset = sead::Vector3f::zero;
    s32 mKeyMoveWaitTime = 30;
    s32 mKeyMoveMoveTime = 0;
    s32 mDelayTime = 0;
    bool mIsFloorTouchStart = false;
    bool mIsHipDropStart = false;
    bool mIsStopKill = false;
    const char* mSeMoveName = nullptr;
};

static_assert(sizeof(KeyMoveMapParts) == 0x150);
}  // namespace al
