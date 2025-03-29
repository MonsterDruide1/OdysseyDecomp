#pragma once

#include "Library/LiveActor/LiveActor.h"

class KuriboMini : public al::LiveActor {
public:
    KuriboMini(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void makeActorAlive() override;
    void makeActorDead() override;
    void appear() override;
    void kill() override;
    virtual void appearPop();
    virtual void appearPopBack();
    void control() override;
    bool checkSandSinkPrecisely();
    void updateCollider() override;
    void startClipped() override;
    void endClipped() override;
    void noRevive();
    void exeWait();
    void setShiftTypeOnGround(s32);
    bool isPlayerUp();
    void exeWander();
    bool tryShiftDrown();
    void exeTurn();
    void exeFind();
    void exeChaseReady();
    void exeChase();
    void exeStop();
    void exeAttack();
    void exeBlow();
    void exeBlowLand();
    void exeBlowRecover();
    void exePressDown();
    void exeBlowDown();
    void exeFall();
    void exeLand();
    bool tryShiftChaseOrWander();
    void exeSink();
    bool updateSink();
    void exeReset();
    void exeSandGeyser();
    void exeDrown();
    void exeHide();
    void exePopAppearStart();
    void exePopAppear();
    void exePopAppearEnd();
    void exeBind();
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void prepareKillByShineGet();
    bool tryReceiveMsgNormal(const al::SensorMsg* message, al::HitSensor* other,
                             al::HitSensor* self);
    void notifyJumpSink(f32);
    void validateSpecialPush(u32);
    void forceStartClipped();

private:
    void* filler[22];
};
