#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class SensorMsg;
class HitSensor;
class LiveActor;
class WaterSurfaceFinder;
}  // namespace al

struct EnemyStateSwoonInitParam {
    EnemyStateSwoonInitParam(const char* startAnimName, const char* loopAnimName,
                             const char* endAnimName, const char* trampledAnimName,
                             const char* startFallAnimName, const char* startLandAnimName)
        : startAnimName(startAnimName), loopAnimName(loopAnimName), endAnimName(endAnimName),
          trampledAnimName(trampledAnimName), startFallAnimName(startFallAnimName),
          startLandAnimName(startLandAnimName) {}

    const char* startAnimName = "SwoonStart";
    const char* loopAnimName = "Swoon";
    const char* endAnimName = "SwoonEnd";
    const char* trampledAnimName = "SwoonTrampled";
    const char* startFallAnimName = "SwoonStartFall";
    const char* startLandAnimName = "SwoonStartLand";
    const char* endSignAnimName = nullptr;
    const char* nearWaterStartAnimName = nullptr;
    const char* nearWaterLoopAnimName = nullptr;
    const char* nearWaterEndAnimName = nullptr;
    const char* nearWaterStartLandAnimName = nullptr;
    const char* nearWaterTrampledAnimName = nullptr;
    const char* hitReactionAnimName = nullptr;
    const char* hitReactionLandAnimName = nullptr;
    bool hasSubActors = false;
    bool hasStartLandAnimation = true;
    bool hasLockOnDelay = false;
    bool isCancelLoopOnProhibitedArea = false;
    s32 swoonDuration = 600;
    s32 endSignDelay = 60;
};

static_assert(sizeof(EnemyStateSwoonInitParam) == 0x80, "EnemyStateSwoonInitParam Size");

class EnemyStateSwoon : public al::ActorStateBase {
public:
    EnemyStateSwoon(al::LiveActor* actor, const char* startAnimName, const char* loopAnimName,
                    const char* endAnimName, bool hasSubActors, bool hasStartLandAnimation);

    void appear() override;
    void control() override;

    bool tryReceiveMsgAttack(const al::SensorMsg* message);
    bool tryReceiveMsgStartHack(const al::SensorMsg* message);
    bool tryReceiveMsgEndSwoon(const al::SensorMsg* message);
    bool tryReceiveMsgPressDown(const al::SensorMsg* message);
    bool tryReceiveMsgObjHipDropAll(const al::SensorMsg* message);
    bool tryReceiveMsgTrample(const al::SensorMsg* message);
    bool tryReceiveMsgTrample(const al::SensorMsg*, const al::HitSensor* other,
                              const al::HitSensor* self);
    bool tryReceiveMsgTrampleReflect(const al::SensorMsg* message);
    bool tryReceiveMsgTrampleReflect(const al::SensorMsg*, const al::HitSensor* other,
                                     const al::HitSensor* self);
    bool tryReceiveMsgObjHipDropReflect(const al::SensorMsg* message);
    bool tryReceiveMsgObjLeapFrog(const al::SensorMsg*, const al::HitSensor* other,
                                  const al::HitSensor* self);
    bool tryReceiveMsgEnableLockOn(const al::SensorMsg* message);
    bool tryReceiveMsgStartLockOn(const al::SensorMsg* message);

    bool requestTrampled();
    void initParams(s32 swoonDuration, const char* trampledAnimName);
    void initParams(const EnemyStateSwoonInitParam& initParam);
    const char* getSwoonStartAnimName() const;
    const char* getSwoonStartLandAnimName() const;
    bool isOnGroundOrWaterSurface() const;
    bool tryStartHitReactionLand();
    const char* getSwoonLoopAnimName() const;
    bool isPlayingActionIncorrect() const;
    const char* getSwoonEndAnimName() const;
    const char* getSwoonTrampledAnimName() const;

    void exeSwoonStart();
    void exeSwoonStartFall();
    void exeSwoonStartLand();
    void exeSwoonLoop();
    void exeSwoonEndSign();
    void exeSwoonEnd();
    void exeSwoonTrampled();

    void enableLockOnDelay(bool hasLockOnDelay) { mHasLockOnDelay = hasLockOnDelay; }

private:
    s32 mSwoonDuration = 600;
    s32 _24 = 0;  // A delay counter
    const char* mStartAnimName = nullptr;
    const char* mLoopAnimName = nullptr;
    const char* mEndAnimName = nullptr;
    const char* mTrampledAnimName = "SwoonTrampled";
    const char* mStartFallAnimName = "SwoonStartFall";
    const char* mStartLandAnimName = "SwoonStartLand";
    const char* mEndSignAnimName = nullptr;
    const char* mNearWaterStartAnimName = nullptr;
    const char* mNearWaterLoopAnimName = nullptr;
    const char* mNearWaterEndAnimName = nullptr;
    const char* mNearWaterStartLandAnimName = nullptr;
    const char* mNearWaterTrampledAnimName = nullptr;
    const char* mHitReactionAnimName = nullptr;
    const char* mHitReactionLandAnimName = nullptr;
    bool mIsLockOn = false;
    bool mHasSubActors = false;
    bool mHasStartLandAnimation = false;
    bool mHasLockOnDelay = false;
    s32 mEndSignDelay = 60;
    al::WaterSurfaceFinder* mWaterSurfaceFinder = nullptr;
    bool mIsAppearItem = false;
    bool mIsCancelLoopOnProhibitedArea = false;
};
