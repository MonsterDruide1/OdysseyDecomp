#pragma once

#include "Library/LiveActor/LiveActor.h"

class Donsuke : public al::LiveActor {
public:
    Donsuke(const char*);

    void init(const al::ActorInitInfo& info) override;
    void endClipped() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void control() override;

    void exeWait();
    void exeFind();
    void mainMove();
    void exeSideStep();
    void exeRunFront();
    void startActionRunFront();
    void exeRunBack();
    void exeSideStepUnwilling();
    void exeRunFrontUnwilling();
    void exeTurn();
    void exeAttackSign();
    void exeAttack();
    void exeAttackImpact();
    void exeAttackWait();
    void exeAttackEndSign();
    void exeAttackEnd();
    void exeDamage();
    void exeDown();
    void exeReviveWait();
    void exeAppearSign();

    bool tryCalcPlayerPos();
    bool isAbleAttack();
    bool isAbleMove(s32);
    bool isPlayerPosCenter();
    bool isPlayerPosHead();
    bool isAbleTurn(s32);
    bool isPlayerPosFront();
    bool isPlayerPosAttack();
    bool isPlayerPosAttackCenterLine();
    bool isPlayerPosAttackNotMoveable();
    bool isShouldSideStep(s32);
    bool isPlayerPosRight();
    bool isPlayerPosLeft();
    bool isPlayerPosSide();
    bool isPlayerPosCenterLine();
    bool isPlayerPosBack();

private:
    char _108[0x180 - sizeof(al::LiveActor)];
};
