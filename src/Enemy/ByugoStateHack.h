#pragma once

#include <basis/seadTypes.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class ActorCameraTarget;
class HitSensor;
class LiveActor;
class SensorMsg;
}  // namespace al

class CapTargetInfo;
class EnemyStateHackStart;
struct EnemyStateHackStartParam;
class IUsePlayerHack;

class ByugoStateHack : public al::ActorStateBase {
public:
    ByugoStateHack(al::LiveActor* actor, CapTargetInfo* capTargetInfo);

    void appear() override;
    void kill() override;

    bool receiveMsgInitCapTargetInfo(const al::SensorMsg* msg);
    bool receiveMsgHackStart(const al::SensorMsg* msg, al::HitSensor* other, al::HitSensor* self);
    bool receiveMsg(const al::SensorMsg* msg, al::HitSensor* other, al::HitSensor* self);
    bool receiveMsgHackEnd(const al::SensorMsg* msg, al::HitSensor* other, al::HitSensor* self);
    void attackSensor(al::HitSensor* self, al::HitSensor* other);
    f32 calcHackBlowPowerRate() const;
    bool isWideBlow() const;

    void exeStartDemo();
    void exeWait();
    void calcMove();
    bool tryChangeNerveIfTriggerBlow();
    void exeMove();
    void exeBlowStart();
    void exeBlow();
    void exeBlowWide();
    void exeBlowEnd();
    void forceEndHack();

private:
    EnemyStateHackStart* mStateHackStart = nullptr;
    EnemyStateHackStartParam* mStateHackStartParam = nullptr;
    IUsePlayerHack* mPlayerHack = nullptr;
    CapTargetInfo* mCapTargetInfo = nullptr;
    al::ActorCameraTarget* mCameraTarget;
    bool mIsWideBlow = false;
    f32 mBaseTransY = 0.0f;
    s32 mWideBlowFrame = 0;
};

static_assert(sizeof(ByugoStateHack) == 0x58);
