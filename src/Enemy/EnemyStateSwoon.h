#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class SensorMsg;
class HitSensor;
class LiveActor;
class WaterSurfaceFinder;
}  // namespace al

struct EnemyStateSwoonInitParam {
    EnemyStateSwoonInitParam(const char* startName, const char* defaultName, const char* endName,
                             const char* trampledName, const char* startFallName,
                             const char* startLandName)
        : mStartName(startName), mDefaultName(defaultName), mEndName(endName),
          mTrampledName(trampledName), mStartFallName(startFallName),
          mStartLandName(startLandName) {}

    const char* mStartName = "SwoonStart";
    const char* mDefaultName = "Swoon";
    const char* mEndName = "SwoonEnd";
    const char* mTrampledName = "SwoonTrampled";
    const char* mStartFallName = "SwoonStartFall";
    const char* mStartLandName = "SwoonStartLand";
    const char* _30 = nullptr;
    const char* _38 = nullptr;
    const char* _40 = nullptr;
    const char* _48 = nullptr;
    const char* _50 = nullptr;
    const char* _58 = nullptr;
    const char* _60 = nullptr;
    const char* _68 = nullptr;
    bool _70 = false;
    bool _71 = true;
    bool _72 = false;
    bool _73 = false;
    s32 _74 = 600;
    s32 _78 = 60;
};

static_assert(sizeof(EnemyStateSwoonInitParam) == 0x80, "EnemyStateSwoonInitParam Size");

class EnemyStateSwoon : public al::ActorStateBase {
public:
    EnemyStateSwoon(al::LiveActor*, const char*, const char*, const char*, bool, bool);

    void appear() override;
    void control() override;

    bool tryReceiveMsgAttack(const al::SensorMsg*);
    bool tryReceiveMsgStartHack(const al::SensorMsg*);
    bool tryReceiveMsgEndSwoon(const al::SensorMsg*);
    bool tryReceiveMsgPressDown(const al::SensorMsg*);
    bool tryReceiveMsgObjHipDropAll(const al::SensorMsg*);
    bool tryReceiveMsgTrample(const al::SensorMsg*);
    bool tryReceiveMsgTrample(const al::SensorMsg*, const al::HitSensor*, al::HitSensor const*);
    bool tryReceiveMsgTrampleReflect(const al::SensorMsg*);
    bool tryReceiveMsgTrampleReflect(const al::SensorMsg*, const al::HitSensor*,
                                     al::HitSensor const*);
    bool tryReceiveMsgObjHipDropReflect(const al::SensorMsg*);
    bool tryReceiveMsgObjLeapFrog(const al::SensorMsg*, const al::HitSensor*, al::HitSensor const*);
    bool tryReceiveMsgEnableLockOn(const al::SensorMsg*);
    bool tryReceiveMsgStartLockOn(const al::SensorMsg*);

    void requestTrampled();
    void initParams(s32, const char*);
    void initParams(const EnemyStateSwoonInitParam&);
    const char* getSwoonStartAnimName() const;
    const char* getSwoonStartLandAnimName() const;
    bool isOnGroundOrWaterSurface() const;
    void tryStartHitReactionLand();
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

    void set9b(bool value) { _9b = value; }

private:
    s32 _20;
    s32 _24;
    const char* _28;
    const char* _30;
    const char* _38;
    const char* _40;
    const char* _48;
    const char* _50;
    const char* _58;
    const char* _60;
    const char* _68;
    const char* _70;
    const char* _78;
    const char* _80;
    const char* _88;
    const char* _90;
    bool _98;
    bool _99;
    bool _9a;
    bool _9b;
    s32 _9c;
    al::WaterSurfaceFinder* mWaterSurfaceFinder;
    bool _a8;
    bool _a9;
};
