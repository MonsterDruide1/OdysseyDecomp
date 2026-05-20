#pragma once

#include "Library/LiveActor/LiveActor.h"

#include "Demo/IUseDemoSkip.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class Nerve;
class SensorMsg;
}  // namespace al

class BossKnuckle : public al::LiveActor, public IUseDemoSkip {
public:
    BossKnuckle(const char* name);

    void init(const al::ActorInitInfo& initInfo) override;
    void initAfterPlacement() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* msg, al::HitSensor* other, al::HitSensor* self) override;
    void control() override;
    void appear() override;

    bool isFirstDemo() const override;
    bool isEnableSkipDemo() const override;
    void skipDemo() override;

    void finishStartDemo(bool isSkip);
    void exeWait();
    void turnToPlayer();
    void updateBodySyncPos();
    void syncHandIfWaiting();
    void checkHackAndSetNerve();
    void exeRelay();
    void updatePlayerDemo();
    void exeAttackBeatStart();
    void exeAttackBeat();
    void genMummyAgainstPlayer(s32 mummyCount);
    void relayNextNerve(const al::Nerve* nerve, s32 step, bool isImmediate);
    void exeAttackHandFall();
    s32 countAliveIce();
    void exeAttackHandFallFirstPainDemo();
    void exeAttackSideStamp();
    void exeRunAwayStart();
    void exeRunAway();
    void breakAirIce();
    void startPlayerDemo();
    void exeDamage();
    void exeRestoreHand();
    void exeAttackRocketPunch();
    void genMummy(const al::LiveActor* actor, s32 mummyCount);
    void exeReturn();
    void exeBeforeBattleStart();
    void exeBeforeDemoBattleStartWait();
    void exeDemoBattleStart();
    void addDemoActorAll();
    void exeDemoBattleEnd();
    void exeAfterBattleEnd();
    void breakAllIce();
};
